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


VS_DEFAULT_OUTPUT VS_Default(VS_DEFAULT_INPUT input)
{
    VS_DEFAULT_OUTPUT output;
    output.position = mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxViewProj/*gmtxLightViewProj*/);
    output.position_w = mul(float4(input.position, 1.0f), gmtxWorld).xyz;
    output.normal = normalize(mul(float4(input.normal, 0.0f), gmtxWorld).xyz);
    
    return (output);
}

float4 PS_Default(VS_DEFAULT_OUTPUT input) : SV_TARGET
{
    float4 cColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    float4 debugColor;
    int cascadedIndex = 0;
    
    for (int i = 0; i < 3; ++i)
    {
        float4 Cascaded = mul(float4(input.position_w, 1.0f), gmtxCascadedViewProj[i]);
        float3 base = float3(0.0f, 0.0f, 0.0f);
        Cascaded = Cascaded / Cascaded.w;
        if (Cascaded.x > -1.0f && Cascaded.x < 1.0f && Cascaded.z > -1.0f && Cascaded.z < 1.0f && Cascaded.y > -1.0f && Cascaded.y < 1.0f)
        {
            cascadedIndex = i;
            break; 
        }
    }
    
    float4 position_shadow = mul(float4(input.position_w, 1.0f), gmtxShadowTransform[cascadedIndex]);
    
    cColor += material.AmbientLight * material.DiffuseAlbedo;
    
    float3 toEyeW = normalize(cameraPos - input.position_w);
    
    float3 shadowFactor = float3(1.0f, 1.0f, 1.0f);
    shadowFactor[0] = CalcShadowFactor(position_shadow, cascadedIndex);
    
    for (int i = 0; i < nLights; i++)
    {
        if (position_shadow.x < 0.0f || position_shadow.x > 1.0f || position_shadow.z < 0.0f || position_shadow.z > 1.0f || position_shadow.y < 0.0f || position_shadow.y > 1.0f)
            cColor += ComputeLighting(light[i], input.position_w, input.normal, toEyeW, 1.0f);
        else
            cColor += ComputeLighting(light[i], input.position_w, input.normal, toEyeW, shadowFactor[0]);
    }
    // Add in specular reflections.
    
    float3 r = reflect(-toEyeW, input.normal);
    float4 reflectionColor = { 1.0f, 1.0f, 1.0f, 0.0f };
    
    float3 fresnelFactor = SchlickFresnel(material.FresnelR0, input.normal, r);
    cColor.rgb += material.Shininess * fresnelFactor * reflectionColor.rgb;
	
    // Common convention to take alpha from diffuse albedo.
    
    cColor.a = material.DiffuseAlbedo.a;
    
    // cColor *= debugColor;
    
    return (cColor);
}
