#include "camera.fxh"

// Vertex shader takes two inputs: vertex position and uv coordinates.
// By convention, Diligent Engine expects vertex shader inputs to be 
// labeled 'ATTRIBn', where n is the attribute number.
struct VSInput {
    float3 pos             : ATTRIB0;
    float4 lightLocation   : ATTRIB1;
    float3 lightColor      : ATTRIB2;
    float lightAttenuation : ATTRIB3;
};

struct PSInput { 
    float4 pos             : SV_POSITION; 
    float4 lightLocation   : LIGHT_LOCATION;
    float3 lightColor      : LIGHT_COLOR;
    float lightAttenuation : LIGHT_COEFFS;
};

// Note that if separate shader objects are not supported (this is only the case for old GLES3.0 devices), vertex
// shader output variable name must match exactly the name of the pixel shader input variable.
// If the variable has structure type (like in this example), the structure declarations must also be identical.
void main(in  uint    InstID : SV_InstanceID,
          in  VSInput VSIn,
          out PSInput PSIn) 
{
    float3 pos = VSIn.lightLocation.xyz + VSIn.pos.xyz * VSIn.lightLocation.w;
    PSIn.pos = g_projView * float4(pos, 1.0);
    PSIn.lightLocation = VSIn.lightLocation;
    PSIn.lightColor    = VSIn.lightColor;
    PSIn.lightAttenuation   = VSIn.lightAttenuation;
}