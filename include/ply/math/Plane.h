#pragma once

#include <ply/math/Types.h>

namespace ply {

///////////////////////////////////////////////////////////
/// \brief A class representing a 2D plane in 3D space
///
///////////////////////////////////////////////////////////
class Plane {
public:
    ///////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ///////////////////////////////////////////////////////////
    Plane() = default;

    ///////////////////////////////////////////////////////////
    /// \brief Create a plane from a normal vector and distance
    ///
    /// Not really sure if this is accurate.
    ///
    /// \param n The normal vector
    /// \param d The distance to origin along the normal vector
    ///
    ///////////////////////////////////////////////////////////
    Plane(const Vector3f& n, float d);

    ///////////////////////////////////////////////////////////
    /// \brief Create a plane using its 4 equation constants
    ///
    /// Ax + By + Cz = D
    ///
    /// \param a The a constant
    /// \param b The b constant
    /// \param c The c constant
    /// \param d The d constant
    ///
    ///////////////////////////////////////////////////////////
    Plane(float a, float b, float c, float d);

    ///////////////////////////////////////////////////////////
    /// \brief Reflect a 3D point across the plane
    ///
    /// \param p The point to reflect acrss the plane
    ///
    /// \return The reflected point
    ///
    ///////////////////////////////////////////////////////////
    Vector3f reflectPoint(const Vector3f& p) const;

    ///////////////////////////////////////////////////////////
    /// \brief Reflect a 3D vector, or direction, across the plane
    ///
    /// \param v The vector to reflect
    ///
    /// \return The reflected vector
    ///
    ///////////////////////////////////////////////////////////
    Vector3f reflectVector(const Vector3f& v) const;

    Vector3f n;
    float d;
};

///////////////////////////////////////////////////////////
/// \brief Get the signed distance between a plane and a 3D point
///
/// This will return a distance where points that are in the
/// direction of the normal vector will be positive, and points
/// on the other side will be negative.
///
/// \param plane The plane to measure distance from
/// \param p The point to measure distance to
///
/// \ret The signed distance
///
///////////////////////////////////////////////////////////
float dist(const Plane& plane, const Vector3f& p);

} // namespace ply
