#pragma once

#include <ply/math/Types.h>

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

///////////////////////////////////////////////////////////
/// \brief Create a view matrix based on a camera's position, front vector, and right vector
///
/// \param p Position
/// \param f The normalized front vector
/// \param r The normalized right vector
///
/// \return The resulting view matrix
///
///////////////////////////////////////////////////////////
Matrix4f toViewMatrix(const Vector3f& p, const Vector3f& f, const Vector3f& r);

///////////////////////////////////////////////////////////
/// \brief Create a perspective projection matrix based on camera fov, aspect ration, near plane, and far plane
///
/// \param fov The field of view
/// \param ar The aspect ratio
/// \param near The distance to the near plane
/// \param far The distance to the far plane
///
/// \return The resulting projection matrix
///
///////////////////////////////////////////////////////////
Matrix4f toPerspectiveMatrix(float fov, float ar, float near, float far);

///////////////////////////////////////////////////////////
/// \brief Create an orthographic projection matrix based on left, right, bottom, top, near, and far planes
///
/// \param left The distance from the camera to the left orthographic plane
/// \param right The distance from the camera to the right orthographic plane
/// \param bottom The distance from the camera to the bottom orthographic plane
/// \param top The distance from the camera to the top orthographic plane
/// \param near The distance from the camera to the near orthographic plane
/// \param far The distance from the camera to the far orthographic plane
///
/// \return The resulting projection matrix
///
///////////////////////////////////////////////////////////
Matrix4f toOrthographicMatrix(float left, float right, float bottom, float top, float near, float far);

}  // namespace ply
