#pragma once

#include <ply/core/Mutex.h>
#include <ply/core/PoolAllocator.h>
#include <ply/core/Types.h>
#include <ply/ecs/Entity.h>

#include <typeindex>

namespace ply {

class World;
class EntityGroup;

///////////////////////////////////////////////////////////
/// \brief Utility class that is used to create entities
///
///////////////////////////////////////////////////////////
class EntityFactory {
public:
    ///////////////////////////////////////////////////////////
    /// \brief Constructor
    ///
    ///////////////////////////////////////////////////////////
    EntityFactory();

    ///////////////////////////////////////////////////////////
    /// \brief Constructor
    ///
    ///////////////////////////////////////////////////////////
    EntityFactory(World* world);

    ///////////////////////////////////////////////////////////
    /// \brief Frees all temp allocated components if still there
    ///
    ///////////////////////////////////////////////////////////
    ~EntityFactory();

#ifndef DOXYGEN_SKIP
    EntityFactory(const EntityFactory&);
    EntityFactory(EntityFactory&&) noexcept;
    EntityFactory& operator=(const EntityFactory&);
    EntityFactory& operator=(EntityFactory&&) noexcept;
#endif

    ///////////////////////////////////////////////////////////
    /// \brief Add component to entity creation
    ///
    ///	\param component The component to add
    ///
    /// \return The builder to chain commands
    ///
    ///////////////////////////////////////////////////////////
    template <ComponentType C> EntityFactory& add(const C& component);

    ///////////////////////////////////////////////////////////
    /// \brief Add tag to entity creation
    ///
    ///	\param tag The tag to add
    ///
    /// \return The builder to chain commands
    ///
    ///////////////////////////////////////////////////////////
    template <ComponentType T> EntityFactory& tag(const T& tag);

    ///////////////////////////////////////////////////////////
    /// \brief Create entities using all the comonents that have been added to the
    /// builder
    ///
    /// The builder is reset after this function is finished, so
    /// any following components must be readded.
    ///
    /// \param num The number of copies of the entity to create
    ///
    /// \return A list of entity ids that were created
    ///
    ///////////////////////////////////////////////////////////
    std::vector<EntityId> create(uint32_t num = 1);

    ///////////////////////////////////////////////////////////
    /// \brief Create entities using all the comonents that have been added to the
    /// builder
    ///
    /// The builder is reset after this function is finished, so
    /// any following components must be readded.
    ///
    /// This version provides a function callback that gets invoked
    /// for every entity that gets created. This is useful for setting
    /// individual values in a batch entity create operation.
    ///
    /// If the first parameter of the function is an integer type,
    /// then the index of the entity within the batch will be provided.
    ///
    /// \param onCreate The function that gets invoked for each created entity
    /// \param num The number of copies of the entity to create
    ///
    /// \return A list of entity ids that were created
    ///
    /// \note There is not compile time checking to see if the right components
    /// are being used
    ///
    ///////////////////////////////////////////////////////////
    template <ComponentType... Cs, typename Func>
    std::vector<EntityId> create(Func&& onCreate, uint32_t num = 1);

private:
    ///////////////////////////////////////////////////////////
    /// \brief Contains data required to create components
    ///////////////////////////////////////////////////////////
    struct ComponentMetadata {
        void* m_data;     //!< Pointer to component data
        uint32_t m_size;  //!< Size of component type
        uint32_t m_align; //!< Align of component type
    };

    ///////////////////////////////////////////////////////////
    /// \brief Common create code
    /// \return A map of component type to pointers to start locations of new
    /// components
    ///////////////////////////////////////////////////////////
    std::vector<EntityId> createImpl(uint32_t num, HashMap<std::type_index, void*>& ptrs);

    ///////////////////////////////////////////////////////////
    /// \brief Send entity event
    ///////////////////////////////////////////////////////////
    void sendEvent(const std::vector<EntityId>& ids, const HashMap<std::type_index, void*>& ptrs);

private:
    World* m_world;       //!< A pointer to the scene the builder belongs to
    EntityGroup* m_group; //!< The group the entities will be added to
    HashMap<std::type_index, ComponentMetadata>
        m_components; //!< Map of component types to their instantiated data

    static HashMap<std::type_index, ObjectPool>
        s_pools; //!< Map of component types to their temp allocator
};

} // namespace ply

#include <ply/ecs/EntityFactory.inl>