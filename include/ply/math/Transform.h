#pragma once

#include <ply/math/Types.h>

#include <glm/ext/matrix_transform.hpp>

namespace ply {

///////////////////////////////////////////////////////////
/// \brief Create a transform matrix from position, rotation, and scale
///
/// \param t Position
/// \param r Rotation
/// \param s Scale
///
/// \return The resulting transform matrix
///
///////////////////////////////////////////////////////////
Matrix4f toTransformMatrix(const Vector3f& t, const Vector3f& r, const Vector3f& s);

///////////////////////////////////////////////////////////
/// \brief Create a transform matrix from position, quaternion, and scale
///
/// \param t Position
/// \param q Quaternion orientation
/// \param s Scale
///
/// \return The resulting transform matrix
///
///////////////////////////////////////////////////////////
Matrix4f toTransformMatrix(const Vector3f& t, const Quaternion& r, const Vector3f& s);

using glm::perspective;
using glm::ortho;
using glm::lookAt;

}  // namespace ply
