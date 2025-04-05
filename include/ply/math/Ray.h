#pragma once

#include <ply/math/Types.h>

namespace ply {

///////////////////////////////////////////////////////////
/// \brief A struct used to represent an infinite ray in 3D space
///
///////////////////////////////////////////////////////////
struct Ray {
    ///////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ///////////////////////////////////////////////////////////
    Ray();

    ///////////////////////////////////////////////////////////
    /// \brief Create a ray from an origin and a direction
    ///
    /// The direction will not be normalized
    ///
    ///////////////////////////////////////////////////////////
    Ray(const Vector3f& origin, const Vector3f& direction);

    Vector3f m_origin;     //!< The origin of the ray
    Vector3f m_direction;  //!< The direction of the ray
};

}  // namespace ply

///////////////////////////////////////////////////////////
/// \struct ply::Ray
/// \ingroup Math
///
/// Usage example:
/// \code
///
/// using namespace ply;
///
/// Ray ray;
/// ray.m_position = Vector3f(0.0f, 10.0f, 0.0f);
/// ray.m_direction = Vector3f(0.0f, -1.0f, 0.0f);
///
/// \endcode
///
///////////////////////////////////////////////////////////