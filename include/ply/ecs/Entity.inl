#pragma once

#include <loguru.hpp>
#include <ply/ecs/EntityGroup.h>
#include <ply/ecs/World.h>

namespace ply {

///////////////////////////////////////////////////////////
template <ComponentType C> bool Entity::has() const {
    CHECK_F(m_group != nullptr, "entity is invalid");

    auto it = m_group->m_components.find(typeid(C));
    return it != m_group->m_components.end() && it.value().data(m_index) != nullptr;
}

///////////////////////////////////////////////////////////
template <ComponentType C> C& Entity::get() const {
    CHECK_F(m_group != nullptr, "entity is invalid");

    auto it = m_group->m_components.find(typeid(C));
    auto ptr = it != m_group->m_components.end() ? (C*)it.value().data(m_index) : nullptr;
    CHECK_F(ptr != nullptr, "component %s not found", typeid(C).name());
    return *ptr;
}

///////////////////////////////////////////////////////////
template <ComponentType C> void Entity::add(const C& component) {
    CHECK_F(m_group != nullptr, "entity is invalid");
    m_group->m_mutex.unlock_shared();
    m_world->addComponent(m_id, component);
    m_group->m_mutex.lock_shared();
}

///////////////////////////////////////////////////////////
template <ComponentType C> void Entity::remove() {
    CHECK_F(m_group != nullptr, "entity is invalid");
    m_group->m_mutex.unlock_shared();
    m_world->removeComponent<C>(m_id);
    m_group->m_mutex.lock_shared();
}

}