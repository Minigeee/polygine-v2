#pragma once

#include <ply/core/Tuple.h>
#include <ply/ecs/World.h>

namespace ply {

///////////////////////////////////////////////////////////
template <ComponentType... Cs> QueryFactory& QueryFactory::match() {
    PARAM_EXPAND(addInclude(typeid(Cs)));
    return *this;
}

///////////////////////////////////////////////////////////
template <ComponentType... Cs> QueryFactory& QueryFactory::exclude() {
    PARAM_EXPAND(addExclude(typeid(Cs)));
    return *this;
}

///////////////////////////////////////////////////////////
template <typename Func> void Query::each(Func&& fn) {
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

    iterate(std::forward<Func>(fn), type_wrapper<decayed_tuple_t<CTypes>>{});
}

///////////////////////////////////////////////////////////
template <typename Func, typename... Cs>
void Query::iterate(Func&& fn, type_wrapper<std::tuple<Cs...>>) {
    // Get first parameter type
    using FirstParamType = typename first_param<std::decay_t<decltype(fn)>>::type;
    using DecayedType = std::remove_cv_t<std::remove_reference_t<FirstParamType>>;

    QueryFactory& q = *m_factory;

    // Lock user mutexes if provided
    std::vector<std::unique_lock<std::mutex>> locks;
    locks.reserve(q.m_mutexes.size());
    for (size_t m = 0; m < q.m_mutexes.size(); ++m)
        locks.push_back(std::unique_lock<std::mutex>(*q.m_mutexes[m]));

    // Iterate through each table, invoking function for each entity
    for (size_t t = 0, cum = 0 /* hehe */; t < q.m_groups.size(); ++t) {
        EntityGroup& group = *m_world->m_groups[q.m_groups[t]];

        // Lock table
        ReadLock lock(group.m_mutex);

        // Create tuple bc it should be a little faster to access
        Tuple<Cs*...> tuple((Cs*)group.m_components[typeid(Cs)].data()...);

        // Iterate number of entities, passing each component and id
        for (size_t i = 0; i < group.m_entities.size(); ++i) {
            if constexpr (std::is_same_v<DecayedType, QueryIterator>) {
                QueryIterator it(
                    group.m_entities[i], cum + i, m_world, &group, i, m_world->m_elapsed
                );
                fn(it, tuple.template get<Cs*>()[i]...);
            } else if constexpr (std::is_same_v<DecayedType, EntityId>)
                fn(group.m_entities[i], tuple.template get<Cs*>()[i]...);
            else if constexpr (std::is_integral_v<FirstParamType>)
                fn(cum + i, tuple.template get<Cs*>()[i]...);
            else
                fn(tuple.template get<Cs*>()[i]...);
        }

        // Update aggregate index
        cum += group.m_entities.size();
    }
}

} // namespace ply