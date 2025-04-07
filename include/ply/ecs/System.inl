#pragma once

#include <ply/core/Macros.h>
#include <ply/core/Tuple.h>
#include <ply/core/Types.h>
#include <ply/ecs/World.h>

namespace ply {

///////////////////////////////////////////////////////////
template <ComponentType... Cs> System& System::match() {
    PARAM_EXPAND(addInclude(typeid(Cs)));
    return *this;
}

///////////////////////////////////////////////////////////
template <ComponentType... Cs> System& System::exclude() {
    PARAM_EXPAND(addExclude(typeid(Cs)));
    return *this;
}

///////////////////////////////////////////////////////////
namespace priv {
    template <typename Func, typename... Cs>
    System::IteratorFn makeSystemIteratorFn(Func&& fn, type_wrapper<std::tuple<Cs...>>) {
        // Get first parameter type
        using FirstParamType = typename first_param<std::decay_t<decltype(fn)>>::type;
        using DecayedType = std::remove_cv_t<std::remove_reference_t<FirstParamType>>;

        return [fn](
                   const std::vector<EntityId>& ids,
                   const HashMap<std::type_index, void*>& ptrs,
                   World* world,
                   EntityGroup* group,
                   float dt
               ) {
            // Create tuple bc it should be a little faster to access
            Tuple<Cs*...> tuple((Cs*)ptrs.find(typeid(Cs)).value()...);

            // Iterate number of entities, passing each component and id
            for (size_t i = 0; i < ids.size(); ++i) {
                if constexpr (std::is_same_v<DecayedType, QueryIterator>) {
                    QueryIterator it(ids[i], i, world, group, i, dt);
                    fn(it, tuple.template get<Cs*>()[i]...);
                } else if constexpr (std::is_same_v<DecayedType, EntityId>)
                    fn(ids[i], tuple.template get<Cs*>()[i]...);
                else if constexpr (std::is_integral_v<FirstParamType>)
                    fn(i, tuple.template get<Cs*>()[i]...);
                else
                    fn(tuple.template get<Cs*>()[i]...);
            }
        };
    }
}

///////////////////////////////////////////////////////////
template <typename Func> System* System::each(Func&& fn) {
    // Get first parameter type
    using FirstParamType = typename first_param<std::decay_t<decltype(fn)>>::type;
    using DecayedType = std::remove_cv_t<std::remove_reference_t<FirstParamType>>;

    // Check if first parameter is a meta type (QueryIterator, EntityId, or integral)
    constexpr bool HasMetaFirst = std::is_same_v<DecayedType, QueryIterator> ||
        std::is_same_v<DecayedType, EntityId> || std::is_integral_v<FirstParamType>;

    // Get component types from function parameters
    // If first parameter is meta, use rest_param_types, otherwise use param_types
    using CTypes = typename std::conditional_t<
        HasMetaFirst,
        typename rest_param_types<std::decay_t<decltype(fn)>>::type,
        typename param_types<std::decay_t<decltype(fn)>>::type>;

    // Create iterator
    m_iterator =
        priv::makeSystemIteratorFn(std::forward<Func>(fn), type_wrapper<decayed_tuple_t<CTypes>>{});

    // Register observer
    m_world->registerSystem(this);

    return this;
}

}