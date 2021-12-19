#include "Common.hlsli"

//정점 셰이더의 입력을 위한 구조체를 선언한다.
struct VS_CMPlayer_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
};

//정점 셰이더의 출력(픽셀 셰이더의 입력)을 위한 구조체를 선언한다.
struct VS_CMPlayer_OUTPUT
{
    float4 position : SV_POSITION;
    float4 velocityPosition : VELOCITY;
    float4 direction : DIRECTION;
    float3 position_w : POSITION;
    float3 normal_w : NORMAL;
};

VS_CMPlayer_OUTPUT VS_CMPlayer(VS_CMPlayer_INPUT input)
{
    VS_CMPlayer_OUTPUT output;
    output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxView), gmtxProj);
    output.position_w = mul(float4(input.position, 1.0f), gmtxWorld).xyz;
    output.normal_w = normalize(mul(float4(input.normal, 0.0f), gmtxWorld).xyz);
    
    float4 fOldPos = mul(mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxOldView), gmtxProj);

    float3 fDir = output.position.xyz - fOldPos.xyz;

    float a = dot(normalize(fDir), normalize(mul(float4(output.normal_w, 0.0f), gmtxView).xyz));

    if(a < 0.5f)
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

PS_OUTPUT PS_CMPlayer(VS_CMPlayer_OUTPUT input)
{
    PS_OUTPUT output;
    float4 cColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    float4 debugColor;
    int cascadedIndex = 2;
    
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
    
    cColor += material.AmbientLight * material.DiffuseAlbedo;
    
    float3 toEyeW = normalize(cameraPos - input.position_w);
    
    float3 shadowFactor = float3(1.0f, 1.0f, 1.0f);
    shadowFactor[0] = CalcShadowFactor(position_shadow, cascadedIndex);
    
    for (int i = 0; i < nLights; i++)
    {
        if (position_shadow.x < 0.0f || position_shadow.x > 1.0f || position_shadow.z < 0.0f || position_shadow.z > 1.0f || position_shadow.y < 0.0f || position_shadow.y > 1.0f)
            cColor += ComputeLighting(light[i], input.position_w, input.normal_w, toEyeW, 1.0f);
        else
            cColor += ComputeLighting(light[i], input.position_w, input.normal_w, toEyeW, shadowFactor[0]);
    }
    // Add in specular reflections.
    float3 r = reflect(-toEyeW, input.normal_w);
    float4 reflectionColor = { 1.0f, 1.0f, 1.0f, 0.0f };
    
    float3 fresnelFactor = SchlickFresnel(material.FresnelR0, input.normal_w, r);
    cColor.rgb += material.Shininess * fresnelFactor * reflectionColor.rgb;
    
    float3 fromCamera = normalize(input.position_w - cameraPos);
    float3 reflected = normalize(reflect(fromCamera, input.normal_w));
    float4 CubeTextureColor = gtxtCubeMap.Sample(gsamLinearWrap, reflected);
    
    cColor = saturate((CubeTextureColor * 0.5f) + (cColor * 0.5f));
    
    cColor.a = material.DiffuseAlbedo.a;
    
    // cColor *= debugColor;
    
    output.color = cColor;

    output.VelocityMap.xy = input.direction.xy;
    output.VelocityMap.z = 1.0f;
    output.VelocityMap.w = input.direction.z / input.direction.w;

    return (output);
}
