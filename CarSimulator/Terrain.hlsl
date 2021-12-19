#include "Common.hlsli"

//정점 셰이더의 입력을 위한 구조체를 선언한다.
struct VS_Terrain_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv0 : TEXCOORD0;
    float2 uv1 : TEXCOORD1;
};

//정점 셰이더의 출력(픽셀 셰이더의 입력)을 위한 구조체를 선언한다.
struct VS_Terrain_OUTPUT
{
    float4 position : POSITION0;
    float3 position_w : POSITION1;
    float3 normal : NORMAL;
    float2 uv0 : TEXCOORD0;
    float2 uv1 : TEXCOORD1;
};

struct HS_TERRAIN_TESSELLATION_CONSTANT
{
    float fTessEdges[4] : SV_TessFactor;
    float fTessInsides[2] : SV_InsideTessFactor;
};

struct HS_TERRAIN_TESSELLATION_OUTPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv0 : TEXCOORD0;
    float2 uv1 : TEXCOORD1;
};

struct DS_TERRAIN_TESSELLATION_OUTPUT
{
    float4 position : SV_POSITION;
    float4 position_w : POSITION;
    float3 normal : NORMAL;
    float2 uv0 : TEXCOORD0;
    float2 uv1 : TEXCOORD1;
    float4 tessellation : TEXCOORD2;
    float4 velocityPosition :VELOCITY;
    float4 direction : DIRECTION;
};


VS_Terrain_OUTPUT VS_Terrain(VS_Terrain_INPUT input)
{
    VS_Terrain_OUTPUT output;
    output.position = float4(input.position, 1.0f);
    output.position_w = mul(float4(input.position, 1.0f), gmtxWorld).xyz;
    output.normal = normalize(mul(float4(input.normal, 0.0f), gmtxWorld).xyz);
    output.uv0 = input.uv0;
    output.uv1 = input.uv1;
    
    float4 position_wvp = mul(mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxView), gmtxProj);
    
    return (output);
}


[domain("quad")]
//[partitioning("fractional_even")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(25)]
[patchconstantfunc("HSTerrainTessellationConstant")]
[maxtessfactor(64.0f)]
HS_TERRAIN_TESSELLATION_OUTPUT HSTerrainTessellation(InputPatch<VS_Terrain_OUTPUT, 25> input, uint i : SV_OutputControlPointID)
{
    HS_TERRAIN_TESSELLATION_OUTPUT output;

    output.position = input[i].position.xyz;
    output.uv0 = input[i].uv0;
    output.uv1 = input[i].uv1;
    output.normal = input[i].normal;

    return (output);
}

float CalculateTessFactor(float3 f3Position)
{
    float fDistToCamera = distance(f3Position, cameraPos);
    float s = saturate((fDistToCamera - 10.0f) / (500.0f - 10.0f));

    return (lerp(64.0f, 1.0f, s));
	//	return(pow(2, lerp(20.0f, 4.0f, s)));
}

HS_TERRAIN_TESSELLATION_CONSTANT HSTerrainTessellationConstant(InputPatch<VS_Terrain_OUTPUT, 25> input)
{
    HS_TERRAIN_TESSELLATION_CONSTANT output;
    
    float3 e0 = 0.5f * (input[0].position_w + input[4].position_w);
    float3 e1 = 0.5f * (input[0].position_w + input[20].position_w);
    float3 e2 = 0.5f * (input[4].position_w + input[24].position_w);
    float3 e3 = 0.5f * (input[20].position_w + input[24].position_w);

    output.fTessEdges[0] = CalculateTessFactor(e0);
    output.fTessEdges[1] = CalculateTessFactor(e1);
    output.fTessEdges[2] = CalculateTessFactor(e2);
    output.fTessEdges[3] = CalculateTessFactor(e3);

    float3 f3Sum = float3(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < 25; i++)
        f3Sum += input[i].position_w;
    
    float3 f3Center = f3Sum / 25.0f;
    output.fTessInsides[0] = output.fTessInsides[1] = CalculateTessFactor(f3Center);

    return (output);
}

void BernsteinCoeffcient5x5(float t, out float fBernstein[5])
{
    float tInv = 1.0f - t;
    fBernstein[0] = tInv * tInv * tInv * tInv;
    fBernstein[1] = 4.0f * t * tInv * tInv * tInv;
    fBernstein[2] = 6.0f * t * t * tInv * tInv;
    fBernstein[3] = 4.0f * t * t * t * tInv;
    fBernstein[4] = t * t * t * t;
}

float3 CubicBezierSum5x5(OutputPatch<HS_TERRAIN_TESSELLATION_OUTPUT, 25> patch, float uB[5], float vB[5])
{
    float3 f3Sum = float3(0.0f, 0.0f, 0.0f);
    f3Sum = vB[0] * (uB[0] * patch[0].position + uB[1] * patch[1].position + uB[2] * patch[2].position + uB[3] * patch[3].position + uB[4] * patch[4].position);
    f3Sum += vB[1] * (uB[0] * patch[5].position + uB[1] * patch[6].position + uB[2] * patch[7].position + uB[3] * patch[8].position + uB[4] * patch[9].position);
    f3Sum += vB[2] * (uB[0] * patch[10].position + uB[1] * patch[11].position + uB[2] * patch[12].position + uB[3] * patch[13].position + uB[4] * patch[14].position);
    f3Sum += vB[3] * (uB[0] * patch[15].position + uB[1] * patch[16].position + uB[2] * patch[17].position + uB[3] * patch[18].position + uB[4] * patch[19].position);
    f3Sum += vB[4] * (uB[0] * patch[20].position + uB[1] * patch[21].position + uB[2] * patch[22].position + uB[3] * patch[23].position + uB[4] * patch[24].position);

    return (f3Sum);
}

[domain("quad")]
DS_TERRAIN_TESSELLATION_OUTPUT DSTerrainTessellation(HS_TERRAIN_TESSELLATION_CONSTANT patchConstant, float2 uv : SV_DomainLocation, OutputPatch<HS_TERRAIN_TESSELLATION_OUTPUT, 25> patch)
{
    DS_TERRAIN_TESSELLATION_OUTPUT output = (DS_TERRAIN_TESSELLATION_OUTPUT) 0;

    float uB[5], vB[5];
    BernsteinCoeffcient5x5(uv.x, uB);
    BernsteinCoeffcient5x5(uv.y, vB);
    
    output.uv0 = lerp(lerp(patch[0].uv0, patch[4].uv0, uv.x), lerp(patch[20].uv0, patch[24].uv0, uv.x), uv.y);
    output.uv1 = lerp(lerp(patch[0].uv1, patch[4].uv1, uv.x), lerp(patch[20].uv1, patch[24].uv1, uv.x), uv.y);

    float3 position = CubicBezierSum5x5(patch, uB, vB);
    output.position_w = mul(float4(position, 1.0f), gmtxWorld);
    output.position = mul(mul(output.position_w, gmtxView), gmtxProj);
    output.tessellation = float4(patchConstant.fTessEdges[0], patchConstant.fTessEdges[1], patchConstant.fTessEdges[2], patchConstant.fTessEdges[3]);
    
    output.normal = lerp(lerp(patch[0].normal, patch[4].normal, uv.x), lerp(patch[20].normal, patch[24].normal, uv.x), uv.y);

    float4 fOldPos = mul(mul(mul(float4(position, 1.0f), gmtxWorld), gmtxOldView), gmtxProj);

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


PS_OUTPUT PS_Terrain(DS_TERRAIN_TESSELLATION_OUTPUT input)
{
    PS_OUTPUT output;
    float4 cColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 debugColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    int cascadedIndex = 2;
    
    for (int i = 0; i < 3; ++i)
    {
        float4 Cascaded = mul(input.position_w, gmtxLightViewProj[i]);
        Cascaded = Cascaded / Cascaded.w;
        if (Cascaded.x > -1.0f && Cascaded.x < 1.0f && Cascaded.z > -1.0f && Cascaded.z < 1.0f && Cascaded.y > -1.0f && Cascaded.y < 1.0f)
        {
            cascadedIndex = i;
            break;
        }
    }
    
    float4 position_shadow = mul(input.position_w, gmtxShadowTransform[cascadedIndex]);
    
    cColor += material.AmbientLight * material.DiffuseAlbedo;
    
    float3 toEyeW = normalize(cameraPos - input.position_w.xyz);
    
    float3 shadowFactor = float3(1.0f, 1.0f, 1.0f);
    shadowFactor[0] = CalcShadowFactor(position_shadow, cascadedIndex);
    
    for (int i = 0; i < nLights; i++)
    {
        if (position_shadow.x < 0.0f || position_shadow.x > 1.0f || position_shadow.z < 0.0f || position_shadow.z > 1.0f || position_shadow.y < 0.0f || position_shadow.y > 1.0f)
            cColor += ComputeLighting(light[i], input.position_w.xyz, input.normal, toEyeW, 1.0f);
        else
        {
            cColor += ComputeLighting(light[i], input.position_w.xyz, input.normal, toEyeW, shadowFactor[0]);
            debugColor = float4(1.0f, 0.0f, 0.0f, 0.0f);
        }
    }
    // Add in specular reflections.
    
    float3 r = reflect(-toEyeW, input.normal);
    float4 reflectionColor = { 1.0f, 1.0f, 1.0f, 0.0f };
    
    float3 fresnelFactor = SchlickFresnel(material.FresnelR0, input.normal, r);
    cColor.rgb += material.Shininess * fresnelFactor * reflectionColor.rgb;
	
    // Common convention to take alpha from diffuse albedo.
    
    cColor.a = material.DiffuseAlbedo.a;
    
    // cColor *= debugColor;
    
    float4 cBaseTexColor = gtxtTerrain[0].Sample(gsamAnisotropicWrap, input.uv0);
    float4 cDetailTexColor = gtxtTerrain[1].Sample(gsamAnisotropicWrap, input.uv1);
    float4 cRoadTexColor = gtxtTerrain[2].Sample(gsamAnisotropicWrap, input.uv0);
    
    if (cRoadTexColor.a > 0.0f)
    {
        cColor *= saturate((cBaseTexColor * (0.5f - cRoadTexColor.a * 0.5f)) + (cDetailTexColor * (0.5f - cRoadTexColor.a * 0.5f)) + (cRoadTexColor * cRoadTexColor.a));
    }
    else
    {
        cColor *= saturate((cBaseTexColor * 0.5f) + (cDetailTexColor * 0.5f));
    }

    output.color = cColor;

    output.VelocityMap.xy = input.direction.xy;
    output.VelocityMap.z = 1.0f;
    output.VelocityMap.w = input.direction.z / input.direction.w;

    return (output);
}
