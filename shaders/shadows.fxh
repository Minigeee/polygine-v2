
cbuffer Shadows {
    float4x4 g_lightProjView;
    float g_shadowMapSize;
};

Texture2D g_shadowMap;

#ifndef GLSL
SamplerState g_shadowMap_sampler;
#endif