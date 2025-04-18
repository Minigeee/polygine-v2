#pragma once

#include <ply/math/Types.h>

namespace ply {

///////////////////////////////////////////////////////////
/// \brief An engine components that describes an entity's
///	       position, rotation, and scale
/// \ingroup Components
///
///////////////////////////////////////////////////////////
struct Transform {
    Vector3f position = Vector3f(0.0f);               //!< The position
    Quaternion rotation = glm::identity<glm::quat>(); //!< The rotation
    Vector3f scale = Vector3f(1.0f);                  //!< The scale
};

///////////////////////////////////////////////////////////
/// \brief Dynamic spatial component tag
/// \ingroup Components
///
/// Notifies engine that the entity transform will be updated many times.
///
///////////////////////////////////////////////////////////
struct T_Dynamic {};

} // namespace ply