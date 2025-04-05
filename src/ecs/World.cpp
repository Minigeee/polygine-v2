#include <ply/ecs/World.h>

#include <loguru.hpp>

namespace ply {

///////////////////////////////////////////////////////////
World::World() : m_numRemoveQueued(0) {
    // Set up dummy entity
    EntityId id = m_entities.push(EntityData());
    m_entities.remove(id);
    m_entities.push(EntityData());
}

///////////////////////////////////////////////////////////
World::~World() {}

///////////////////////////////////////////////////////////
void World::remove(EntityId entity) {
    // Find the correct group
    EntityGroupId table = m_entities[entity].m_group;

    // Entity removal is protected by mutex
    ReadLock lock(m_groupsMutex);

    auto it = m_groups.find(table);
    if (it == m_groups.end()) {
        LOG_F(WARNING, "could not find entity table");
        return;
    }

    // This function adds it to a remove queue
    it.value()->m_removeQueue.push_back(entity);

    // Increment number of queued entities
    ++m_numRemoveQueued;
}

///////////////////////////////////////////////////////////
void World::removeQueuedEntities() {
    // Skip if none to remove
    if (m_numRemoveQueued == 0)
        return;

    // Entity removal is protected by mutex
    WriteLock groupsLock(m_groupsMutex);

    // Clear all queues
    for (auto it = m_groups.begin(); it != m_groups.end(); ++it) {
        EntityGroup& group = *it.value();
        if (!group.m_removeQueue.size())
            continue;

        // Lock table
        WriteLock entityLock(group.m_mutex);

        // Num being removed
        size_t numRemoved = group.m_removeQueue.size();

        // Allocate temp map to copy components to iterate entities removed listeners
        HashMap<std::type_index, void*> ptrs;
        for (auto cIt = group.m_components.begin(); cIt != group.m_components.end(); ++cIt) {
            // Get component array
            priv::ComponentStore& store = cIt.value();

            // Allocate space for temp block
            uint32_t typeSize = store.getTypeSize();
            uint8_t* block = (uint8_t*)MALLOC_DBG(numRemoved * typeSize);
            ptrs[cIt.key()] = block;
        }

        // Remove each component set one by one
        for (size_t i = 0; i < numRemoved; ++i) {
            // Remove entities using swap-pop (or in this case move-pop)
            // - Find the entity at the back of the array
            // - Update the back's index to be the index of the entity that is being removed
            // - Move the back to the removed's index for each component and the entity array
            // - Pop back for each
            // - Remove the entity from the global entity data array

            // Get back
            EntityId back = group.m_entities.back();

            // Index of entity being removed
            uint32_t index = m_entities[group.m_removeQueue[i]].m_index;
            // Update index of back to be that of the one being removed
            m_entities[back].m_index = index;

            // Swap-pop for entity list in table
            group.m_entities[index] = back;
            group.m_entities.pop_back();

            // Remove component at index for each component array
            for (auto cIt = group.m_components.begin(); cIt != group.m_components.end(); ++cIt) {
                priv::ComponentStore& store = cIt.value();

                // Copy the component into the ptrs map (gets sent to observers)
                uint32_t typeSize = store.getTypeSize();
                uint8_t* block = (uint8_t*)ptrs[cIt.key()];
                memcpy(block + i * typeSize, store.data(index), typeSize);

                // Remove the component
                store.remove(index);
            }

            // Remove entity from main list
            m_entities.remove(group.m_removeQueue[i]);
        }

        // Use ptrs to invoke all entity listeners
        sendEntityEvent(OnRemove, group.m_removeQueue, ptrs, &group);

        // Free temp blocks
        for (auto ptrIt = ptrs.begin(); ptrIt != ptrs.end(); ++ptrIt)
            FREE_DBG(ptrIt.value());

        // Clear queue
        group.m_removeQueue.clear();
    }

    // Reset number of queued
    m_numRemoveQueued = 0;
}

///////////////////////////////////////////////////////////
Observer& World::observer(EntityEventType type) {
    Observer* observer = m_observerPool.alloc();
    m_observers[(uint32_t)type].push_back(observer);
    return *observer;
}

///////////////////////////////////////////////////////////
EntityFactory World::entity() {
    return EntityFactory(this);
}

///////////////////////////////////////////////////////////
QueryFactory World::query() {
    return QueryFactory(this);
}

///////////////////////////////////////////////////////////
void World::tick() {
    removeQueuedEntities();
}

///////////////////////////////////////////////////////////
void World::sendEntityEvent(
    EntityEventType type,
    const std::vector<EntityId>& ids,
    const HashMap<std::type_index, void*>& ptrs,
    EntityGroup* group
) {
    if (ids.size() == 0)
        return;

    auto& listeners = m_observers[(uint32_t)type];

    // Iterate through listeners, and send event to all matching listeners
    for (auto listener : listeners) {
        Observer& q = *listener;

        // Check if matches
        bool match = true;
        for (size_t t = 0; t < q.m_include.size(); ++t)
            match &= ptrs.contains(q.m_include[t]);

        for (size_t t = 0; t < q.m_exclude.size(); ++t)
            match &= !ptrs.contains(q.m_exclude[t]);

        // Only invoke if match
        if (match) {
            // Lock mutexes if provided
            std::vector<std::unique_lock<std::mutex>> locks;
            locks.reserve(q.m_mutexes.size());
            for (size_t m = 0; m < q.m_mutexes.size(); ++m)
                locks.push_back(std::unique_lock<std::mutex>(*q.m_mutexes[m]));

            q.m_iterator(ids, ptrs, this, group);
        }
    }
}

///////////////////////////////////////////////////////////
bool World::matchesForGroup(EntityGroup& group, QueryDescriptor* query) {
    QueryDescriptor& q = *query;

    // Check if matches
    bool match = true;
    for (size_t t = 0; t < q.m_include.size(); ++t)
        match &= group.m_components.contains(q.m_include[t]);

    for (size_t t = 0; t < q.m_exclude.size(); ++t)
        match &= !group.m_components.contains(q.m_exclude[t]);

    return match;
}

///////////////////////////////////////////////////////////
EntityGroup& World::getOrCreateEntityGroup(EntityGroupId id) {
    auto groupIt = m_groups.find(id);
    if (groupIt == m_groups.end()) {
        auto group = std::make_unique<EntityGroup>();
        group->m_id = id;

        // Insert table
        groupIt = m_groups.emplace(std::make_pair(id, std::move(group))).first;
    }

    return *groupIt->second;
}

///////////////////////////////////////////////////////////
QueryFactory* World::registerQuery(QueryFactory* query) {
    auto hash = query->getHash();

    // If query already exists, quit
    auto it = m_queries.find(hash);
    if (it != m_queries.end())
        return it.value();

    // Save new query
    QueryFactory* q = m_queryPool.alloc();
    *q = *query;
    m_queries[hash] = q;

    // Update groups that match
    q->m_groups.clear();
    for (auto it = m_groups.begin(); it != m_groups.end(); ++it) {
        EntityGroup& group = *it.value();
        if (matchesForGroup(group, q))
            q->m_groups.push_back(group.m_id);
    }

    return q;
}

}