#include <loguru.hpp>

namespace ply {

///////////////////////////////////////////////////////////
template <ComponentType C> void World::addComponent(EntityId id, const C& component) {
    // Get entity data
    auto& data = m_entities[id];
    auto typeId = std::type_index(typeid(C));

    // Get group
    EntityGroup* group = nullptr;
    {
        // Lock group map
        ReadLock lock(m_groupsMutex);
        group = m_groups.find(data.m_group).value().get();
    }
    CHECK_F(group != NULL, "entity group not found");

    // Check if we should defer
    bool defer = !group->m_mutex.try_lock();

    if (defer) {
        // Allocate space to store component
        uint8_t* block = (uint8_t*)MALLOC_DBG(sizeof(C));
        memcpy(block, &component, sizeof(C));

        // Add to queue
        ComponentChange change(id, typeId);
        change.m_component = (void*)block;
        change.m_size = sizeof(C);
        change.m_align = alignof(C);
        m_changeQueue.push_back(change);
    } else {
        // Lock group
        WriteLock oldGroupLock(group->m_mutex, std::adopt_lock);

        // Add component
        addComponent(
            group, id, typeId, (void*)&component, sizeof(C), alignof(C)
        );
    }
}

///////////////////////////////////////////////////////////
template <ComponentType C> void World::removeComponent(EntityId id) {
    // Get entity data
    auto& data = m_entities[id];
    auto typeId = std::type_index(typeid(C));

    // Get group
    EntityGroup* group = NULL;
    {
        // Lock group map
        ReadLock lock(m_groupsMutex);
        group = m_groups.find(data.m_group).value().get();
    }
    CHECK_F(group != NULL, "entity group not found");

    // Check if we should defer
    bool defer = !group->m_mutex.try_lock();

    if (defer) {
        // Add to queue
        ComponentChange change(id, typeId);
        change.m_component = NULL;
        m_changeQueue.push_back(change);
    } else {
        // Lock group
        WriteLock oldGroupLock(group->m_mutex, std::adopt_lock);

        // Remove component
        removeComponent(group, id, typeId);
    }
}

}
