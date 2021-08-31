#include "Common.hlsli"

struct VertexIn
{
    float3 PosL : POSITION;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
};

VertexOut VS(VertexIn vin, uint InstanceID : SV_InstanceID)
{
    VertexOut vout = (VertexOut) 0.0f;
	
    // Transform to world space.
    float4 posW = mul(float4(vin.PosL, 1.0f), gGameObjectInfos[InstanceID].m_mtxGameObject);

    // Transform to homogeneous clip space.
    vout.PosH = mul(posW, gmtxLightViewProj);
    
    return vout;
}

// This is only used for alpha cut out geometry, so that shadows 
// show up correctly.  Geometry that does not need to sample a
// texture can use a NULL pixel shader for depth pass.
void PS(VertexOut pin)
{
	// Fetch the material data.
    MATERIAL matData = material;
    float4 diffuseAlbedo = matData.DiffuseAlbedo;
}

