struct VSInput {
    uint vertexId : SV_VertexID;
};

struct PSInput {
    float4 pos : SV_POSITION;
};

void main(in  VSInput vs, out PSInput ps) {
    float4 pos[4];
    pos[0] = float4(-1.0, -1.0, 0.0, 1.0);
    pos[1] = float4(-1.0, +1.0, 0.0, 1.0);
    pos[2] = float4(+1.0, -1.0, 0.0, 1.0);
    pos[3] = float4(+1.0, +1.0, 0.0, 1.0);

    ps.pos = pos[vs.vertexId];
}