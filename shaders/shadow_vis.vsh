struct ShadowMapVisPSInput {
    float4 pos : SV_POSITION;
    float2 uv  : TEX_COORD;
};

void main(in uint vertId : SV_VertexID, out ShadowMapVisPSInput psIn) {
    float2 pos[4];
    pos[0] = float2(-1.0, -1.0);
    pos[1] = float2(-1.0, +1.0);
    pos[2] = float2(+1.0, -1.0);
    pos[3] = float2(+1.0, +1.0);

    float2 center = float2(-0.6, -0.6);
    float2 size = float2(0.35, 0.35);

    psIn.pos = float4(center + size * pos[vertId], 0.0, 1.0);
    psIn.uv  = pos[vertId].xy * F3NDC_XYZ_TO_UVD_SCALE.xy + float2(0.5, 0.5);
}