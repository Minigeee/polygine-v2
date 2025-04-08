#include <ply/core/Macros.h>
#include <ply/core/PoolAllocator.h>
#include <ply/core/Tuple.h>
#include <ply/core/Types.h>
#include <ply/ecs/EntityBuilder.h>

#include <loguru.hpp>

namespace ply {

///////////////////////////////////////////////////////////
template <ComponentType C> EntityBuilder& EntityBuilder::add(const C& component) {
    CHECK_F(VALID_COMPONENT_TYPE(C), "component type %s is not valid", typeid(C).name());

    std::type_index tid = typeid(C);
    auto it = m_components.find(tid);

    // Only add if not added
    if (it == m_components.end()) {
        // Create pool allocator if needed
        auto poolIt = s_pools.find(tid);
        if (poolIt == s_pools.end())
            poolIt = s_pools.emplace(std::make_pair(tid, ObjectPool(sizeof(C), 64))).first;

        // Allocate temp component
        C* ptr = (C*)poolIt.value().alloc();
        *ptr = component;

        // Save data
        m_components[tid] = priv::ComponentMetadata({ptr, sizeof(C), alignof(C)});
    }

    return *this;
}

///////////////////////////////////////////////////////////
template <typename Func>
std::vector<EntityId> EntityBuilder::create(Func&& onCreate, uint32_t num) {
    if (!m_components.size())
        return {};

    // Get first parameter type
    using FirstParamType = typename first_param<std::decay_t<decltype(onCreate)>>::type;

    // Check if first parameter is a meta type (QueryIterator, EntityId, or integral)
    constexpr bool HasMetaFirst = std::is_integral_v<FirstParamType>;

    // Get component types from function parameters
    // If first parameter is meta, use rest_param_types, otherwise use param_types
    using CTypes = typename std::conditional_t<
        HasMetaFirst,
        typename rest_param_types<std::decay_t<decltype(onCreate)>>::type,
        typename param_types<std::decay_t<decltype(onCreate)>>::type>;

    return templateCreateImpl(
        std::forward<Func>(onCreate), num, type_wrapper<decayed_tuple_t<CTypes>>{}
    );
}

///////////////////////////////////////////////////////////
template <typename... Cs, typename Func>
std::vector<EntityId>
EntityBuilder::templateCreateImpl(Func&& onCreate, uint32_t num, type_wrapper<std::tuple<Cs...>>) {
    // Get first parameter type
    using FirstParamType = typename first_param<std::decay_t<decltype(onCreate)>>::type;

    // Check that the types the function needs are added to the entity
    {
        std::vector<std::type_index> reqTypeIds;
        PARAM_EXPAND(reqTypeIds.push_back(typeid(Cs)));

        for (size_t i = 0; i < reqTypeIds.size(); ++i)
            CHECK_F(
                m_components.find(reqTypeIds[i]) != m_components.end(),
                "attempted to access a component that has not been added to "
                "entity: %s",
                reqTypeIds[i].name()
            );
    }

    // Create
    HashMap<std::type_index, void*> ptrs;
    // Create (can't allow defered bc no way to store function)
    std::vector<EntityId> ids = createImpl(num, ptrs, false);

    // Create tuple bc it should be a little faster to access
    Tuple<Cs*...> tuple((Cs*)ptrs.find(typeid(Cs)).value()...);

    // Call function for each instance
    for (uint32_t i = 0; i < num; ++i) {
        if constexpr (std::is_integral_v<FirstParamType>)
            onCreate(i, tuple.template get<Cs*>()[i]...);
        else
            onCreate(tuple.template get<Cs*>()[i]...);
    }

    // Send add event
    sendEvent(ids, ptrs);

    return ids;
}

} // namespace ply