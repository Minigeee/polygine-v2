#include <ply/math/Plane.h>

namespace ply {

///////////////////////////////////////////////////////////
Plane::Plane(const Vector3f& n, float d) {
    float mag = length(n);
    this->n = n / mag;
    this->d = d / mag;
}

///////////////////////////////////////////////////////////
Plane::Plane(float a, float b, float c, float d) :
    n(a, b, c),
    d(d) {
    float mag = length(n);
    this->n /= mag;
    this->d /= mag;
}

///////////////////////////////////////////////////////////
Vector3f Plane::reflectPoint(const Vector3f& p) const {
    return p - 2.0f * n * (dot(n, p) + d);
}

///////////////////////////////////////////////////////////
Vector3f Plane::reflectVector(const Vector3f& v) const {
    return v - 2.0f * n * dot(v, n) / (n.x * n.x + n.y * n.y + n.z * n.z);
}

///////////////////////////////////////////////////////////
float dist(const Plane& plane, const Vector3f& p) {
    return dot(plane.n, p) + plane.d;
}

} // namespace ply