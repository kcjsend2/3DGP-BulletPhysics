#include "Common.hlsli"

struct VS_INSTANCING_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct VS_INSTANCING_OUTPUT
{
    float4 position : SV_POSITION;
    float3 position_w : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    int InstanceID : SV_InstanceID;
};

VS_INSTANCING_OUTPUT VS_Instancing(VS_INSTANCING_INPUT input, uint InstanceID : SV_InstanceID)
{
    VS_INSTANCING_OUTPUT output;
    output.position = mul(mul(float4(input.position, 1.0f), gGameObjectInfos[InstanceID].m_mtxGameObject), gmtxViewProj);
    output.position_w = mul(float4(input.position, 1.0f), gGameObjectInfos[InstanceID].m_mtxGameObject).xyz;
    output.normal = normalize(mul(float4(input.normal, 0.0f), gGameObjectInfos[InstanceID].m_mtxGameObject).xyz);
    output.uv = input.uv;
    output.InstanceID = InstanceID;
    
    return (output);
}

float4 PS_Instancing(VS_INSTANCING_OUTPUT input) : SV_TARGET
{
    float4 cColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float3 shadowFactor = float3(1.0f, 1.0f, 1.0f);
    
    int cascadedIndex = 0;
    for (int i = 0; i < 3; ++i)
    {
        float4 Cascaded = mul(float4(input.position_w, 1.0f), gmtxLightViewProj[i]);
        Cascaded = Cascaded / Cascaded.w;
        if (Cascaded.x > -1.0f && Cascaded.x < 1.0f && Cascaded.z > -1.0f && Cascaded.z < 1.0f && Cascaded.y > -1.0f && Cascaded.y < 1.0f)
        {
            cascadedIndex = i;
            break;
        }
    }
    
    float4 position_shadow = mul(float4(input.position_w, 1.0f), gmtxShadowTransform[cascadedIndex]);
    shadowFactor[0] = CalcShadowFactor(position_shadow, cascadedIndex);
    
    float3 toEyeW = normalize(cameraPos - input.position_w);
    
    cColor += material.AmbientLight * material.DiffuseAlbedo;
    
    for (int i = 0; i < nLights; i++)
    {
        cColor += ComputeLighting(light[i], input.position_w, input.normal, normalize(cameraPos - input.position_w), shadowFactor[0]);
    }
    
    float3 r = reflect(-toEyeW, input.normal);
    float4 reflectionColor = { 1.0f, 1.0f, 1.0f, 0.0f };
    
    float3 fresnelFactor = SchlickFresnel(material.FresnelR0, input.normal, r);
    cColor.rgb += material.Shininess * fresnelFactor * reflectionColor.rgb;
	
    // Common convention to take alpha from diffuse albedo.
    
    cColor.a = material.DiffuseAlbedo.a;
    cColor *= gTextureMaps[gGameObjectInfos[input.InstanceID].m_nTextrueIndex].Sample(gsamLinearWrap, input.uv);
    
    return (cColor);
}