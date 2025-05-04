#pragma once

#include <ply/math/Types.h>

namespace ply {

class Texture;

///////////////////////////////////////////////////////////
/// \brief A material that defines the appearance of a mesh
///
///////////////////////////////////////////////////////////
struct Material {
    /// The color of the material
    Vector3f albedoColor = Vector3f{1.0f};
    /// Albedo texture of the material
    Texture* albedoTexture = nullptr;
    /// The metallic factor of the material, where 0.0 is non-metallic and 1.0
    /// is metallic
    float metallic = 0.0f;
    /// The metallic texture of the material
    Texture* metallicTexture = nullptr;
    /// The roughness factor of the material, where 0.0 is smooth and 1.0 is
    /// rough
    float roughness = 1.0f;
    /// The roughness texture of the material
    Texture* roughnessTexture = nullptr;
    /// The specular intensity of the material, where 0.0 is no specular and
    /// 1.0 is full specular
    float specularIntensity = 1.0f;
    /// The emission color of the material
    Vector3f emissionColor = Vector3f{0.0f};
    /// The emission intensity of the material, where 0.0 is no emission and
    /// 1.0 is full emission
    float emissionIntensity = 1.0f;
    /// The emission intensity texture of the material
    Texture* emissionTexture = nullptr;
    /// The rim light intensity of the material, where 0.0 is no rim light and
    /// 1.0 is full rim light
    float rimIntensity = 1.0f;
    /// The rim light power of the material, where higher values make the rim
    /// light more focused on the edges
    float rimPower = 2.5f;
    /// The rim light color of the material
    Vector3f rimColor = Vector3f{1.0f};
    /// Rim light intensity texture of the material
    Texture* rimTexture = nullptr;
    /// The subsurface scattering factor of the material, where 0.0 is no
    /// subsurface scattering and 1.0 is full subsurface scattering
    float subsurface = 0.0f;
    /// The subsurface color of the material
    Vector3f subsurfaceColor = Vector3f{1.0f, 0.2f, 0.2f};
    /// The subsurface intensity texture of the material
    Texture* subsurfaceTexture = nullptr;
    /// Normal texture of the material
    Texture* normalTexture = nullptr;
    /// Is the material transparent?
    bool transparent = false;
};

} // namespace ply