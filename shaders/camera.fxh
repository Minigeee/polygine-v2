cbuffer Camera {
    float4x4 g_projView;
    float4x4 g_invProjView;
    float4 g_cameraPos;
    float4 g_viewport; // w, h, 1 / w, 1 / h
};