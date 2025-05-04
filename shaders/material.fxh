struct Material {
    /// The color of the material
    float3 albedoColor;
    /// The metallic factor of the material, where 0.0 is non-metallic and 1.0
    /// is metallic
    float metallic;

    /// The emission color of the material
    float3 emissionColor;
    /// The roughness factor of the material, where 0.0 is smooth and 1.0 is
    /// rough
    float roughness;

    /// The rim light color of the material
    float3 rimColor;
    /// The specular intensity of the material, where 0.0 is no specular and
    /// 1.0 is full specular
    float specularIntensity;

    /// The subsurface color of the material
    float3 subsurfaceColor;
    /// The emission intensity of the material, where 0.0 is no emission and
    /// 1.0 is full emission
    float emissionIntensity;

    /// The rim light intensity of the material, where 0.0 is no rim light and
    /// 1.0 is full rim light
    float rimIntensity;
    /// The rim light power of the material, where higher values make the rim
    /// light more focused on the edges
    float rimPower;
    /// The subsurface scattering factor of the material, where 0.0 is no
    /// subsurface scattering and 1.0 is full subsurface scattering
    float subsurface;
    /// Is the material transparent?
    bool transparent;
};

cbuffer Materials {
    Material g_materials[MAX_NUM_MATERIALS];
};

/// The albedo texture of the material
Texture2DArray g_albedoTexture;
/// The metallic texture of the material
Texture2DArray g_metallicTexture;
/// The roughness texture of the material
Texture2DArray g_roughnessTexture;
/// The emission intensity texture of the material
Texture2DArray g_emissionTexture;
/// Rim light intensity texture of the material
Texture2DArray g_rimTexture;
/// The subsurface intensity texture of the material
Texture2DArray g_subsurfaceTexture;
/// Normal texture of the material
Texture2DArray g_normalTexture;

#ifndef GLSL
SamplerState g_albedoTexture_sampler;
SamplerState g_metallicTexture_sampler;
SamplerState g_roughnessTexture_sampler;
SamplerState g_emissionTexture_sampler;
SamplerState g_rimTexture_sampler;
SamplerState g_subsurfaceTexture_sampler;
SamplerState g_normalTexture_sampler;
#endif

Material getMaterial(float materialId) {
    return g_materials[int(materialId * MAX_NUM_MATERIALS)];
}