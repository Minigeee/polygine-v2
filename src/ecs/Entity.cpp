#include <ply/ecs/Entity.h>
#include <ply/ecs/EntityGroup.h>
#include <loguru.hpp>

namespace ply {

///////////////////////////////////////////////////////////
Entity::Entity() : m_world(nullptr), m_group(nullptr), m_index(0), m_id(0) {}

///////////////////////////////////////////////////////////
Entity::Entity(World* world, EntityId id, EntityGroup* group, uint32_t index)
    : m_world(world),
      m_group(group),
      m_index(index),
      m_id(id) {
    // Lock group mutex
    if (m_group)
        m_group->m_mutex.lock_shared();
}

///////////////////////////////////////////////////////////
Entity::~Entity() {
    // Unlock group mutex
    if (m_group)
        m_group->m_mutex.unlock_shared();
}

///////////////////////////////////////////////////////////
void Entity::release() {
    // Unlock group mutex
    m_group->m_mutex.unlock_shared();
    m_world = nullptr;
    m_group = nullptr;
    m_index = 0;
    m_id = 0;
}

///////////////////////////////////////////////////////////
HashMap<std::type_index, void*> Entity::getAll() {
    CHECK_F(m_group != nullptr, "entity is invalid");

    // Create map of pointers
    HashMap<std::type_index, void*> ptrs;
    for (auto it = m_group->m_components.begin(); it != m_group->m_components.end(); ++it)
        ptrs[it.key()] = it.value().data(m_index);

    return ptrs;
}

///////////////////////////////////////////////////////////
EntityId Entity::getId() const {
    return m_id;
}

}