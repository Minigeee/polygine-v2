#pragma once

#include <ply/math/Types.h>

namespace ply {

///////////////////////////////////////////////////////////
/// \brief A class representing a 3D axis-aligned bounding box
///
///////////////////////////////////////////////////////////
class BoundingBox {
   public:
    ///////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ///////////////////////////////////////////////////////////
    BoundingBox();

    ///////////////////////////////////////////////////////////
    /// \brief Create a bounding box from a minimum coordinate and a maximum coordinate
    ///
    /// \param min The minimum coordinate
    /// \param max The maximum coordinate
    ///
    ///////////////////////////////////////////////////////////
    BoundingBox(const Vector3f& min, const Vector3f& max);

    ///////////////////////////////////////////////////////////
    /// \brief Get the center coordinate of the bounding box
    ///
    /// \return The center coordinate
    ///
    ///////////////////////////////////////////////////////////
    Vector3f getCenter() const;

    ///////////////////////////////////////////////////////////
    /// \brief Get the size of the bounding box
    ///
    /// \return The size or dimentions of the bounding box
    ///
    ///////////////////////////////////////////////////////////
    Vector3f getDimensions() const;

    ///////////////////////////////////////////////////////////
    /// \brief Check if this bounding box overlaps another bounding box
    ///
    /// This function returns if there is any intersection of the
    /// two boxes.
    ///
    /// \param bbox The other bounding box to test against
    ///
    /// \return True if the bounding boxes overlap
    ///
    ///////////////////////////////////////////////////////////
    bool overlaps(const BoundingBox& bbox);

    Vector3f m_min;  //!< The minimum coordinate of the box
    Vector3f m_max;  //!< The maximum coordinate of the box
};

}  // namespace ply

///////////////////////////////////////////////////////////
/// \class ply::BoundingBox
/// \ingroup Math
///
/// The same thing as a 3D axis-aligned regtangular prsim.
///
/// Usage example:
/// \code
///
/// using namespace ply;
///
/// // Create a bounding box of size (2, 2, 2) centered at the origin
/// BoundingBox box(Vector3f(-1.0f), Vector3f(1.0f));
///
/// box.getCenter();		// (0, 0, 0)
/// box.getDimensions();	// (2, 2, 2)
///
/// \endcode
///
///////////////////////////////////////////////////////////