#pragma once

#include <ply/ecs/QueryBase.h>
#include <ply/ecs/Types.h>

namespace ply {

class World;
class EntityGroup;

class System : public QueryBase {
    friend World;

public:
    using IteratorFn = std::function<void(
        const std::vector<EntityId>&,
        const HashMap<std::type_index, void*>&,
        World*,
        EntityGroup*,
        float
    )>;

public:
    System() = default;
    System(World* world);
    System(const System& other) = delete;
    System(System&& other) = delete;
    System& operator=(const System& other) = delete;
    System& operator=(System&& other) = delete;

    ///////////////////////////////////////////////////////////
    /// \brief Add a mutex to lock before iterating comopnents
    ///
    /// The mutex will be locked before any callback functions are called.
    ///
    /// \param mutex The mutex to lock
    ///
    ///////////////////////////////////////////////////////////
    System& lock(std::mutex& mutex);

    ///////////////////////////////////////////////////////////
    /// \brief Set the which component types should be included in the component query
    ///
    /// \param include The type set to include
    ///
    ///////////////////////////////////////////////////////////
    System& match(const TypeSet& include);

    ///////////////////////////////////////////////////////////
    /// \brief Add a type to the include type set
    ///
    ///////////////////////////////////////////////////////////
    template <ComponentType... Cs> System& match();

    ///////////////////////////////////////////////////////////
    /// \brief Set the which component types should be excluded from the component query
    ///
    /// \param exclude The type set to exclude
    ///
    ///////////////////////////////////////////////////////////
    System& exclude(const TypeSet& exclude);

    ///////////////////////////////////////////////////////////
    /// \brief Add a type to the exclude type set
    ///
    ///////////////////////////////////////////////////////////
    template <ComponentType... Cs> System& exclude();

    ///////////////////////////////////////////////////////////
    /// \brief Specify that this system should run before another
    ///
    /// \param system The system to run before
    ///
    ///////////////////////////////////////////////////////////
    System& before(System* system);

    ///////////////////////////////////////////////////////////
    /// \brief Specify that this system should run after another
    ///
    /// \param system The system to run after
    ///
    ///////////////////////////////////////////////////////////
    System& after(System* system);

    ///////////////////////////////////////////////////////////
    /// \brief Set the function that will get called on all entities that match the System query
    ///
    ///////////////////////////////////////////////////////////
    template <typename Func> System* each(Func&& fn);

private:
    World* m_world;                      //!< World the System is attached to
    IteratorFn m_iterator;               //!< The function that will get called
    std::vector<System*> m_dependencies; //!< The dependencies of the system
    std::vector<EntityGroupId> m_groups; //!< List of entity groups that match query
};

} // namespace ply

#ifndef PLY_ECS_WORLD_H
#include <ply/ecs/System.inl>
#endif
