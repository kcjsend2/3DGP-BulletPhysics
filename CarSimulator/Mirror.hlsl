#include "Common.hlsli"

struct VS_TEXTURED_INPUT
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_TEXTURED_OUTPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

VS_TEXTURED_OUTPUT VS_Mirror(VS_TEXTURED_INPUT input)
{
    VS_TEXTURED_OUTPUT output;

    output.position = mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxViewProj);

    output.uv = input.uv;

    return (output);
}


float4 PS_Mirror(VS_TEXTURED_OUTPUT input) : SV_TARGET
{
    float4 cColor = gtxtMirror.Sample(gsamLinearClamp, input.uv);

    return (cColor);
}
