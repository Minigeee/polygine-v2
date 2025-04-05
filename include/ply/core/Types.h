#pragma once

#include <tsl/hopscotch_map.h>
#include <tsl/hopscotch_set.h>
#include <tuple>

namespace ply {

// Data structure implementations
template <class Key, class T>
using HashMap = tsl::hopscotch_map<Key, T>;

template <class Key>
using HashSet = tsl::hopscotch_set<Key>;

} // namespace ply

///////////////////////////////////////////////////////////
/// \brief Utility to get the first parameter type of a function
///
///////////////////////////////////////////////////////////
template <typename T>
struct first_param;

// Specialization for regular functions
template <typename R, typename First, typename... Rest>
struct first_param<R(First, Rest...)> {
    using type = First;
};

// Specialization for function pointers
template <typename R, typename First, typename... Rest>
struct first_param<R (*)(First, Rest...)> {
    using type = First;
};

// Specialization for member functions
template <typename R, typename C, typename First, typename... Rest>
struct first_param<R (C::*)(First, Rest...)> {
    using type = First;
};

// Specialization for const member functions
template <typename R, typename C, typename First, typename... Rest>
struct first_param<R (C::*)(First, Rest...) const> {
    using type = First;
};

// Specialization for function objects with operator()
template <typename T>
struct first_param {
    using type = typename first_param<decltype(&T::operator())>::type;
};

///////////////////////////////////////////////////////////
/// \brief Utility to get all parameter types of a function
///
///////////////////////////////////////////////////////////
template <typename T>
struct param_types;

// Specialization for regular functions
template <typename R, typename... Args>
struct param_types<R(Args...)> {
    using type = std::tuple<Args...>;
};

// Specialization for function pointers
template <typename R, typename... Args>
struct param_types<R (*)(Args...)> {
    using type = std::tuple<Args...>;
};

// Specialization for member functions
template <typename R, typename C, typename... Args>
struct param_types<R (C::*)(Args...)> {
    using type = std::tuple<Args...>;
};

// Specialization for const member functions
template <typename R, typename C, typename... Args>
struct param_types<R (C::*)(Args...) const> {
    using type = std::tuple<Args...>;
};

// Specialization for function objects with operator()
template <typename T>
struct param_types {
    using type = typename param_types<decltype(&T::operator())>::type;
};

///////////////////////////////////////////////////////////
/// \brief Utility to get all parameter types except the first one
///
///////////////////////////////////////////////////////////
template <typename T>
struct rest_param_types;

// Specialization for regular functions
template <typename R, typename First, typename... Rest>
struct rest_param_types<R(First, Rest...)> {
    using type = std::tuple<Rest...>;
};

// Specialization for function pointers
template <typename R, typename First, typename... Rest>
struct rest_param_types<R (*)(First, Rest...)> {
    using type = std::tuple<Rest...>;
};

// Specialization for member functions
template <typename R, typename C, typename First, typename... Rest>
struct rest_param_types<R (C::*)(First, Rest...)> {
    using type = std::tuple<Rest...>;
};

// Specialization for const member functions
template <typename R, typename C, typename First, typename... Rest>
struct rest_param_types<R (C::*)(First, Rest...) const> {
    using type = std::tuple<Rest...>;
};

// Specialization for function objects with operator()
template <typename T>
struct rest_param_types {
    using type = typename rest_param_types<decltype(&T::operator())>::type;
};

///////////////////////////////////////////////////////////
/// \brief Utility to decay all types in a tuple
///
/// Takes a tuple type and applies std::decay to each element
///////////////////////////////////////////////////////////
template <typename T>
struct decayed_tuple;

// Specialization for tuples
template <typename... Args>
struct decayed_tuple<std::tuple<Args...>> {
    using type = std::tuple<std::remove_cv_t<std::remove_reference_t<Args>>...>;
};

// Helper alias template
template <typename T>
using decayed_tuple_t = typename decayed_tuple<T>::type;

///////////////////////////////////////////////////////////
/// \brief Utility struct to wrap a type
///
///////////////////////////////////////////////////////////
template <typename T>
struct type_wrapper { using type = T; };