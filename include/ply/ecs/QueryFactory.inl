#include <loguru.hpp>
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
template <ComponentType C> QueryFactory& QueryFactory::match() {
    std::type_index type = typeid(C);

    // Check if the list already has type
    bool alreadyContains = false;
    for (auto t : m_include)
        alreadyContains |= t == type;

    // Only add if it doesn't
    if (!alreadyContains)
        m_include.push_back(type);

    return *this;
}

///////////////////////////////////////////////////////////
template <ComponentType C> QueryFactory& QueryFactory::exclude() {
    std::type_index type = typeid(C);

    // Check if the list already has type
    bool alreadyContains = false;
    for (auto t : m_exclude)
        alreadyContains |= t == type;

    // Only add if it doesn't
    if (!alreadyContains)
        m_exclude.push_back(type);

    return *this;
}

} // namespace ply
