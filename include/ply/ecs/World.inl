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

    // Lock group
    WriteLock oldGroupLock(group->m_mutex);
    // Don't add if component already exists
    if (group->m_components.contains(typeId))
        return;

    // Get list of component types
    auto& componentMap = group->m_components;
    std::vector<std::type_index> types;
    types.reserve(componentMap.size());
    for (auto it = componentMap.begin(); it != componentMap.end(); ++it)
        types.push_back(it.key());

    types.push_back(typeId);

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
            componentMetaMap[typeId] = priv::ComponentMetadata(NULL, sizeof(C), alignof(C));

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
        newComponents[typeId].push(&component, 1);
        ptrs[typeId] = newComponents[typeId].data(newGroup->m_entities.size() - 1);

        // Manage components
        for (auto it = componentMap.begin(); it != componentMap.end(); ++it) {
            auto& componentStore = newComponents[it.key()];

            // Add components to new group
            componentStore.push(it.value().data(oldIndex), 1);
            ptrs[it.key()] = componentStore.data(componentStore.size() - 1);

            // Remove components from old group
            it.value().remove(oldIndex);
        }
    }

    // Send events
    std::vector<EntityId> ids = {id};
    sendEntityEvent(OnExit, ids, ptrs, group);
    sendEntityEvent(OnEnter, ids, ptrs, newGroup);
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

    // Lock group
    WriteLock oldGroupLock(group->m_mutex);
    // Don't remove if component doesn't exist
    if (!group->m_components.contains(typeId))
        return;

    // Get list of component types
    auto& componentMap = group->m_components;
    std::vector<std::type_index> types;
    types.reserve(componentMap.size());
    for (auto it = componentMap.begin(); it != componentMap.end(); ++it) {
        if (it.key() == typeId)
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
                if (it.key() == typeId)
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
            if (it.key() == typeId) {
                // Save NULL pointer for removed component
                ptrs[typeId] = NULL;
            } else {
                // Add components to new group
                auto& componentStore = newGroup->m_components[it.key()];
                componentStore.push(it.value().data(oldIndex), 1);
                ptrs[it.key()] = componentStore.data(componentStore.size() - 1);

                // Remove components from old group
                it.value().remove(oldIndex);
            }
        }
    }

    // Send events
    std::vector<EntityId> ids = {id};
    sendEntityEvent(OnExit, ids, ptrs, group);
    sendEntityEvent(OnEnter, ids, ptrs, newGroup);
}

}
