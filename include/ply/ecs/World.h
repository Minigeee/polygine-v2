#pragma once

#ifndef PLY_ECS_WORLD_H
#define PLY_ECS_WORLD_H
#endif

#include <ply/core/Clock.h>
#include <ply/core/HandleArray.h>
#include <ply/core/Mutex.h>
#include <ply/ecs/Entity.h>
#include <ply/ecs/EntityFactory.h>
#include <ply/ecs/EntityGroup.h>
#include <ply/ecs/Observer.h>
#include <ply/ecs/QueryFactory.h>

#include <memory>
#include <typeindex>

namespace ply {

class Query;

///////////////////////////////////////////////////////////
/// \brief ECS World
///
/// The World class is the main container for the Entity-Component-System (ECS)
/// architecture. It manages entities, components, and provides the infrastructure
/// for systems to operate on them.
///
/// World is a more focused and concise implementation of an ECS container,
/// designed for performance and ease of use. It organizes entities into groups
/// based on their component composition for efficient querying and iteration.
///
/// Key responsibilities:
/// - Creating and removing entities
/// - Managing entity groups (collections of entities with the same components)
/// - Providing query mechanisms to find and process entities with specific components
/// - Handling entity events (creation, removal)
///
/// Usage example:
/// \code
/// ply::World world;
///
/// // Create entities with components
/// auto ids = world.entity()
///     .add(Position{0, 0})
///     .add(Velocity{1, 0})
///     .create(10); // Create 10 entities with these components
///
/// // Query and process entities
/// world.query()
///     .match<Position, Velocity>()
///     .compile()
///     .each([](QueryIterator it, Position& pos, Velocity& vel) {
///         pos.x += vel.x;
///         pos.y += vel.y;
///     });
///
/// // Remove entities
/// world.remove(ids[0]);
/// world.removeQueuedEntities(); // Actually removes the queued entities
/// \endcode
///
///////////////////////////////////////////////////////////
class World {
    friend class Entity;
    friend class EntityFactory;
    friend class QueryAccessor;
    friend class QueryFactory;
    friend class Query;
    friend class Observer;

public:
    ///////////////////////////////////////////////////////////
    /// \brief Type of entity events
    ///
    ///////////////////////////////////////////////////////////
    enum EntityEventType {
        OnCreate = 0, //!< Called when entities are created
        OnRemove,     //!< Called when entities are removed
        OnEnter,      //!< Called when entities enter a query (when they start matching a query)
        OnExit,       //!< Called when entities exit a query (when they stop matching a query)
        NUM_EVENTS
    };

public:
    ///////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Creates an empty world with no entities.
    ///
    ///////////////////////////////////////////////////////////
    World();

    ///////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    /// Cleans up all entities, components, and related resources.
    ///
    ///////////////////////////////////////////////////////////
    ~World();

    ///////////////////////////////////////////////////////////
    /// \brief Create an entity factory for building new entities
    ///
    /// This function returns an entity factory that can be used to
    /// create new entities with components. The factory allows for
    /// a fluent interface to add components and then create one or
    /// more entities with the same component structure.
    ///
    /// Usage example:
    /// \code
    /// auto ids = world.entity()
    ///     .add(Position{0, 0})
    ///     .add(Velocity{1, 0})
    ///     .create(10); // Create 10 entities
    /// \endcode
    ///
    /// \return An entity factory for building and creating entities
    ///
    ///////////////////////////////////////////////////////////
    EntityFactory entity();

    ///////////////////////////////////////////////////////////
    /// \brief Queue an entity for removal
    ///
    /// Removing entities while component data is being processed
    /// can lead to unpredictable behavior, so entities
    /// must be queued for removal instead.
    ///
    /// The queued entities can then all be removed at once
    /// at a later time, most often at the end of the update frame.
    /// tick() must be called to remove queued entities.
    ///
    /// \param id The id of the entity to remove
    ///
    ///////////////////////////////////////////////////////////
    void remove(EntityId id);

    ///////////////////////////////////////////////////////////
    /// \brief Get an entity accessor for the specified entity
    ///
    /// Returns an Entity accessor object that provides temporary
    /// access to the entity's components. The accessor manages
    /// mutex locking tied to its lifetime, so it should be used
    /// for short-term operations only.
    ///
    /// \param id The id of the entity to access
    ///
    /// \return An Entity accessor for the specified entity
    ///
    /// \see Entity
    ///
    ///////////////////////////////////////////////////////////
    Entity getEntity(EntityId id);

    template <ComponentType C> void addComponent(EntityId id, const C& component);

    template <ComponentType C> void removeComponent(EntityId id);

    ///////////////////////////////////////////////////////////
    /// \brief Get an observer for entity events
    ///
    /// Returns an Observer that can be configured to respond to
    /// entity events of the specified type. Observers can filter
    /// which entities they respond to based on component types.
    ///
    /// Usage example:
    /// \code
    /// world.observer(World::OnCreate)
    ///     .match<Position, Health>()
    ///     .each([](QueryIterator it, Position& pos, Health& health) {
    ///         // Initialize new entities
    ///         health.current = health.max;
    ///     });
    /// \endcode
    ///
    /// \param type The type of entity event to observe
    ///
    /// \return An Observer for the specified event type
    ///
    ///////////////////////////////////////////////////////////
    Observer& observer(EntityEventType type);

    ///////////////////////////////////////////////////////////
    /// \brief Create a query factory for building entity queries
    ///
    /// Returns a QueryFactory that can be used to build and execute
    /// queries against entities in the world. Queries can filter
    /// entities based on which components they have or don't have.
    ///
    /// Usage example:
    /// \code
    /// auto query = world.query()
    ///     .match<Position, Velocity>()
    ///     .exclude<Static>()
    ///     .compile();
    ///
    /// query.each([](QueryIterator it, Position& pos, Velocity& vel) {
    ///     pos.x += vel.x;
    ///     pos.y += vel.y;
    /// });
    /// \endcode
    ///
    /// \return A QueryFactory for building entity queries
    ///
    ///////////////////////////////////////////////////////////
    QueryFactory query();

    void tick();

private:
    ///////////////////////////////////////////////////////////
    /// \brief Data for entities
    ///
    /// Internal structure that stores the location information
    /// for each entity, mapping entity IDs to their group and
    /// index within that group.
    ///////////////////////////////////////////////////////////
    struct EntityData {
        EntityGroupId m_group; //!< Group the entity belongs to
        uint32_t m_index;      //!< Index of entity's components within group
        bool m_isAlive;        //!< If entity is alive
    };

    ///////////////////////////////////////////////////////////
    /// \brief Data for deferred component change operations
    ///////////////////////////////////////////////////////////
    struct ComponentChange {
        ComponentChange(EntityId id, std::type_index type);

        EntityId m_id;          //!< Entity that is changing
        std::type_index m_type; //!< Type of component
        void* m_component;      //!< Component to add or NULL for remove
        size_t m_size;          //!< Type size
        size_t m_align;         //!< Type alignment
    };

    ///////////////////////////////////////////////////////////
    /// \brief Send entity event
    ///
    /// Internal method that notifies observers about entity events.
    /// This is called automatically when entities are created or
    /// removed.
    ///
    /// \param type The type of event (creation or removal)
    /// \param ids List of entity IDs affected by the event
    /// \param ptrs Map of component type to component data pointers
    /// \param group Pointer to the entity group containing the entities
    ///
    ///////////////////////////////////////////////////////////
    void sendEntityEvent(
        EntityEventType type,
        const std::vector<EntityId>& ids,
        const HashMap<std::type_index, void*>& ptrs,
        EntityGroup* group
    );

    ///////////////////////////////////////////////////////////
    /// \brief Remove all entities that are queued for removal
    ///
    /// Removes all queued entities. This function should only be
    /// called once per frame. Most often, it should be called at
    /// the end of an update frame, after all component data has
    /// finished processing.
    ///
    /// \see remove
    ///
    ///////////////////////////////////////////////////////////
    void removeQueuedEntities();

    ///////////////////////////////////////////////////////////
    /// \brief Add component to entity implementation, does not handle thread safety for original
    /// entity group
    ///////////////////////////////////////////////////////////
    void addComponent(
        EntityGroup* group,
        EntityId id,
        std::type_index type,
        void* component,
        size_t size,
        size_t align
    );

    ///////////////////////////////////////////////////////////
    /// \brief Remove component from entity implementation, does not handle thread safety for
    /// original entity group
    ///////////////////////////////////////////////////////////
    void removeComponent(EntityGroup* group, EntityId id, std::type_index type);

    ///////////////////////////////////////////////////////////
    /// \brief Handles sending enter and exit queries
    ///////////////////////////////////////////////////////////
    void dispatchEntityChangeEvents(
        EntityId id,
        const HashMap<std::type_index, void*>& ptrs,
        EntityGroup* oldGroup,
        EntityGroup* newGroup
    );

    ///////////////////////////////////////////////////////////
    /// \brief Apply queued entity creation
    ///////////////////////////////////////////////////////////
    void addQueuedEntities();

    ///////////////////////////////////////////////////////////
    /// \brief Apply queued component changes
    ///////////////////////////////////////////////////////////
    void changeQueuedEntities();

    ///////////////////////////////////////////////////////////
    /// \brief Check if query matches for an entity group
    ///
    /// Internal method that determines if a query matches the
    /// component structure of an entity group. Used to optimize
    /// query execution by only checking groups that could
    /// potentially contain matching entities.
    ///
    /// \param group The entity group to check against
    /// \param query The query descriptor to match
    ///
    /// \return True if the query could match entities in the group
    ///
    ///////////////////////////////////////////////////////////
    bool matchesForGroup(EntityGroup& group, QueryDescriptor* query);

    ///////////////////////////////////////////////////////////
    /// \brief Get or create entity group
    ///
    /// \param id The id of the group
    ///\param components The components that will be stored in the group
    ///
    /// \return A pointer to the entity group
    ///
    ///////////////////////////////////////////////////////////
    EntityGroup* getOrCreateEntityGroup(
        EntityGroupId id,
        const HashMap<std::type_index, priv::ComponentMetadata>& components
    );

    ///////////////////////////////////////////////////////////
    /// \brief Register observer so that dispatching events is faster
    ///////////////////////////////////////////////////////////
    void registerObserver(Observer* observer);

    ///////////////////////////////////////////////////////////
    /// \brief Register query so that it can properly find components and entities
    ///
    /// Internal method that registers a query with the world and
    /// identifies which entity groups match the query. This
    /// optimization allows queries to only iterate over groups
    /// that could potentially contain matching entities.
    ///
    /// \param query The query factory to register
    ///
    /// \return The registered query factory
    ///
    ///////////////////////////////////////////////////////////
    QueryFactory* registerQuery(QueryFactory* query);

private:
    SharedMutex m_groupsMutex;          //!< Mutex protecting access to entity groups
    HandleArray<EntityData> m_entities; //!< Array of entity data mapping IDs to groups and indices
    HashMap<EntityGroupId, std::unique_ptr<EntityGroup>>
        m_groups; //!< Map of group IDs to entity groups

    // Deferred operations
    std::vector<EntityFactory*> m_addQueue;     //!< List of entities to add
    std::vector<ComponentChange> m_changeQueue; //!< List of component changes to apply
    uint32_t m_numRemoveQueued; //!< Tracks number of entities queued for removal for optimization

    // Observers
    TypePool<Observer>
        m_observerPool; //!< Pool allocator for observers to reduce memory fragmentation
    std::vector<Observer*>
        m_observers[EntityEventType::NUM_EVENTS]; //!< Lists of observers for each event type

    // Queries
    TypePool<QueryFactory> m_queryPool;         //!< Pool allocator for query factories
    HashMap<uint32_t, QueryFactory*> m_queries; //!< Map of query hash to query factory for reuse
};

} // namespace ply

#include <ply/ecs/Entity.inl>
#include <ply/ecs/Observer.inl>
#include <ply/ecs/QueryFactory.inl>
#include <ply/ecs/World.inl>

// Include so that .cpp files have access to the function w/o having to use extra includes
#include <ply/ecs/Query.h>
