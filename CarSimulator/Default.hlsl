#include "Common.hlsli"

//���� ���̴��� �Է��� ���� ����ü�� �����Ѵ�.
struct VS_DEFAULT_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
};

//���� ���̴��� ���(�ȼ� ���̴��� �Է�)�� ���� ����ü�� �����Ѵ�.
struct VS_DEFAULT_OUTPUT
{
    float4 position : SV_POSITION;
    float3 position_w : POSITION;
    float3 normal : NORMAL;
};


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
    
    float3 shadowFactor = float3(1.0f, 1.0f, 1.0f);
    for (int i = 0; i < nLights; i++)
    {
        cColor += ComputeLighting(light[i], input.position_w, input.normal, normalize(cameraPos - input.position_w), shadowFactor);
    }
    
    return (cColor);
}
