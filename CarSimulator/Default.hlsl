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
    float4 position_shadow : POSITION0;
    float3 position_w : POSITION1;
    float3 normal : NORMAL;
    float3 color : COLOR;
};


VS_DEFAULT_OUTPUT VS_Default(VS_DEFAULT_INPUT input)
{
    VS_DEFAULT_OUTPUT output;
    output.position = mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxViewProj);
    output.position_w = mul(float4(input.position, 1.0f), gmtxWorld).xyz;
    output.normal = normalize(mul(float4(input.normal, 0.0f), gmtxWorld).xyz);
    output.position_shadow = mul(float4(output.position_w, 1.0f), gmtxShadowTransform);
    
    for (int i = 0; i < 3; ++i)
    {
        float4 Cascaded = mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxCascadedViewProj[i]);
        float3 base = float3(0.0f, 0.0f, 0.0f);
        if (Cascaded.x / Cascaded.w > -1.0f && Cascaded.x / Cascaded.w < 1.0f && Cascaded.z / Cascaded.w > -1.0f && Cascaded.z / Cascaded.w < 1.0f && Cascaded.y / Cascaded.w > -1.0f && Cascaded.y / Cascaded.w < 1.0f)
        {
            if (i == 0)
            {
                output.color = float4(1.0f, 0.0f, 0.0f, 0.0f);
            }
            if (i == 1)
            {
                output.color = float4(0.0f, 1.0f, 0.0f, 0.0f);
            }
            if (i == 2)
            {
                output.color = float4(0.0f, 0.0f, 1.0f, 0.0f);
            }
        }
    }
    
    return (output);
}

float4 PS_Default(VS_DEFAULT_OUTPUT input) : SV_TARGET
{
    float4 cColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    cColor += material.AmbientLight * material.DiffuseAlbedo;
    
    float3 toEyeW = normalize(cameraPos - input.position_w);
    
    float3 shadowFactor = float3(1.0f, 1.0f, 1.0f);
    shadowFactor[0] = CalcShadowFactor(input.position_shadow);
    
    for (int i = 0; i < nLights; i++)
    {
        if (input.position_shadow.x < 0.0f || input.position_shadow.x > 1.0f || input.position_shadow.z < 0.0f || input.position_shadow.z > 1.0f || input.position_shadow.y < 0.0f || input.position_shadow.y > 1.0f)
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
    
    cColor = float4(input.color, 0.0f);
    
    return (cColor);
}
