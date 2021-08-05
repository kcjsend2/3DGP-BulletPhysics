#include "Common.hlsli"

//정점 셰이더의 입력을 위한 구조체를 선언한다.
struct VS_DEFAULT_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
};

//정점 셰이더의 출력(픽셀 셰이더의 입력)을 위한 구조체를 선언한다.
struct VS_DEFAULT_OUTPUT
{
    float4 position : SV_POSITION;
    float3 position_w : POSITION;
    float3 normal : NORMAL;
};

struct VS_INSTANCING_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
};

struct VS_INSTANCING_OUTPUT
{
    float4 position : SV_POSITION;
    float3 position_w : POSITION;
    float3 normal : NORMAL;
};


float CalcShadowFactor(float4 shadowPosH)
{
    // Complete projection by doing division by w.
    shadowPosH.xyz /= shadowPosH.w;

    // Depth in NDC space.
    float depth = shadowPosH.z;

    uint width, height, numMips;
    gShadowMap.GetDimensions(0, width, height, numMips);

    // Texel size.
    float dx = 1.0f / (float) width;

    float percentLit = 0.0f;
    const float2 offsets[9] =
    {
        float2(-dx, -dx), float2(0.0f, -dx), float2(dx, -dx),
        float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
        float2(-dx, +dx), float2(0.0f, +dx), float2(dx, +dx)
    };

    [unroll]
    for (int i = 0; i < 9; ++i)
    {
        percentLit += gShadowMap.SampleCmpLevelZero(gsamShadow, shadowPosH.xy + offsets[i], depth).r;
    }
    
    return percentLit / 9.0f;
}

VS_DEFAULT_OUTPUT VS_Default(VS_DEFAULT_INPUT input)
{
    VS_DEFAULT_OUTPUT output;
    output.position = mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxViewProj);
    output.position_w = mul(float4(input.position, 1.0f), gmtxWorld).xyz;
    output.normal = normalize(mul(float4(input.normal, 0.0f), gmtxWorld).xyz);
    
    return (output);
}

float4 PS_Default(VS_DEFAULT_OUTPUT input) : SV_TARGET
{
    float4 cColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    for (int i = 0; i < nLights; i++)
    {
        cColor += ComputeLight(light[i], input.position_w, input.normal, normalize(cameraPos - input.position_w));
    }
    
    return (cColor);
}

VS_INSTANCING_OUTPUT VS_Instancing(VS_INSTANCING_INPUT input, uint InstanceID : SV_InstanceID)
{
    VS_INSTANCING_OUTPUT output;
    output.position = mul(mul(float4(input.position, 1.0f), gGameObjectInfos[InstanceID].m_mtxGameObject), gmtxViewProj);    
    output.position_w = mul(float4(input.position, 1.0f), gGameObjectInfos[InstanceID].m_mtxGameObject).xyz;
    output.normal = normalize(mul(float4(input.normal, 0.0f), gGameObjectInfos[InstanceID].m_mtxGameObject).xyz);
    
    return (output);
}

float4 PS_Instancing(VS_INSTANCING_OUTPUT input) : SV_TARGET
{
    float4 cColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    for (int i = 0; i < nLights; i++)
    {
        cColor += ComputeLight(light[i], input.position_w, input.normal, normalize(cameraPos - input.position_w));
    }
    
    return (cColor);
}