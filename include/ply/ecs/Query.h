#pragma once

#include <ply/core/Types.h>
#include <ply/ecs/QueryBase.h>

#include <functional>
#include <type_traits>

namespace ply {

class World;

///////////////////////////////////////////////////////////
/// \brief A query builder
///
///////////////////////////////////////////////////////////
class QueryFactory : public QueryBase {
    friend World;
    friend Query;

public:
    QueryFactory() = default;
    QueryFactory(World* world);

    ///////////////////////////////////////////////////////////
    /// \brief Add a mutex to lock before iterating comopnents
    ///
    /// The mutex will be locked before any callback functions are called.
    ///
    /// \param mutex The mutex to lock
    ///
    ///////////////////////////////////////////////////////////
    QueryFactory& lock(std::mutex& mutex);

    ///////////////////////////////////////////////////////////
    /// \brief Set the which component types should be included in the component query
    ///
    /// \param include The type set to include
    ///
    ///////////////////////////////////////////////////////////
    QueryFactory& match(const TypeSet& include);

    ///////////////////////////////////////////////////////////
    /// \brief Add a type to the include type set
    ///
    ///////////////////////////////////////////////////////////
    template <ComponentType... Cs> QueryFactory& match();

    ///////////////////////////////////////////////////////////
    /// \brief Set the which component types should be excluded from the component query
    ///
    /// \param exclude The type set to exclude
    ///
    ///////////////////////////////////////////////////////////
    QueryFactory& exclude(const TypeSet& exclude);

    ///////////////////////////////////////////////////////////
    /// \brief Add a type to the exclude type set
    ///
    ///////////////////////////////////////////////////////////
    template <ComponentType... Cs> QueryFactory& exclude();

    ///////////////////////////////////////////////////////////
    /// \brief Compile the query
    ///
    ///////////////////////////////////////////////////////////
    Query compile();

private:
    World* m_world;                      //!< World access to iterate components
    std::vector<EntityGroupId> m_groups; //!< List of entity groups that match query
};

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

#ifndef PLY_ECS_WORLD_H
#include <ply/ecs/Query.inl>
#endif
