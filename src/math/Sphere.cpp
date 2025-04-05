#include <ply/math/Functions.h>
#include <ply/math/Sphere.h>

namespace ply {

///////////////////////////////////////////////////////////
Sphere::Sphere() : m_position(0.0f),
                   m_radius(1.0f) {
}

///////////////////////////////////////////////////////////
Sphere::Sphere(const Vector3f& p, float r) : m_position(p),
                                             m_radius(r) {
}

///////////////////////////////////////////////////////////
bool Sphere::contains(const Vector3f& p) const {
    return distance(m_position, p) < m_radius;
}

///////////////////////////////////////////////////////////
bool Sphere::contains(const Sphere& s) const {
    return distance(m_position, s.m_position) < m_radius + s.m_radius;
}

}  // namespace ply