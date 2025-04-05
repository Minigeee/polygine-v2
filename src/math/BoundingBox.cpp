#include <ply/math/BoundingBox.h>

namespace ply {

///////////////////////////////////////////////////////////
BoundingBox::BoundingBox() : m_min(0.0f),
                             m_max(0.0f) {
}

///////////////////////////////////////////////////////////
BoundingBox::BoundingBox(const Vector3f& min, const Vector3f& max) : m_min(min),
                                                                     m_max(max) {
}

///////////////////////////////////////////////////////////
Vector3f BoundingBox::getCenter() const {
    return (m_min + m_max) * 0.5f;
}

///////////////////////////////////////////////////////////
Vector3f BoundingBox::getDimensions() const {
    return m_max - m_min;
}

///////////////////////////////////////////////////////////
bool BoundingBox::overlaps(const BoundingBox& bbox) {
    return (m_min.x <= bbox.m_max.x && m_max.x >= bbox.m_min.x) &&
           (m_min.y <= bbox.m_max.y && m_max.y >= bbox.m_min.y) &&
           (m_min.z <= bbox.m_max.z && m_max.z >= bbox.m_min.z);
}

}  // namespace ply