#pragma once

#include <glm/common.hpp>
#include <glm/exponential.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>

namespace ply {

// Geometric functions
using glm::cross;
using glm::distance;
using glm::dot;
using glm::length;
using glm::normalize;

// Common functions
using glm::abs;
using glm::ceil;
using glm::clamp;
using glm::floor;
using glm::fma;
using glm::fract;
using glm::frexp;
using glm::ldexp;
using glm::max;
using glm::min;
using glm::mix;
using glm::mod;
using glm::modf;
using glm::round;
using glm::roundEven;
using glm::sign;
using glm::smoothstep;
using glm::step;
using glm::trunc;

// Type conversion functions
using glm::floatBitsToInt;
using glm::floatBitsToUint;
using glm::intBitsToFloat;
using glm::uintBitsToFloat;

// Query functions
using glm::isinf;
using glm::isnan;

// Angle conversion
using glm::degrees;
using glm::radians;

constexpr float PI = 3.14159265f;

}  // namespace ply
