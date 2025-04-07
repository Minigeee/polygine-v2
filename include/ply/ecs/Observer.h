#pragma once

#include <ply/ecs/QueryBase.h>
#include <ply/ecs/Types.h>

namespace ply {

class World;
class EntityGroup;

class Observer : public QueryBase {
    friend World;

public:
    using IteratorFn = std::function<
        void(const std::vector<EntityId>&, const HashMap<std::type_index, void*>&, World*, EntityGroup*, float)>;

public:
    Observer() = default;
    Observer(World* world);
    Observer(const Observer& other) = delete;
    Observer(Observer&& other) = delete;
    Observer& operator=(const Observer& other) = delete;
    Observer& operator=(Observer&& other) = delete;

    ///////////////////////////////////////////////////////////
    /// \brief Add a mutex to lock before iterating comopnents
    ///
    /// The mutex will be locked before any callback functions are called.
    ///
    /// \param mutex The mutex to lock
    ///
    ///////////////////////////////////////////////////////////
    Observer& lock(std::mutex& mutex);

    ///////////////////////////////////////////////////////////
    /// \brief Set the which component types should be included in the component query
    ///
    /// \param include The type set to include
    ///
    ///////////////////////////////////////////////////////////
    Observer& match(const TypeSet& include);

    ///////////////////////////////////////////////////////////
    /// \brief Add a type to the include type set
    ///
    ///////////////////////////////////////////////////////////
    template <ComponentType... Cs> Observer& match();

    ///////////////////////////////////////////////////////////
    /// \brief Set the which component types should be excluded from the component query
    ///
    /// \param exclude The type set to exclude
    ///
    ///////////////////////////////////////////////////////////
    Observer& exclude(const TypeSet& exclude);

    ///////////////////////////////////////////////////////////
    /// \brief Add a type to the exclude type set
    ///
    ///////////////////////////////////////////////////////////
    template <ComponentType... Cs> Observer& exclude();

    ///////////////////////////////////////////////////////////
    /// \brief Set the function that will get called on all entities that match the Observer query
    ///
    ///////////////////////////////////////////////////////////
    template <typename Func> Observer* each(Func&& fn);

private:
    World* m_world;                       //!< World the Observer is attached to
    IteratorFn m_iterator;                //!< The function that will get called
    HashSet<EntityGroupId> m_watchGroups; //!< The set of groups the Observer should watch
};

} // namespace ply

#ifndef PLY_ECS_WORLD_H
#include <ply/ecs/Observer.inl>
#endif
