#include <ply/ecs/EntityBuilder.h>
#include <ply/ecs/World.h>

namespace ply {

///////////////////////////////////////////////////////////
HashMap<std::type_index, ObjectPool> EntityBuilder::s_pools;

///////////////////////////////////////////////////////////
EntityBuilder::EntityBuilder() : m_world(0), m_numCreate(0) {}

///////////////////////////////////////////////////////////
EntityBuilder::EntityBuilder(World* world) : m_world(world), m_numCreate(0) {}

///////////////////////////////////////////////////////////
EntityBuilder::EntityBuilder(const EntityBuilder& other)
    : m_world(other.m_world),
      m_numCreate(other.m_numCreate),
      m_components(other.m_components) {
    // Create copies of components
    if (m_components.size() > 0) {
        for (auto it = m_components.begin(); it != m_components.end(); ++it) {
            // Allocate new component in pool, then copy data
            auto& mdata = it.value();
            void* newData = s_pools[it.key()].alloc();

            // Copy old to new
            memcpy(newData, mdata.m_data, mdata.m_size);

            // Update pointer
            mdata.m_data = newData;
        }
    }
}

///////////////////////////////////////////////////////////
EntityBuilder::EntityBuilder(EntityBuilder&& other) noexcept
    : m_world(other.m_world),
      m_numCreate(other.m_numCreate),
      m_components(std::move(other.m_components)) {
    other.m_world = 0;
    other.m_numCreate = 0;
}

///////////////////////////////////////////////////////////
EntityBuilder::~EntityBuilder() {
    // Free components if still allocated
    if (m_components.size() > 0) {
        for (auto it = m_components.begin(); it != m_components.end(); ++it)
            s_pools[it.key()].free(it.value().m_data);

        m_components.clear();
    }
}

///////////////////////////////////////////////////////////
EntityBuilder& EntityBuilder::operator=(const EntityBuilder& other) {
    if (this != &other) {
        m_world = other.m_world;
        m_numCreate = other.m_numCreate;
        m_components = other.m_components;

        // Create copies of components
        if (m_components.size() > 0) {
            for (auto it = m_components.begin(); it != m_components.end(); ++it) {
                // Allocate new component in pool, then copy data
                auto& mdata = it.value();
                void* newData = s_pools[it.key()].alloc();

                // Copy old to new
                memcpy(newData, mdata.m_data, mdata.m_size);

                // Update pointer
                mdata.m_data = newData;
            }
        }
    }

    return *this;
}

///////////////////////////////////////////////////////////
EntityBuilder& EntityBuilder::operator=(EntityBuilder&& other) noexcept {
    if (this != &other) {
        m_world = other.m_world;
        m_numCreate = other.m_numCreate;
        m_components = std::move(other.m_components);

        other.m_world = 0;
        other.m_numCreate = 0;
    }

    return *this;
}

///////////////////////////////////////////////////////////
std::vector<EntityId> EntityBuilder::create(uint32_t num) {
    if (!m_components.size())
        return {};

    // Create
    HashMap<std::type_index, void*> ptrs;
    std::vector<EntityId> ids = createImpl(num, ptrs);

    // Send add event
    sendEvent(ids, ptrs);

    return ids;
}

///////////////////////////////////////////////////////////
std::vector<EntityId>
EntityBuilder::createImpl(uint32_t num, HashMap<std::type_index, void*>& ptrs, bool allowDefer) {
    // Get group hash
    std::vector<std::type_index> typeIds;
    for (auto it = m_components.begin(); it != m_components.end(); ++it)
        typeIds.push_back(it.key());

    EntityGroupId groupId = entityGroupHash(typeIds);

    // Get entity group (create if needed)
    EntityGroup* group = nullptr;
    {
        // Lock group map
        WriteLock worldLock(m_world->m_groupsMutex);
        group = m_world->getOrCreateEntityGroup(groupId, m_components);
    }

    // Store group pointer
    m_group = group;
    m_numCreate = num;

    // Check if we should defer
    bool defer = !group->m_mutex.try_lock();
    if (defer) {
        if (allowDefer) {
            // Add to queue
            m_world->m_addQueue.push_back(new EntityBuilder(std::move(*this)));
            return {};
        } else {
            // If not allowed to defer, then just wait
            defer = false;
            group->m_mutex.lock();
        }
    }

    // Lock access to group
    WriteLock groupLock(group->m_mutex, std::adopt_lock);

    // Create entities (and ids)
    auto& entityData = m_world->m_entities;
    uint32_t startIndex = group->m_entities.size();

    std::vector<EntityId> ids;
    ids.reserve(num);
    for (uint32_t i = 0; i < num; ++i) {
        // Create entity data
        World::EntityData data;
        data.m_group = groupId;
        data.m_index = startIndex + i;
        data.m_isAlive = true;

        // Add to global list of entity data
        Handle handle = entityData.push(data);

        // Add to lists
        group->m_entities.push_back(handle);
        ids.push_back(handle);
    }

    // Copy components to component arrays
    for (auto it = m_components.begin(); it != m_components.end(); ++it)
        // Add component to array, using the allocated data, then store the
        // location of the new component values
        ptrs[it.key()] = group->m_components[it.key()].push(it.value().m_data, num);

    return ids;
}

///////////////////////////////////////////////////////////
void EntityBuilder::sendEvent(
    const std::vector<EntityId>& ids,
    const HashMap<std::type_index, void*>& ptrs
) {
    if (!m_group || ids.size() == 0)
        return;

    ReadLock lock(m_group->m_mutex);
    m_world->sendEntityEvent(World::OnCreate, ids, ptrs, m_group);
    m_world->sendEntityEvent(World::OnEnter, ids, ptrs, m_group);
}

} // namespace ply
