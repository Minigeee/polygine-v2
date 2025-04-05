#pragma once

#include <ply/math/Types.h>

namespace ply
{

///////////////////////////////////////////////////////////
/// \brief A class representing a 3D sphere
///
///////////////////////////////////////////////////////////
class Sphere
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor that creates a 1 unit radius sphere
	///
	///////////////////////////////////////////////////////////
	Sphere();

	///////////////////////////////////////////////////////////
	/// \brief Create a sphere from position and radius
	///
	/// \param p The position of the sphere
	/// \param r The radius of the sphere
	///
	///////////////////////////////////////////////////////////
	Sphere(const Vector3f& p, float r);

	///////////////////////////////////////////////////////////
	/// \brief Check if a 3D point is inside the sphere
	///
	/// \param p The point to check
	///
	/// \return True if the point is inside the sphere
	///
	///////////////////////////////////////////////////////////
	bool contains(const Vector3f& p) const;

	///////////////////////////////////////////////////////////
	/// \brief Check if another sphere intersects or is inside the current sphere
	///
	/// \param s The other sphere to check
	///
	/// \return True if the sphere intersects or is inside the current sphere
	///
	///////////////////////////////////////////////////////////
	bool contains(const Sphere& s) const;

	Vector3f m_position;	//!< The position of the sphere
	float m_radius;			//!< The radius of the sphere
};

}  // namespace ply

///////////////////////////////////////////////////////////
/// \class ply::Sphere
/// \ingroup Math
///
/// A sphere is pretty self explanatory...
///
/// Usage example:
/// \code
///
/// // Create a sphere at (0, 10, 0) with a radius of 2
/// Sphere spehere(Vector3f(0.0f, 10.0f, 0.0f), 2.0f);
///
/// // Test if the sphere contains the origin
/// bool containsOrigin = sphere.contains(Vector3f(0.0f));
///
/// \endcode
///
///////////////////////////////////////////////////////////