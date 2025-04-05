#pragma once

#include <ply/core/Handle.h>
#include <type_traits>

namespace ply {

typedef Handle EntityId;

///////////////////////////////////////////////////////////
/// \brief Constraint on components using this system
///
///////////////////////////////////////////////////////////
template<class T>
concept ComponentType = std::is_standard_layout_v<T> && std::is_trivially_copyable_v<T>;

}

#define VALID_COMPONENT_TYPE(T) std::is_standard_layout_v<T> && std::is_trivially_copyable_v<T>
