#pragma once

#include <ply/math/Types.h>

namespace ply {

///////////////////////////////////////////////////////////
/// \brief A component that defines properties of a directional light
///
///////////////////////////////////////////////////////////
struct DirectionalLight {
    /// The color of the light
    Vector3f color = Vector3f{1.0f};
    /// The direction of the light
    Vector3f direction = {0.0f, -1.0f, 0.0f};
};

///////////////////////////////////////////////////////////
/// \brief A component that defines properties of a point light
///
/// A point light is a light that emits light in all directions
/// equally, and its intensity grows weaker as the distance from
/// the light increases. The fadeout of intensity can be controlled
/// with the 3 attenuation coefficients, where the x-component is the
/// \a constant coefficient, the y-component is the \a linear coefficient,
/// and the z-component is the \a quadratic coefficient.
///
/// \note The position of the point light can be set by attaching
/// a Transform component to the same entity that contains this
/// point light.
///
/// \note There is a maximum radius from the camera, where point
/// lights outside the range will be disabled. This is for performance
/// issues.
///
///////////////////////////////////////////////////////////
struct PointLight {
    /// The color of the light
    Vector3f color = Vector3f{1.0f};
    /// The range of the light
    float range = 5.0f;
    /// The attenuation factor of the light, determines how quickly the light
    /// fades out with distance
    float attenuation = 2.0f;
};

///////////////////////////////////////////////////////////
/// \brief Struct containing shadow properties, which are treated as global
/// properties
///
///////////////////////////////////////////////////////////
struct Shadows {
    /// The base resolution of the shadow map
    uint32_t resolution = 2048;
    /// The closest distance shadows should be rendered
    float startDistance = 0.0f;
    /// The furthest distance shadows should be rendered
    float distance = 50.0f;
    /// The amount of light blocked by shadowed areas
    float strength = 1.0f;
    /// The number of shadow cascades to use
    uint32_t numCascades = 3;
    /// The shadow distance cutoff multplier between each cascade level
    float cascadeDistMultiplier = 1.5f;
    /// Flag that determines if the directional light should cast shadows or not
    bool enabled = true;
};

} // namespace ply