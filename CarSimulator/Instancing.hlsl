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
    float4 velocityPosition : VELOCITY;
    float4 direction : DIRECTION;
    int InstanceID : SV_InstanceID;
};

VS_INSTANCING_OUTPUT VS_Instancing(VS_INSTANCING_INPUT input, uint InstanceID : SV_InstanceID)
{
    VS_INSTANCING_OUTPUT output;
    output.position = mul(mul(mul(float4(input.position, 1.0f), gGameObjectInfos[InstanceID].m_mtxGameObject), gmtxView), gmtxProj);
    output.position_w = mul(float4(input.position, 1.0f), gGameObjectInfos[InstanceID].m_mtxGameObject).xyz;
    output.normal = normalize(mul(float4(input.normal, 0.0f), gGameObjectInfos[InstanceID].m_mtxGameObject).xyz);
    output.uv = input.uv;
    output.InstanceID = InstanceID;
    
    float4 fOldPos = mul(mul(mul(float4(input.position, 1.0f), gGameObjectInfos[InstanceID].m_mtxGameObject), gmtxOldView), gmtxProj);

    float3 fDir = output.position.xyz - fOldPos.xyz;

    float a = dot(normalize(fDir), normalize(mul(float4(output.normal, 0.0f), gmtxView).xyz));

    if (a < 0.5f)
        output.velocityPosition = fOldPos;
    else
        output.velocityPosition = output.position;

    float2 velocity = (output.position.xy / output.position.w) - (fOldPos.xy / fOldPos.w);
    output.direction.xy = velocity * 0.5f;
    output.direction.y *= -1.0f;
    output.direction.z = output.velocityPosition.z;
    output.direction.w = output.velocityPosition.w;

    return (output);
}

PS_OUTPUT PS_Instancing(VS_INSTANCING_OUTPUT input)
{
    PS_OUTPUT output;
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
    
    output.color = cColor;

    output.VelocityMap.xy = input.direction.xy;
    output.VelocityMap.z = 1.0f;
    output.VelocityMap.w = input.direction.z / input.direction.w;

    return (output);
}