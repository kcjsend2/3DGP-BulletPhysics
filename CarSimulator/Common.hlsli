#include "LightUtil.hlsli"

//카메라의 정보를 위한 상수 버퍼를 선언한다.

cbuffer cbCommonInfo : register(b1)
{
    matrix gmtxProj : packoffset(c0);
    float3 cameraPos : packoffset(c4);
    int nLights : packoffset(c8.x);
    int nShadowIndex : packoffset(c8.y);
    int nSkyboxTextureIndex : packoffset(c8.z);
    float fTimeElapsed : packoffset(c8.w);
}

cbuffer cbShadowInfo : register(b2)
{
    matrix gmtxShadowTransform[3];
    matrix gmtxLightViewProj[3];
    float3 ShadowCameraPos;
}

cbuffer cbMotionBlurInfo : register(b3)
{
    matrix gmtxOldView;
    matrix gmtxView;
}

cbuffer cbParticleInfo : register(b4)
{
    float3 fRandomVelocity : packoffset(c0);
}


struct PS_OUTPUT
{
    float4 color : SV_TARGET0;
    float4 VelocityMap : SV_TARGET1;
};

Texture2D gShadowMap[3] : register(t0);
Texture2D gTextureMaps[4] : register(t3);
Texture2D gtxtTerrain[3] : register(t7);
Texture2D gtxtSkybox[6] : register(t10);
Texture2DArray gtxtTreeBillBoard[2] : register(t16);
Texture2D gtxtExplosionBillBoard : register(t18);
TextureCube gtxtCubeMap : register(t19);
Texture2D gtxtParticle : register(t20);

struct INSTANCED_GAMEOBJECT_INFO
{
    matrix m_mtxGameObject;
    int m_nTextrueIndex;
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

float CalcShadowFactor(float4 shadowPosH, uint shadowIndex)
{
    // Complete projection by doing division by w.
    shadowPosH.xyz /= shadowPosH.w;

    // Depth in NDC space.
    float depth = shadowPosH.z;

    uint width, height, numMips;
    gShadowMap[shadowIndex].GetDimensions(0, width, height, numMips);

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
        percentLit += gShadowMap[shadowIndex].SampleCmpLevelZero(gsamShadow, shadowPosH.xy + offsets[i], depth).r;
    }
    
    return percentLit / 9.0f;
}