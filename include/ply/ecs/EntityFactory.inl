#include <ply/core/Macros.h>
#include <ply/core/PoolAllocator.h>
#include <ply/core/Tuple.h>
#include <ply/core/Types.h>
#include <ply/ecs/Entity.h>
#include <ply/ecs/EntityFactory.h>

#include <loguru.hpp>

namespace ply {

///////////////////////////////////////////////////////////
template <ComponentType C>
EntityFactory& EntityFactory::add(const C& component) {
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
        m_components[tid] = ComponentMetadata({ptr, sizeof(C), alignof(C)});
    }

    return *this;
}

///////////////////////////////////////////////////////////
template <ComponentType... Cs, typename Func>
std::vector<EntityId> EntityFactory::create(Func&& onCreate, uint32_t num) {
    if (!m_components.size())
        return {};

    // Get first parameter type
    using FirstParamType =
        typename first_param<std::decay_t<decltype(onCreate)>>::type;

    // Check that the types the function needs are added to the entity
    {
        std::vector<std::type_index> reqTypeIds;
        PARAM_EXPAND(reqTypeIds.push_back(typeid(Cs)));

        for (size_t i = 0; i < reqTypeIds.size(); ++i)
            CHECK_F(m_components.find(reqTypeIds[i]) != m_components.end(),
                    "attempted to access a component that has not been added to "
                    "entity: %s",
                    reqTypeIds[i].name());
    }

    // Create
    HashMap<std::type_index, void*> ptrs;
    std::vector<EntityId> ids = createImpl(num, ptrs);

    // Call function for each instance
    for (uint32_t i = 0; i < num; ++i) {
        if constexpr (std::is_integral_v<FirstParamType>)
            onCreate(i, reinterpret_cast<Cs*>(ptrs[typeid(Cs)])[i]...);
        else
            onCreate(reinterpret_cast<Cs*>(ptrs[typeid(Cs)])[i]...);
    }

    // Send add event
    sendEvent(ids, ptrs);

    return ids;
}

} // namespace ply