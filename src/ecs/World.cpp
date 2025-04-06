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
World::~World() {
    // Free all queued entities
    for (auto factory : m_addQueue) {
        delete factory;
    }

    // Free all component changes
    for (auto it = m_changeQueue.begin(); it != m_changeQueue.end(); ++it) {
        if (it->m_component)
            FREE_DBG(it->m_component);
    }

    m_addQueue.clear();
    m_changeQueue.clear();
}

///////////////////////////////////////////////////////////
void World::remove(EntityId entity) {
    // Find the correct group
    EntityGroupId groupId = m_entities[entity].m_group;

    // Entity removal is protected by mutex
    ReadLock lock(m_groupsMutex);

    auto it = m_groups.find(groupId);
    if (it == m_groups.end()) {
        LOG_F(WARNING, "could not find entity group");
        return;
    }

    EntityGroup* group = it.value().get();

    // Check if we need to defer
    bool defer = !group->m_mutex.try_lock();

    if (defer) {
        // Add to remove queue if mutex not available
        group->m_removeQueue.push_back(entity);

        // Increment number of queued entities
        ++m_numRemoveQueued;
    } else {
        // Replace manual lock with lock object
        std::unique_lock<std::shared_mutex> lock(group->m_mutex, std::adopt_lock);

        // Get entity index
        uint32_t index = m_entities[entity].m_index;

        // Get back entity (for swap-pop operation)
        EntityId back = group->m_entities.back();

        // Update index of back entity to be that of the one being removed
        m_entities[back].m_index = index;

        // Swap-pop for entity list in table
        group->m_entities[index] = back;
        group->m_entities.pop_back();

        // Allocate temp map to copy components for entity removed listeners
        HashMap<std::type_index, void*> ptrs;
        for (auto cIt = group->m_components.begin(); cIt != group->m_components.end(); ++cIt) {
            // Get component array
            priv::ComponentStore& store = cIt.value();

            // Allocate space for temp block (just one entity)
            uint32_t typeSize = store.getTypeSize();
            uint8_t* block = (uint8_t*)MALLOC_DBG(typeSize);
            ptrs[cIt.key()] = block;

            // Copy the component into the ptrs map (gets sent to observers)
            memcpy(block, store.data(index), typeSize);

            // Remove the component
            store.remove(index);
        }

        // Remove entity from main list
        m_entities.remove(entity);

        // Create a vector with just this entity for event sending
        std::vector<EntityId> entities = {entity};

        // Use ptrs to invoke all entity listeners
        sendEntityEvent(OnExit, entities, ptrs, group);
        sendEntityEvent(OnRemove, entities, ptrs, group);

        // Free temp blocks
        for (auto ptrIt = ptrs.begin(); ptrIt != ptrs.end(); ++ptrIt)
            FREE_DBG(ptrIt.value());
    }
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
        sendEntityEvent(OnExit, group.m_removeQueue, ptrs, &group);
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
Entity World::getEntity(EntityId id) {
    ReadLock lock(m_groupsMutex);

    const auto& entityData = m_entities[id];
    auto groupIt = m_groups.find(entityData.m_group);
    if (groupIt == m_groups.end())
        return Entity();

    return Entity(this, id, groupIt.value().get(), entityData.m_index);
}

///////////////////////////////////////////////////////////
Observer& World::observer(EntityEventType type) {
    Observer* observer = m_observerPool.alloc();
    observer->m_world = this;

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
    addQueuedEntities();
    changeQueuedEntities();
}

///////////////////////////////////////////////////////////
void World::addComponent(
    EntityGroup* group,
    EntityId id,
    std::type_index type,
    void* component,
    size_t size,
    size_t align
) {
    // Get entity data
    auto& data = m_entities[id];

    // Don't add if component already exists
    if (group->m_components.contains(type))
        return;

    // Get list of component types
    auto& componentMap = group->m_components;
    std::vector<std::type_index> types;
    types.reserve(componentMap.size());
    for (auto it = componentMap.begin(); it != componentMap.end(); ++it)
        types.push_back(it.key());

    types.push_back(type);

    // Get new group hash
    EntityGroupId newGroupId = entityGroupHash(types);

    // Get new group (create if needed)
    EntityGroup* newGroup = nullptr;
    {
        // Lock group map
        WriteLock lock(m_groupsMutex);

        // Perform search first so that we don't have to create component meta map if not needed
        auto it = m_groups.find(newGroupId);
        if (it != m_groups.end()) {
            newGroup = it.value().get();
        }

        else {
            // Create component map
            HashMap<std::type_index, priv::ComponentMetadata> componentMetaMap;
            for (auto it = componentMap.begin(); it != componentMap.end(); ++it)
                componentMetaMap[it.key()] = priv::ComponentMetadata(
                    NULL, it.value().getTypeSize(), it.value().getTypeAlign()
                );
            componentMetaMap[type] = priv::ComponentMetadata(NULL, size, align);

            newGroup = getOrCreateEntityGroup(newGroupId, componentMetaMap);
        }
    }

    HashMap<std::type_index, void*> ptrs;
    {
        // Lock new group
        WriteLock newGroupLock(newGroup->m_mutex);

        // Update entity data
        size_t oldIndex = data.m_index;
        data.m_group = newGroupId;
        data.m_index = newGroup->m_entities.size();

        // Add entity to new group
        newGroup->m_entities.push_back(id);
        // Remove entity from old group
        group->m_entities[oldIndex] = group->m_entities.back();
        group->m_entities.pop_back();

        // Add new component
        auto& newComponents = newGroup->m_components;
        newComponents[type].push(&component, 1);
        ptrs[type] = newComponents[type].data(newGroup->m_entities.size() - 1);

        // Manage components
        for (auto it = componentMap.begin(); it != componentMap.end(); ++it) {
            auto& componentStore = newComponents[it.key()];

            // Add components to new group
            componentStore.push(it.value().data(oldIndex), 1);
            ptrs[it.key()] = componentStore.data(componentStore.size() - 1);

            // Remove components from old group
            it.value().remove(oldIndex);
        }

        // Send events
        dispatchEntityChangeEvents(id, ptrs, group, newGroup);
    }
}

///////////////////////////////////////////////////////////
void World::removeComponent(EntityGroup* group, EntityId id, std::type_index type) {
    // Get entity data
    auto& data = m_entities[id];

    // Don't remove if component doesn't exist
    if (!group->m_components.contains(type))
        return;

    // Get list of component types
    auto& componentMap = group->m_components;
    std::vector<std::type_index> types;
    types.reserve(componentMap.size());
    for (auto it = componentMap.begin(); it != componentMap.end(); ++it) {
        if (it.key() == type)
            continue;

        types.push_back(it.key());
    }

    // Get new group hash
    EntityGroupId newGroupId = entityGroupHash(types);

    // Get new group (create if needed)
    EntityGroup* newGroup = nullptr;
    {
        // Lock group map
        WriteLock lock(m_groupsMutex);

        // Perform search first so that we don't have to create component meta map if not needed
        auto it = m_groups.find(newGroupId);
        if (it != m_groups.end()) {
            newGroup = it.value().get();
        }

        else {
            // Create component map
            HashMap<std::type_index, priv::ComponentMetadata> componentMetaMap;
            for (auto it = componentMap.begin(); it != componentMap.end(); ++it) {
                if (it.key() == type)
                    continue;

                componentMetaMap[it.key()] = priv::ComponentMetadata(
                    NULL, it.value().getTypeSize(), it.value().getTypeAlign()
                );
            }

            newGroup = getOrCreateEntityGroup(newGroupId, componentMetaMap);
        }
    }

    HashMap<std::type_index, void*> ptrs;
    {
        // Lock new group
        WriteLock newGroupLock(newGroup->m_mutex);

        // Update entity data
        size_t oldIndex = data.m_index;
        data.m_group = newGroupId;
        data.m_index = newGroup->m_entities.size();

        // Add entity to new group
        newGroup->m_entities.push_back(id);
        // Remove entity from old group
        group->m_entities[oldIndex] = group->m_entities.back();
        group->m_entities.pop_back();

        // Manage components
        for (auto it = componentMap.begin(); it != componentMap.end(); ++it) {
            if (it.key() == type) {
                // Save NULL pointer for removed component
                ptrs[type] = NULL;
            } else {
                // Add components to new group
                auto& componentStore = newGroup->m_components[it.key()];
                componentStore.push(it.value().data(oldIndex), 1);
                ptrs[it.key()] = componentStore.data(componentStore.size() - 1);

                // Remove components from old group
                it.value().remove(oldIndex);
            }
        }

        // Send events
        dispatchEntityChangeEvents(id, ptrs, group, newGroup);
    }
}

///////////////////////////////////////////////////////////
void World::dispatchEntityChangeEvents(
    EntityId id,
    const HashMap<std::type_index, void*>& ptrs,
    EntityGroup* oldGroup,
    EntityGroup* newGroup
) {
    // Detect enter queries
    auto& enterQueries = m_observers[(uint32_t)EntityEventType::OnEnter];
    for (auto observer : enterQueries) {
        Observer& q = *observer;
        if (q.m_watchGroups.contains(newGroup->m_id) && !q.m_watchGroups.contains(oldGroup->m_id)) {
            // Lock mutexes if provided
            std::vector<std::unique_lock<std::mutex>> locks;
            locks.reserve(q.m_mutexes.size());
            for (size_t m = 0; m < q.m_mutexes.size(); ++m)
                locks.push_back(std::unique_lock<std::mutex>(*q.m_mutexes[m]));

            // Invoke observer
            q.m_iterator({id}, ptrs, this, newGroup);
        }
    }

    // Detect exit queries
    auto& exitQueries = m_observers[(uint32_t)EntityEventType::OnExit];
    for (auto observer : exitQueries) {
        Observer& q = *observer;
        if (q.m_watchGroups.contains(oldGroup->m_id) && !q.m_watchGroups.contains(newGroup->m_id)) {
            // Lock mutexes if provided
            std::vector<std::unique_lock<std::mutex>> locks;
            locks.reserve(q.m_mutexes.size());
            for (size_t m = 0; m < q.m_mutexes.size(); ++m)
                locks.push_back(std::unique_lock<std::mutex>(*q.m_mutexes[m]));

            // Invoke observer
            q.m_iterator({id}, ptrs, this, newGroup);
        }
    }
}

///////////////////////////////////////////////////////////
void World::addQueuedEntities() {
    // Process all entity creation requests
    for (auto factory : m_addQueue) {
        // Create entities (and ids)
        HashMap<std::type_index, void*> ptrs;
        std::vector<EntityId> ids = factory->createImpl(factory->m_numCreate, ptrs, false);

        // Send add event
        factory->sendEvent(ids, ptrs);

        // Free factory
        delete factory;
    }

    // Clear queue
    m_addQueue.clear();
}

///////////////////////////////////////////////////////////
void World::changeQueuedEntities() {
    // Process all component changes
    for (auto& change : m_changeQueue) {
        // Get entity data
        auto& data = m_entities[change.m_id];

        // Get group
        EntityGroup* group = nullptr;
        {
            // Lock group map
            ReadLock lock(m_groupsMutex);
            group = m_groups.find(data.m_group).value().get();
        }
        CHECK_F(group != NULL, "entity group not found");

        // Lock group
        WriteLock lock(group->m_mutex);

        if (change.m_component) {
            // Add component
            addComponent(
                group, change.m_id, change.m_type, change.m_component, change.m_size, change.m_align
            );

            // Free
            FREE_DBG(change.m_component);
        } else {
            // Remove component
            removeComponent(group, change.m_id, change.m_type);
        }
    }

    // Clear component changes
    m_changeQueue.clear();
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

        // Only invoke if part of observer's watch list
        if (q.m_watchGroups.contains(group->m_id)) {
            // Lock mutexes if provided
            std::vector<std::unique_lock<std::mutex>> locks;
            locks.reserve(q.m_mutexes.size());
            for (size_t m = 0; m < q.m_mutexes.size(); ++m)
                locks.push_back(std::unique_lock<std::mutex>(*q.m_mutexes[m]));

            // Invoke observer
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
EntityGroup* World::getOrCreateEntityGroup(
    EntityGroupId id,
    const HashMap<std::type_index, priv::ComponentMetadata>& components
) {
    auto groupIt = m_groups.find(id);
    if (groupIt == m_groups.end()) {
        // Insert table
        groupIt = m_groups.emplace(std::make_pair(id, std::make_unique<EntityGroup>())).first;

        auto group = groupIt->second.get();
        group->m_id = id;

        // Create component arrays
        for (auto it = components.begin(); it != components.end(); ++it) {
            const priv::ComponentMetadata& meta = it.value();
            group->m_components[it.key()] = priv::ComponentStore(meta.m_size, meta.m_align);
        }

        // Register group with observers
        for (size_t i = 0; i < EntityEventType::NUM_EVENTS; ++i) {
            for (size_t o = 0; o < m_observers[i].size(); ++o) {
                Observer* observer = m_observers[i][o];
                if (matchesForGroup(*group, observer))
                    observer->m_watchGroups.insert(id);
            }
        }

        // Register group with queries
        for (auto it = m_queries.begin(); it != m_queries.end(); ++it) {
            QueryFactory* q = it.value();
            if (matchesForGroup(*group, q))
                q->m_groups.push_back(id);
        }
    }

    return groupIt->second.get();
}

///////////////////////////////////////////////////////////
void World::registerObserver(Observer* observer) {
    ReadLock lock(m_groupsMutex);

    // Create set of groups to watch
    for (auto it = m_groups.begin(); it != m_groups.end(); ++it) {
        EntityGroup* group = it.value().get();
        if (matchesForGroup(*group, observer))
            observer->m_watchGroups.insert(it.key());
    }
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

///////////////////////////////////////////////////////////
World::ComponentChange::ComponentChange(EntityId id, std::type_index type)
    : m_id(id),
      m_type(type) {}

}