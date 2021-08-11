#include "LightUtil.hlsli"

//ī�޶��� ������ ���� ��� ���۸� �����Ѵ�.
cbuffer cbCameraInfo : register(b1)
{
    matrix gmtxViewProj : packoffset(c0);
    float3 cameraPos : packoffset(c4);
};

cbuffer cbLightInfo : register(b2)
{
    int nLights : packoffset(c0);
}

cbuffer cbShadowInfo : register(b3)
{
    matrix gmtxShadowTransform : packoffset(c0);
    float3 ShadowCameraPos : packoffset(c4);
}

Texture2D gShadowMap : register(t0);

struct INSTANCED_GAMEOBJECT_INFO
{
    matrix m_mtxGameObject;
};
StructuredBuffer<INSTANCED_GAMEOBJECT_INFO> gGameObjectInfos : register(t0, space1);

StructuredBuffer<LIGHT_INFO> light : register(t1, space1);

SamplerState gsamPointWrap : register(s0);
SamplerState gsamPointClamp : register(s1);
SamplerState gsamLinearWrap : register(s2);
SamplerState gsamLinearClamp : register(s3);
SamplerState gsamAnisotropicWrap : register(s4);
SamplerState gsamAnisotropicClamp : register(s5);
SamplerComparisonState gsamShadow : register(s6);

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