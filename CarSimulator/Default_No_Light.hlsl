#include "Common.hlsli"

//���� ���̴��� �Է��� ���� ����ü�� �����Ѵ�.
struct VS_DEFAULT_INPUT
{
    float3 position : POSITION;
};

//���� ���̴��� ���(�ȼ� ���̴��� �Է�)�� ���� ����ü�� �����Ѵ�.
struct VS_DEFAULT_OUTPUT
{
    float4 position : SV_POSITION;
};


VS_DEFAULT_OUTPUT VS_Default(VS_DEFAULT_INPUT input)
{
    VS_DEFAULT_OUTPUT output;
    output.position = mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxViewProj /*gmtxLightViewProj*/);
    
    return (output);
}

float4 PS_Default(VS_DEFAULT_OUTPUT input) : SV_TARGET
{
    float4 cColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    cColor = material.AmbientLight * material.DiffuseAlbedo;

    return (cColor);
}
