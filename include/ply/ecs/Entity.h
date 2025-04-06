#pragma once

#include <ply/core/Handle.h>
#include <ply/core/Types.h>
#include <ply/ecs/Types.h>
#include <type_traits>

namespace ply {

class World;
class EntityGroup;

///////////////////////////////////////////////////////////
/// \brief Entity accessor type for short-term component access
///
/// This class provides temporary access to an entity's components
/// and manages mutex locking tied to its lifetime. It should be
/// used for short-term operations only, as it holds a read lock
/// on the entity's group until either release() is called or the
/// object is destroyed.
///
/// IMPORTANT: Failing to release or allow this object to be
/// destroyed can block other threads from accessing the entity
/// group, potentially deadlocking the program.
///
/// \see World::getEntity()
///
///////////////////////////////////////////////////////////
class Entity {
public:
    ///////////////////////////////////////////////////////////
    /// \brief Constructor
    ///
    /// Default constructor creates an invalid entity accessor
    /// that doesn't hold any locks.
    ///////////////////////////////////////////////////////////
    Entity();

    ///////////////////////////////////////////////////////////
    /// \brief Constructor
    ///
    /// Creates an entity accessor and acquires a read lock on
    /// the entity's group. The lock will be held until release()
    /// is called or the object is destroyed.
    ///
    /// \param world Pointer to the world containing the entity
    /// \param id Entity identifier
    /// \param group Pointer to the entity's group
    /// \param index Index of the entity within its group
    ///////////////////////////////////////////////////////////
    Entity(World* world, EntityId id, EntityGroup* group, uint32_t index);
    
    ///////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    /// Unlocks read mutex
    ///
    ///////////////////////////////////////////////////////////
    ~Entity();
    
    ///////////////////////////////////////////////////////////
    /// \brief Release entity lock
    ///
    /// Manually releases the read lock on the entity's group.
    /// After calling this method, the entity accessor becomes
    /// invalid and should not be used to access components.
    ///
    /// This method is automatically called by the destructor,
    /// but can be called earlier if access is no longer needed.
    ///////////////////////////////////////////////////////////
    void release();
    
    ///////////////////////////////////////////////////////////
    /// \brief Check if the current entity has a component
    ///
    /// \tparam C The component type to check for
    /// \return True if the entity has the component, false otherwise
    ///////////////////////////////////////////////////////////
    template <ComponentType C> bool has() const;
    
    ///////////////////////////////////////////////////////////
    /// \brief Get a component from the entity
    ///
    /// Retrieves a reference to a component attached to this entity.
    /// The entity must have the component, or undefined behavior
    /// will occur. Use has<C>() to check first if unsure.
    ///
    /// \tparam C The component type to retrieve
    /// \return A reference to requested component
    ///
    /// \warning Accessing the returned reference after the Entity
    /// accessor is released or destroyed is undefined behavior.
    ///////////////////////////////////////////////////////////
    template <ComponentType C> C& get() const;

    ///////////////////////////////////////////////////////////
    /// \brief Get the set of components attached to the entity
    ///
    /// This is an untemplated version of the function, so all component
    /// data is returned as void pointers.
    ///
    /// \return A map of type index to pointer to the entity's components
    ///
    ///////////////////////////////////////////////////////////
    HashMap<std::type_index, void*> getAll();

    ///////////////////////////////////////////////////////////
    /// \brief Add a component to the entity
    ///
    /// \tparam C The component type to add
    /// \param component The component to add
    ///
    ///////////////////////////////////////////////////////////
    template <ComponentType C> void add(const C& component);

    ///////////////////////////////////////////////////////////
    /// \brief Remove a component from the entity
    ///
    /// \tparam C The component type to remove
    ///
    ///////////////////////////////////////////////////////////
    template <ComponentType C> void remove();

    ///////////////////////////////////////////////////////////
    /// \brief Get the id of the entity
    ///
    ///////////////////////////////////////////////////////////
    EntityId getId() const;

private:
    World* m_world;       //!< Pointer to world containing the entity
    EntityGroup* m_group; //!< Group the entity belongs to (holds the mutex)
    uint32_t m_index;     //!< Index of entity in its group
    EntityId m_id;        //!< Id of the entity
};

}

#ifndef PLY_ECS_WORLD_H
#include <ply/ecs/Entity.inl>
#endif
