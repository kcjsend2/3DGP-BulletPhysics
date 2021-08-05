#include "LightUtil.hlsli"

//카메라의 정보를 위한 상수 버퍼를 선언한다.
cbuffer cbCameraInfo : register(b1)
{
    matrix gmtxViewProj : packoffset(c0);
    float3 cameraPos : packoffset(c4);
};

cbuffer cbLightInfo : register(b2)
{
    int nLights : packoffset(c0);
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
