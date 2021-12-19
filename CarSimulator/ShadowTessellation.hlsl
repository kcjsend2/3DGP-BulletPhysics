#include "Common.hlsli"

//정점 셰이더의 입력을 위한 구조체를 선언한다.
struct VS_Terrain_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
};

//정점 셰이더의 출력(픽셀 셰이더의 입력)을 위한 구조체를 선언한다.
struct VS_Terrain_OUTPUT
{
    float4 position : POSITION0;
    float3 position_w : POSITION1;
    float3 normal : NORMAL;
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
};

struct DS_TERRAIN_TESSELLATION_OUTPUT
{
    float4 position_w : SV_POSITION;
    float3 normal : NORMAL;
    float4 tessellation : TEXCOORD;
};


VS_Terrain_OUTPUT VS_Terrain(VS_Terrain_INPUT input)
{
    VS_Terrain_OUTPUT output;
    output.position = float4(input.position, 1.0f);
    output.position_w = mul(float4(input.position, 1.0f), gmtxWorld).xyz;
    output.normal = normalize(mul(float4(input.normal, 0.0f), gmtxWorld).xyz);
    
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

    float3 position = CubicBezierSum5x5(patch, uB, vB);
    matrix mtxWorldViewProjection = mul(gmtxWorld, gmtxLightViewProj[nShadowIndex]);
    output.position_w = mul(float4(position, 1.0f), mtxWorldViewProjection);

    output.tessellation = float4(patchConstant.fTessEdges[0], patchConstant.fTessEdges[1], patchConstant.fTessEdges[2], patchConstant.fTessEdges[3]);
    
    output.normal = lerp(lerp(patch[0].normal, patch[4].normal, uv.x), lerp(patch[20].normal, patch[24].normal, uv.x), uv.y);
    
    return (output);
}

void PS_Terrain(DS_TERRAIN_TESSELLATION_OUTPUT input)
{
	// Fetch the material data.
    MATERIAL matData = material;
    float4 diffuseAlbedo = matData.DiffuseAlbedo;
}
