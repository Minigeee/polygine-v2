#pragma once

#include <functional>
#include <ply/core/Mutex.h>
#include <ply/core/TypeSet.h>
#include <ply/core/Types.h>
#include <ply/ecs/Entity.h>
#include <ply/ecs/EntityGroup.h>
#include <type_traits>
#include <vector>

namespace ply {

class World;
class Query;

///////////////////////////////////////////////////////////
/// \brief Used to access entity components during a query
///
///////////////////////////////////////////////////////////
struct QueryAccessor {
    ///////////////////////////////////////////////////////////
    /// \brief Constructor
    ///////////////////////////////////////////////////////////
    QueryAccessor(World* world, EntityGroup* group, uint32_t entityIdx);

    ///////////////////////////////////////////////////////////
    /// \brief Check if the current entity has a component
    ///
    ///////////////////////////////////////////////////////////
    template <ComponentType C> bool has() const;

    ///////////////////////////////////////////////////////////
    /// \brief Get a component from the current entity
    ///
    ///////////////////////////////////////////////////////////
    template <ComponentType C> C& get() const;

    ///////////////////////////////////////////////////////////
    /// \brief Get an accessor for another entity
    ///
    ///////////////////////////////////////////////////////////
    QueryAccessor get(EntityId id) const;

private:
    World* m_world;       //!< World pointer used to create new accessors
    EntityGroup* m_group; //!< Group being iterated
    uint32_t m_entityIdx; //!< Index of entity within group
};

///////////////////////////////////////////////////////////
/// \brief Iterator that can be used to access extra information during queries
///
///////////////////////////////////////////////////////////
struct QueryIterator : public QueryAccessor {
    ///////////////////////////////////////////////////////////
    /// \brief Constructor (not meant to be called publicly)
    ///
    /// \param id The id of the entity to access
    /// \param index The index of the iteration
    /// \param world The world pointer used to create new accessors
    /// \param group The group being iterated
    /// \param entityIdx The index of the entity within the group
    ///
    ///////////////////////////////////////////////////////////
    QueryIterator(
        EntityId id,
        uint32_t index,
        World* world,
        EntityGroup* group,
        uint32_t entityIdx
    );

    EntityId id;    //!< Entity id of current iteration
    uint32_t index; //!< Index of current iteration
};

///////////////////////////////////////////////////////////
/// \brief Query base class
///
///////////////////////////////////////////////////////////
class QueryDescriptor {
    friend World;

public:
    ///////////////////////////////////////////////////////////
    /// \brief Get the hash of the query
    ///
    /// This hash depends on which components are included and excluded from the query.
    /// Hash value is not cached.
    ///
    /// \return The hash of the query
    ///
    ///////////////////////////////////////////////////////////
    uint32_t getHash() const;

protected:
    std::vector<std::type_index> m_include; //!< A set of comopnents to include
    std::vector<std::type_index> m_exclude; //!< A set of comopnents to exclude
    std::vector<std::mutex*>
        m_mutexes; //!< Mutexes to lock when starting the query (locked before any callbacks)
};

///////////////////////////////////////////////////////////
/// \brief A query builder
///
///////////////////////////////////////////////////////////
class QueryFactory : public QueryDescriptor {
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
    template <ComponentType C> QueryFactory& match();

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
    template <ComponentType C> QueryFactory& exclude();

    ///////////////////////////////////////////////////////////
    /// \brief Compile the query
    ///
    ///////////////////////////////////////////////////////////
    Query compile();

private:
    World* m_world;                      //!< World access to iterate components
    std::vector<EntityGroupId> m_groups; //!< List of entity groups that match query
};

} // namespace ply

#include <ply/ecs/QueryFactory.inl>
