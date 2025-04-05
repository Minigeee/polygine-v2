#ifndef PLY_FRUSTUM_H
#define PLY_FRUSTUM_H

#include <ply/math/BoundingBox.h>
#include <ply/math/Plane.h>
#include <ply/math/Sphere.h>

namespace ply
{

///////////////////////////////////////////////////////////
/// \brief A class representing a frustum, which is a pyramid with its top cut off
///
///////////////////////////////////////////////////////////
class Frustum
{
public:
	///////////////////////////////////////////////////////////
	/// \brief An enum defining the sides of a frustum
	///
	///////////////////////////////////////////////////////////
	enum Side
	{
		Left = 0,	//!< The left side of the frustum
		Right,		//!< The right side of the frustum
		Bottom,		//!< The bottom side of the frustum
		Top,		//!< The top side of the frustum
		Near,		//!< The closest side of the frustum
		Far			//!< The furthes side of the frustum
	};

public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	Frustum() = default;

	///////////////////////////////////////////////////////////
	/// \brief Set the value of the specified plane
	///
	/// The 6 possible configurable planes are:
	///
	/// \li Left
	/// \li Right
	/// \li Bottom
	/// \li Top
	/// \li Near
	/// \li Far
	///
	/// \param plane The value of the plane to set
	/// \param side The side to change
	///
	///////////////////////////////////////////////////////////
	void setPlane(const Plane& plane, Side side);

	///////////////////////////////////////////////////////////
	/// \brief Get the value of the specified plane
	///
	/// The 6 possible planes to retrieve are:
	///
	/// \li Left
	/// \li Right
	/// \li Bottom
	/// \li Top
	/// \li Near
	/// \li Far
	///
	/// \param side The side to change
	///
	/// \return A plane in the frustum
	///
	///////////////////////////////////////////////////////////
	const Plane& getPlane(Side side) const;

	///////////////////////////////////////////////////////////
	/// \brief Check if a bounding box intersects or is inside the frustum
	///
	/// \param box The bounding box to test
	///
	/// \return True if the box intersects or is inside the frustum
	///
	///////////////////////////////////////////////////////////
	bool contains(const BoundingBox& box) const;

	///////////////////////////////////////////////////////////
	/// \brief Check if a bounding sphere intersects or is inside the frustum
	///
	/// \param sphere The bounding sphere to test
	///
	/// \return True if the sphere intersects or is inside the frustum
	///
	///////////////////////////////////////////////////////////
	bool contains(const Sphere& sphere) const;

private:
	Plane m_planes[6]; //!< The planes array
};

}

#endif