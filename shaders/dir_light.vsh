struct VSInput {
    uint vertexId : SV_VertexID;
    float3 lightDirection : ATTRIB0;
    float3 lightColor     : ATTRIB1;
};

struct PSInput {
    float4 pos : SV_POSITION;
    float3 lightDirection : LIGHT_DIRECTION;
    float3 lightColor     : LIGHT_COLOR;
};

void main(in  VSInput vs, out PSInput ps) {
    float4 pos[4];
    pos[0] = float4(-1.0, -1.0, 0.0, 1.0);
    pos[1] = float4(-1.0, +1.0, 0.0, 1.0);
    pos[2] = float4(+1.0, -1.0, 0.0, 1.0);
    pos[3] = float4(+1.0, +1.0, 0.0, 1.0);

    ps.pos = pos[vs.vertexId];
    ps.lightDirection = vs.lightDirection;
    ps.lightColor = vs.lightColor;
}