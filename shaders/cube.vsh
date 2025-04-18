#include "camera.fxh"

cbuffer Constants
{
    float4x4 g_Rotation;
};

struct VSInput
{
    // Vertex attributes
    float3 Pos      : ATTRIB0; 
    float2 UV       : ATTRIB1;
    float3 Normal   : ATTRIB2;

    // Instance attributes
    float4 MtrxRow0 : ATTRIB3;
    float4 MtrxRow1 : ATTRIB4;
    float4 MtrxRow2 : ATTRIB5;
    float4 MtrxRow3 : ATTRIB6;
    float TexArrInd : ATTRIB7;
};

struct PSInput 
{ 
    float4 Pos : SV_POSITION; 
    float2 UV  : TEX_COORD;
    float3 Normal  : NORMAL; 
    float TexIndex : TEX_ARRAY_INDEX;
};

// Note that if separate shader objects are not supported (this is only the case for old GLES3.0 devices), vertex
// shader output variable name must match exactly the name of the pixel shader input variable.
// If the variable has structure type (like in this example), the structure declarations must also be identical.
void main(in  VSInput VSIn,
          out PSInput PSIn) 
{
    // HLSL matrices are row-major while GLSL matrices are column-major. We will
    // use convenience function MatrixFromRows() appropriately defined by the engine
    float4x4 InstanceMatr = MatrixFromRows(VSIn.MtrxRow0, VSIn.MtrxRow1, VSIn.MtrxRow2, VSIn.MtrxRow3);
    float4x4 worldTransform = InstanceMatr * g_Rotation;
    // Apply rotation
    float4 TransformedPos = worldTransform * float4(VSIn.Pos, 1.0);
    // Apply view-projection matrix
    PSIn.Pos = g_projView * TransformedPos;
    PSIn.UV  = VSIn.UV;
    PSIn.Normal = normalize((worldTransform * float4(VSIn.Normal, 0.0)).xyz); // Transform normal to world space and normalize it
    PSIn.TexIndex = VSIn.TexArrInd;
}