#pragma once

#include <memory>
#include <ply/core/Clock.h>
#include <ply/core/HandleArray.h>
#include <ply/core/Mutex.h>
#include <ply/ecs/Entity.h>
#include <ply/ecs/EntityFactory.h>
#include <ply/ecs/EntityGroup.h>
#include <ply/ecs/Observer.h>
#include <ply/ecs/QueryFactory.h>

namespace ply {

class Query;

///////////////////////////////////////////////////////////
/// \brief ECS World
///
///////////////////////////////////////////////////////////
class World {
    friend class EntityFactory;
    friend class QueryAccessor;
    friend class QueryFactory;
    friend class Query;

public:
    ///////////////////////////////////////////////////////////
    /// \brief Type of entity events
    ///
    ///////////////////////////////////////////////////////////
    enum EntityEventType {
        OnCreate = 0, //!< Called when entities are created
        OnRemove,     //!< Called when entities are removed
        NUM_EVENTS
    };

public:
    World();
    ~World();

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

    Observer& observer(EntityEventType type);

    QueryFactory query();

    void tick();

private:
    ///////////////////////////////////////////////////////////
    /// \brief Data for entities
    ///////////////////////////////////////////////////////////
    struct EntityData {
        EntityGroupId m_group; //!< Group the entity belongs to
        uint32_t m_index;      //!< Index of entity's components within group
    };

    ///////////////////////////////////////////////////////////
    /// \brief Send entity event
    ///////////////////////////////////////////////////////////
    void sendEntityEvent(
        EntityEventType type,
        const std::vector<EntityId>& ids,
        const HashMap<std::type_index, void*>& ptrs,
        EntityGroup* group
    );

    ///////////////////////////////////////////////////////////
    /// \brief Check if query matches for an entity group
    ///////////////////////////////////////////////////////////
    bool matchesForGroup(EntityGroup& group, QueryDescriptor* query);

    ///////////////////////////////////////////////////////////
    /// \brief Get or create entity group
    ///
    /// \param id The id of the group
    ///
    /// \return A reference to the entity group
    ///
    ///////////////////////////////////////////////////////////
    EntityGroup& getOrCreateEntityGroup(EntityGroupId id);

    ///////////////////////////////////////////////////////////
    /// \brief Register query so that it can properly find components and entities
    ///////////////////////////////////////////////////////////
    QueryFactory* registerQuery(QueryFactory* query);

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

private:
    SharedMutex m_groupsMutex;          //!< Mutex protecting access to entity groups
    HandleArray<EntityData> m_entities; //!< Array of entity data
    HashMap<EntityGroupId, std::unique_ptr<EntityGroup>> m_groups; //!< Map of ids to entity groups
    uint32_t m_numRemoveQueued; //!< Tracks number of entities queued for removal, so each table
                                //!< doesn't have to be checked

    // Observers
    TypePool<Observer> m_observerPool; //!< Pool allocator for component queries (for observers)
    std::vector<Observer*>
        m_observers[EntityEventType::NUM_EVENTS]; //!< List of entity event listener functions for
                                                  //!< each type

    TypePool<QueryFactory> m_queryPool;         //!< Pool allocator for component queries
    HashMap<uint32_t, QueryFactory*> m_queries; //!< Map of query fingerprint id to query objects
};

} // namespace ply

// Include so that .cpp files have access to the function w/o having to use extra includes
#include <ply/ecs/Query.h>
