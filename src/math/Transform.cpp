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

///////////////////////////////////////////////////////////
Matrix4f toViewMatrix(const Vector3f& p, const Vector3f& f, const Vector3f& r) {
    Vector3f u = normalize(cross(r, f));

#ifndef USE_ROW_MAJOR
    return Matrix4f(
        r.x, u.x, -f.x, 0.0f,
        r.y, u.y, -f.y, 0.0f,
        r.z, u.z, -f.z, 0.0f,
        -dot(r, p), -dot(u, p), dot(f, p), 1.0f);
#else
    return Matrix4f(
        r.x, r.y, r.z, -dot(r, p),
        u.x, u.y, u.z, -dot(u, p),
        -f.x, -f.y, -f.z, dot(f, p),
        0.0f, 0.0f, 0.0f, 1.0f);
#endif
}

///////////////////////////////////////////////////////////
Matrix4f toPerspectiveMatrix(float fovy, float ar, float near, float far) {
    fovy = radians(fovy);

#ifndef USE_ROW_MAJOR
    return Matrix4f(
        1.0f / (ar * fovy), 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f / fovy, 0.0f, 0.0f,
        0.0f, 0.0f, -(far + near) / (far - near), -1.0f,
        0.0f, 0.0f, -2.0f * far * near / (far - near), 0.0f);
#else
    return Matrix4f(
        1.0f / (ar * fovy), 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f / fovy, 0.0f, 0.0f,
        0.0f, 0.0f, -(far + near) / (far - near), -2.0f * far * near / (far - near),
        0.0f, 0.0f, -1.0f, 0.0f);
#endif
}

///////////////////////////////////////////////////////////
Matrix4f toOrthographicMatrix(float left, float right, float bottom, float top, float near, float far) {
#ifndef USE_ROW_MAJOR
    return Matrix4f(
        2.0f / (right - left), 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f / (top - bottom), 0.0f, 0.0f,
        0.0f, 0.0f, -2.0f / (far - near), 0.0f,

        -(right + left) / (right - left),
        -(top + bottom) / (top - bottom),
        -(far + near) / (far - near),
        1.0f);
#else
    return Matrix4f(
        2.0f / (right - left), 0.0f, 0.0f, -(right + left) / (right - left),
        0.0f, 2.0f / (top - bottom), 0.0f, -(top + bottom) / (top - bottom),
        0.0f, 0.0f, -2.0f / (far - near), -(far + near) / (far - near),
        0.0f, 0.0f, 0.0f, 1.0f);
#endif
}

}  // namespace ply