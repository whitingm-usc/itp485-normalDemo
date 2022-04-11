#define SHOW_NORMALS 1
#define TANGENT_SPACE 0
#define ONE_LIGHT 1
#define COLOR 0
#define SPECULAR 1

#include "Constants.hlsl"

struct VIn
{
    float3 position : POSITION0;
    float3 normal : NORMAL0;
    float3 tangent : TANGENT0;
    float2 uv : TEXCOORD0;
};

struct VOut
{
    float4 position : SV_POSITION;
    float4 worldPos : POSITION1;
    float3 normal : NORMAL0;
    float3 tangent : TANGENT0;
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
    float4 tangent = float4(vIn.tangent, 0.0);
    output.tangent = mul(tangent, c_modelToWorld).xyz;
    output.uv = vIn.uv;

    return output;
}

float4 PS(VOut pIn) : SV_TARGET
{
    float4 diffuseTex = DiffuseTexture.Sample(DefaultSampler, pIn.uv);

    // normal map
    float3 n = normalize(pIn.normal);
    float3 t = normalize(pIn.tangent);
    float3 b = normalize(cross(n, t));
    float3x3 TBN = float3x3(t, b, n);
    n = NormalTexture.Sample(DefaultSampler, pIn.uv);

    // bias the normal
    n = 2.0f * n - 1.0f;
#if TANGENT_SPACE
    // transform to world space
    n = mul(n, TBN);
#endif
#if SHOW_NORMALS
	n = 0.5f * n + 0.5f;
	return float4(n, 1.0);
#endif

    // do the lighting
    float3 lightColor = c_ambient;
    float3 v = normalize(c_cameraPosition - pIn.worldPos);
#if ONE_LIGHT
	int i = 0;
#else
	for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
#endif
    {
        if (c_pointLight[i].isEnabled)
        {
            float3 l = c_pointLight[i].position - pIn.worldPos.xyz;
            float dist = length(l);
            if (dist > 0.0)
            {
                l = l / dist;
                float falloff = smoothstep(c_pointLight[i].outerRadius, c_pointLight[i].innerRadius, dist);
#if COLOR
				float3 d = falloff * c_pointLight[i].diffuseColor * max(0.0, dot(l, n));
#else
				float3 d = falloff * max(0.0, dot(l, n));
#endif
                lightColor += d;

#if SPECULAR
                float3 r = -reflect(l, n);
#if COLOR
                float3 s = falloff * c_pointLight[i].specularColor * pow(max(0.0, dot(r, v)), c_pointLight[i].specularPower);
#else
				float3 s = falloff * pow(max(0.0, dot(r, v)), c_pointLight[i].specularPower);
#endif
                lightColor += s;
#endif
            }
        }
    }

    // combine the final lighting with the vertex color and the texture color 
    float4 finalColor = diffuseTex * float4(lightColor, 1.0);
    return finalColor;
}
