#include <ply/core/Macros.h>

namespace ply {

///////////////////////////////////////////////////////////
template <typename... Cs>
inline TypeSet TypeSet::create() {
    TypeSet set;
    set.set<Cs...>();
    return set;
}

///////////////////////////////////////////////////////////
template <typename... Cs>
inline void TypeSet::set() {
    // Clear the set
    m_set.clear();

    // Insert all type ids
    PARAM_EXPAND(m_set.insert(typeid(Cs)));
}

///////////////////////////////////////////////////////////
template <typename C>
inline void TypeSet::add() {
    // Insert type id
    m_set.insert(typeid(C));
}

///////////////////////////////////////////////////////////
template <typename C>
inline void TypeSet::remove() {
    // Remove type id
    m_set.erase(typeid(C));
}

///////////////////////////////////////////////////////////
template <typename C>
inline bool TypeSet::has() const {
    return m_set.find(typeid(C)) != m_set.end();
}

}  // namespace ply
