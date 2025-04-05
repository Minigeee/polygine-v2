#include <loguru.hpp>
#include <ply/ecs/EntityGroup.h>

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

}