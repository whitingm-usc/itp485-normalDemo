#include "Constants.hlsl"

struct VIn
{
    float3 position : POSITION0;
    float3 normal : NORMAL0;
    float2 uv : TEXCOORD0;
};

struct VOut
{
    float4 position : SV_POSITION;
    float4 worldPos : POSITION1;
    float3 normal : NORMAL0;
    float2 uv : TEXCOORD0;
};

VOut VS(VIn vIn)
{
    VOut output;

    float4 pos = float4(vIn.position, 1.0);
    output.worldPos = mul(pos, c_modelToWorld);
    output.position = mul(output.worldPos, c_viewProj);
    float4 norm = float4(vIn.normal, 0.0);
    output.normal = mul(norm, c_modelToWorld).xyz;
    output.uv = vIn.uv;

    return output;
}

float4 PS(VOut pIn) : SV_TARGET
{
    float4 diffuseTex = DiffuseTexture.Sample(DefaultSampler, pIn.uv);
    return diffuseTex;
}
