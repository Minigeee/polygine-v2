#pragma once

#include <functional>
#include <ply/core/Types.h>
#include <ply/ecs/QueryFactory.h>
#include <type_traits>

namespace ply {

class World;

///////////////////////////////////////////////////////////
/// \brief An iterable query
///
///////////////////////////////////////////////////////////
class Query {
public:
    Query() = default;
    Query(World* world, QueryFactory* factory);
    
    ///////////////////////////////////////////////////////////
    /// \brief Iterate through all components that match query
    ///
    ///////////////////////////////////////////////////////////
    template <typename Func> void each(Func&& fn);

private:
    ///////////////////////////////////////////////////////////
    /// \brief Actual iterator implementation
    ///////////////////////////////////////////////////////////
    template <typename Func, typename... Cs>
    void iterate(Func&& fn, type_wrapper<std::tuple<Cs...>>);

private:
    World* m_world;          //!< World pointer used to create new accessors
    QueryFactory* m_factory; //!< Factory used to create new accessors
};

} // namespace ply

#include <ply/ecs/Query.inl>