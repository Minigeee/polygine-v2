#pragma once

#include <ply/core/Mutex.h>
#include <ply/core/Types.h>
#include <ply/ecs/ComponentStore.h>
#include <ply/ecs/Entity.h>

#include <typeindex>

namespace ply {

///////////////////////////////////////////////////////////
/// \brief Type of the id for entity groups
///
///////////////////////////////////////////////////////////
using EntityGroupId = uint32_t;

///////////////////////////////////////////////////////////
/// \brief Container structure for entities with the same set of components
///
///////////////////////////////////////////////////////////
struct EntityGroup {
    EntityGroupId m_id;                                           //!< The id of the group
    SharedMutex m_mutex;                                          //!< Mutex protecting access to entity group
    HashMap<std::type_index, priv::ComponentStore> m_components;  //!< The set of components entities of this group have
    std::vector<EntityId> m_entities;                             //!< A list of entity ids in this group that matches the order their components appear in the component arrays
    std::vector<EntityId> m_removeQueue;                          //!< List of entity handles to remove from this group
};

///////////////////////////////////////////////////////////
/// \brief Get hash from a list of type indexes
///
///////////////////////////////////////////////////////////
EntityGroupId entityGroupHash(const std::vector<std::type_index>& ids);

}  // namespace ply
