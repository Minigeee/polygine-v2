#include <ply/math/Functions.h>
#include <ply/math/Transform.h>

namespace ply {

///////////////////////////////////////////////////////////
Matrix4f toTransformMatrix(const Vector3f& t, const Vector3f& r, const Vector3f& k) {
    Vector3f rot(radians(r.x), radians(r.y), radians(r.z));
    Vector3f c(cos(rot.x), cos(rot.y), cos(rot.z));
    Vector3f s(sin(rot.x), sin(rot.y), sin(rot.z));

#ifndef USE_ROW_MAJOR
    return Matrix4f(
        k.x * (c.z * c.y),
        k.x * (s.z * c.y),
        k.x * (-s.y),
        0.0f,

        k.y * (-s.z * c.x + c.z * s.y * s.x),
        k.y * (c.z * c.x + s.z * s.y * s.x),
        k.y * (c.y * s.x),
        0.0f,

        k.z * (s.z * s.x + c.z * s.y * c.x),
        k.z * (-c.z * s.x + s.z * s.y * c.x),
        k.z * (c.y * c.x),
        0.0f,

        t.x, t.y, t.z, 1.0f);
#else
    return Matrix4f(
        k.x * (c.z * c.y),
        k.y * (-s.z * c.x + c.z * s.y * s.x),
        k.z * (s.z * s.x + c.z * s.y * c.x),
        t.x,

        k.x * (s.z * c.y),
        k.y * (c.z * c.x + s.z * s.y * s.x),
        k.z * (-c.z * s.x + s.z * s.y * c.x),
        t.y,

        k.x * (-s.y),
        k.y * (c.y * s.x),
        k.z * (c.y * c.x),
        t.z,

        0.0f, 0.0f, 0.0f, 1.0f);
#endif
}

///////////////////////////////////////////////////////////
Matrix4f toTransformMatrix(const Vector3f& t, const Quaternion& q, const Vector3f& k) {
#ifndef USE_ROW_MAJOR
    return Matrix4f(
        k.x * (1.0f - 2.0f * (q.y * q.y + q.z * q.z)),
        k.x * (2.0f * (q.x * q.y + q.w * q.z)),
        k.x * (2.0f * (q.x * q.z - q.w * q.y)),
        0.0f,

        k.y * (2.0f * (q.x * q.y - q.w * q.z)),
        k.y * (1.0f - 2.0f * (q.x * q.x + q.z * q.z)),
        k.y * (2.0f * (q.y * q.z + q.w * q.x)),
        0.0f,

        k.z * (2.0f * (q.x * q.z + q.w * q.y)),
        k.z * (2.0f * (q.y * q.z - q.w * q.x)),
        k.z * (1.0f - 2.0f * (q.x * q.x + q.y * q.y)),
        0.0f,

        t.x, t.y, t.z, 1.0f);
#else
    return Matrix4f(
        k.x * (1.0f - 2.0f * (q.y * q.y + q.z * q.z)),
        k.y * (2.0f * (q.x * q.y - q.w * q.z)),
        k.z * (2.0f * (q.x * q.z + q.w * q.y)),
        t.x,

        k.x * (2.0f * (q.x * q.y + q.w * q.z)),
        k.y * (1.0f - 2.0f * (q.x * q.x + q.z * q.z)),
        k.z * (2.0f * (q.y * q.z - q.w * q.x)),
        t.y,

        k.x * (2.0f * (q.x * q.z - q.w * q.y)),
        k.y * (2.0f * (q.y * q.z + q.w * q.x)),
        k.z * (1.0f - 2.0f * (q.x * q.x + q.y * q.y)),
        t.z,

        0.0f, 0.0f, 0.0f, 1.0f);
#endif
}

}  // namespace ply