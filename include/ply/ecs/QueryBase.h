#pragma once

#include <functional>
#include <ply/core/Mutex.h>
#include <ply/core/TypeSet.h>
#include <ply/core/Types.h>
#include <ply/ecs/EntityGroup.h>
#include <ply/ecs/Types.h>
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
    QueryAccessor(World* world, EntityGroup* group, EntityId id, uint32_t entityIdx);

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
    /// \brief Add a component to the current entity
    ///
    ///////////////////////////////////////////////////////////
    template <ComponentType C> void add(const C& component);

    ///////////////////////////////////////////////////////////
    /// \brief Remove a component from the current entity
    ///
    ///////////////////////////////////////////////////////////
    template <ComponentType C> void remove();

    ///////////////////////////////////////////////////////////
    /// \brief Get an accessor for another entity
    ///
    ///////////////////////////////////////////////////////////
    QueryAccessor getEntity(EntityId id) const;

    EntityId id; //!< Entity id of current iteration

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
    /// \param dt The time since last frame
    ///
    ///////////////////////////////////////////////////////////
    QueryIterator(
        EntityId id,
        uint32_t index,
        World* world,
        EntityGroup* group,
        uint32_t entityIdx,
        float dt
    );

    uint32_t index; //!< Index of current iteration
    float dt; //!< Time since last frame
};

///////////////////////////////////////////////////////////
/// \brief Query base class
///
///////////////////////////////////////////////////////////
class QueryBase {
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
    ///////////////////////////////////////////////////////////
    /// \brief Add a type to the include type set
    ///////////////////////////////////////////////////////////
    void addInclude(const std::type_index& type);

    ///////////////////////////////////////////////////////////
    /// \brief Add a type to the exclude type set
    ///////////////////////////////////////////////////////////
    void addExclude(const std::type_index& type);

protected:
    std::vector<std::type_index> m_include; //!< A set of comopnents to include
    std::vector<std::type_index> m_exclude; //!< A set of comopnents to exclude
    std::vector<std::mutex*>
        m_mutexes; //!< Mutexes to lock when starting the query (locked before any callbacks)
};

} // namespace ply
