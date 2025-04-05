#pragma once

#include <ply/ecs/QueryFactory.h>

namespace ply {

class World;
class EntityGroup;

class Observer : public QueryDescriptor {
    friend World;

public:
    using IteratorFn = std::function<void(
        const std::vector<EntityId>&,
        const HashMap<std::type_index, void*>&,
        World*,
        EntityGroup*
    )>;

public:
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
    template <ComponentType C> Observer& match();

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
    template <ComponentType C> Observer& exclude();

    ///////////////////////////////////////////////////////////
    /// \brief Set the function that will get called on all entities that match the observer query
    ///
    ///////////////////////////////////////////////////////////
    template <typename Func> void each(Func&& fn);

private:
    IteratorFn m_iterator; //!< The function that will get called
};

} // namespace ply

#include <ply/ecs/Observer.inl>
