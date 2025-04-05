#pragma once

#include <tuple>

namespace ply {

// Skip documentation for private
#ifndef DOXYGEN_SKIP

namespace priv {

///////////////////////////////////////////////////////////
// Unique variadic template parameters
///////////////////////////////////////////////////////////

template <typename... Args>
struct ParameterPack {};

///////////////////////////////////////////////////////////

template <typename... Args>
struct ParamsContain;

template <typename A, typename... Args, typename B>
struct ParamsContain<ParameterPack<A, Args...>, B> {
    static constexpr bool value = ParamsContain<ParameterPack<Args...>, B>::value;
};

template <typename A, typename... Args>
struct ParamsContain<ParameterPack<A, Args...>, A> {
    static constexpr bool value = true;
};

template <typename A>
struct ParamsContain<ParameterPack<>, A> {
    static constexpr bool value = false;
};

///////////////////////////////////////////////////////////

template <typename... Args>
struct IsUnique;

template <typename A, typename... Args>
struct IsUnique<A, Args...> {
    static constexpr bool value = !ParamsContain<ParameterPack<Args...>, A>::value && IsUnique<Args...>::value;
};

template <typename A>
struct IsUnique<A> {
    static constexpr bool value = true;
};

template <>
struct IsUnique<> {
    static constexpr bool value = true;
};

///////////////////////////////////////////////////////////

template <typename T>
struct TupleData {
    TupleData() = default;
    TupleData(const T& data);

    T m_data;
};

}  // namespace priv

#endif

///////////////////////////////////////////////////////////
/// \brief A custom tuple class that stores unique data types
///
///////////////////////////////////////////////////////////
template <typename... Ts>
class Tuple : public priv::TupleData<Ts>... {
    static_assert(priv::IsUnique<Ts...>::value, "tuples are not allowed to have duplicate types");

   public:
    ///////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ///////////////////////////////////////////////////////////
    Tuple() = default;

    ///////////////////////////////////////////////////////////
    /// \brief Construct a tuple from its elements
    ///
    /// \param elements The tuple elements
    ///
    ///////////////////////////////////////////////////////////
    Tuple(Ts&&... elements);

    ///////////////////////////////////////////////////////////
    /// \brief Set the value of an element in the tuple
    ///
    /// Because all element types are unique from each other,
    /// the position of an element in the tuple does not have to
    /// be known (as with std::tuple and std::get), only the element
    /// type has to be specified.
    ///
    /// \param value The new value to assign the tuple element
    ///
    ///////////////////////////////////////////////////////////
    template <typename T>
    void set(const T& value);

    ///////////////////////////////////////////////////////////
    /// \brief Get the value of an element in the tuple by type
    ///
    /// Because all element types are unique from each other,
    /// the position of an element in the tuple does not have to
    /// be known (as with std::tuple and std::get), only the element
    /// type has to be specified.
    ///
    /// \return A reference to a tuple element
    ///
    ///////////////////////////////////////////////////////////
    template <typename T>
    T& get();

    ///////////////////////////////////////////////////////////
    /// \brief Get the value of an element in the tuple by type
    ///
    /// Because all element types are unique from each other,
    /// the position of an element in the tuple does not have to
    /// be known (as with std::tuple and std::get), only the element
    /// type has to be specified.
    ///
    /// \return A const reference to a tuple element
    ///
    ///////////////////////////////////////////////////////////
    template <typename T>
    const T& get() const;

    ///////////////////////////////////////////////////////////
    /// \brief Get the number of elements in the tuple
    ///
    /// \return The number of elements in the tuple
    ///
    ///////////////////////////////////////////////////////////
    constexpr uint32_t getNumElements() const;
};

///////////////////////////////////////////////////////////
/// \brief Make a tuple from its elements
///
/// This is alternative to the tuple constructor.
///
/// \param elements The tuple elements
///
/// \return A tuple
///
///////////////////////////////////////////////////////////
template <typename... Ts>
Tuple<Ts...> makeTuple(Ts&&... elements);

///////////////////////////////////////////////////////////
/// \brief Get the value of an element in the tuple by position
///
/// This is the same as std::get, but it works for poly::Tuple.
///
/// \param t The tuple to retrieve an element from
///
/// \return A reference to a tuple element
///
///////////////////////////////////////////////////////////
template <int N, typename... Ts>
typename std::tuple_element_t<N, std::tuple<Ts...>>& get(Tuple<Ts...>& t);

}  // namespace ply

#include <ply/core/Tuple.inl>

///////////////////////////////////////////////////////////
/// \class Tuple
/// \ingroup Core
///
/// Tuple is a class that stores data, where each element that
/// is stored must be unique from other elemenents in the same
/// tuple. This class is similar to std::tuple, but has a few
/// differences.
///
/// Because every element in the tuple is forced to be unique
/// from each other, elements can be accessed by specifying its
/// type instead of by index (like std::tuple and std::get).
/// To access and modify the tuple elements, use the functions
/// Tuple::set() and Tuple::get().
///
/// It is still possible to access tuple elements by index,
/// by using poly::get().
///
/// If a tuple with multiple elements of the same type are needed,
/// use std::tuple instead.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// // Create a tuple using the default constructor
/// Tuple<int, float> t1;
///
/// // Create a tuple using its elements
/// Tuple<int, float, bool> t2(1, 2.0f, false);
///
/// // Set the tuple elements
/// // The template parameters aren't needed for set(), but it helps clarify which element to set
/// t1.set<int>(3);
/// t1.set<float>(50.0f);
///
/// // Get tuple elements by type
/// std::cout << t1.get<float>() << '\n';
///
/// // Get tuple elements by index
/// get<2>(t2) = true;
/// std::cout << get<0>(t1) << '\n';
///
/// \endcode
///
///////////////////////////////////////////////////////////