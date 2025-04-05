#include <ply/math/Frustum.h>

namespace ply {

///////////////////////////////////////////////////////////
void Frustum::setPlane(const Plane& plane, Side side) {
    m_planes[side] = plane;
}

///////////////////////////////////////////////////////////
const Plane& Frustum::getPlane(Side side) const {
    return m_planes[side];
}

///////////////////////////////////////////////////////////
bool Frustum::contains(const BoundingBox& box) const {
    const Vector3f& min = box.m_min;
    const Vector3f& max = box.m_max;

    for (uint32_t i = 0; i < 6; ++i) {
        const Plane& plane = m_planes[i];

        Vector3f vmin;
        vmin.x = plane.n.x > 0.0f ? max.x : min.x;
        vmin.y = plane.n.y > 0.0f ? max.y : min.y;
        vmin.z = plane.n.z > 0.0f ? max.z : min.z;

        if (distance(plane, vmin) < 0.0f)
            return false;
    }

    return true;
}

///////////////////////////////////////////////////////////
bool Frustum::contains(const Sphere& sphere) const {
    bool contains = true;

    for (uint32_t i = 0; i < 6; ++i) {
        const Plane& plane = m_planes[i];

        if (distance(plane, sphere.m_position) + sphere.m_radius < 0.0f)
            contains = false;
    }

    return contains;
}

}  // namespace ply