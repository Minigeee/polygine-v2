#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace ply {

typedef glm::ivec2 Vector2i;
typedef glm::uvec2 Vector2u;
typedef glm::vec2 Vector2f;
typedef glm::dvec2 Vector2d;
typedef glm::bvec2 Vector2b;

typedef glm::ivec3 Vector3i;
typedef glm::uvec3 Vector3u;
typedef glm::vec3 Vector3f;
typedef glm::dvec3 Vector3d;

typedef glm::ivec4 Vector4i;
typedef glm::uvec4 Vector4u;
typedef glm::vec4 Vector4f;
typedef glm::dvec4 Vector4d;

typedef glm::mat4 Matrix4f;
typedef glm::dmat4 Matrix4d;

typedef glm::quat Quaternion;
typedef glm::dquat Quaterniond;

}  // namespace ply