#pragma once

#include <loguru.hpp>
#include <ply/core/Macros.h>
#include <ply/core/Tuple.h>

namespace ply {

///////////////////////////////////////////////////////////
template <ComponentType C> bool QueryAccessor::has() const {
    auto it = m_group->m_components.find(typeid(C));
    return it != m_group->m_components.end() && it.value().data(m_entityIdx) != nullptr;
}

///////////////////////////////////////////////////////////
template <ComponentType C> C& QueryAccessor::get() const {
    auto it = m_group->m_components.find(typeid(C));
    auto ptr = it != m_group->m_components.end() ? (C*)it.value().data(m_entityIdx) : nullptr;
    CHECK_F(ptr != nullptr, "component %s not found", typeid(C).name());
    return *ptr;
}

///////////////////////////////////////////////////////////
template <ComponentType C> void QueryAccessor::add(const C& component) {
    m_world->addComponent(id, component);
}

///////////////////////////////////////////////////////////
template <ComponentType C> void QueryAccessor::remove() {
    m_world->removeComponent<C>(id);
}

///////////////////////////////////////////////////////////
template <ComponentType... Cs> QueryFactory& QueryFactory::match() {
    PARAM_EXPAND(addInclude(typeid(Cs)));
    return *this;
}

///////////////////////////////////////////////////////////
template <ComponentType... Cs> QueryFactory& QueryFactory::exclude() {
    PARAM_EXPAND(addExclude(typeid(Cs)));
    return *this;
}

} // namespace ply
