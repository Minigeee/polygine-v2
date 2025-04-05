namespace ply {

namespace priv {

template <typename T>
inline TupleData<T>::TupleData(const T& data) : m_data(data) {}

}  // namespace priv

template <typename... Ts>
inline Tuple<Ts...>::Tuple(Ts&&... args) : priv::TupleData<Ts>(std::forward<Ts>(args))... {}

template <typename... Ts>
template <typename T>
inline void Tuple<Ts...>::set(const T& value) {
    static_cast<priv::TupleData<T>*>(this)->m_data = value;
}

template <typename... Ts>
template <typename T>
inline T& Tuple<Ts...>::get() {
    return static_cast<priv::TupleData<T>*>(this)->m_data;
}

template <typename... Ts>
template <typename T>
inline const T& Tuple<Ts...>::get() const {
    return static_cast<priv::TupleData<T>*>(this)->m_data;
}

template <typename... Ts>
inline constexpr uint32_t Tuple<Ts...>::getNumElements() const {
    return sizeof...(Ts);
}

///////////////////////////////////////////////////////////

template <typename... Ts>
inline Tuple<Ts...> makeTuple(Ts&&... args) {
    return Tuple<Ts...>(std::forward<Ts>(args)...);
}

template <int N, typename... Ts>
typename std::tuple_element_t<N, std::tuple<Ts...>>& get(Tuple<Ts...>& t) {
    return t.template get<typename std::tuple_element_t<N, std::tuple<Ts...>>>();
}

}  // namespace ply
