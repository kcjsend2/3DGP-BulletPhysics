#include "Common.hlsli"

struct VS_BILLBOARD_INPUT
{
    float3 position_w : POSITION;
    float2 size : SIZE; // x : Height, y : Width
};

struct GS_BILLBOARD_INPUT
{
    float3 position_w : POSITION;
    float2 size : SIZE;
};

struct GS_BILLBOARD_OUTPUT
{
    uint p_id : SV_PrimitiveID;
    float2 uv : TEXCOORD;
    uint textureIndex : INDEX;
    float4 position : SV_POSITION;
};

GS_BILLBOARD_INPUT VS_BillBoard(VS_BILLBOARD_INPUT input)
{
    GS_BILLBOARD_INPUT output;
    
    output.position_w = input.position_w;
    output.size = input.size;

    return (output);
}

[maxvertexcount(4)]
void GS_BillBoard(point GS_BILLBOARD_INPUT input[1], uint p_id : SV_PrimitiveID, inout TriangleStream<GS_BILLBOARD_OUTPUT> triStream)
{
    float3 up = float3(0.0f, 1.0f, 0.0f);
    float3 look = cameraPos - input[0].position_w;
    
    look.y = 0.0f;
    look = normalize(look);
    
    float3 right = cross(up, look);
    
    float h_width = input[0].size.y * 0.5f;
    float h_height = input[0].size.x * 0.5f;
    
    float4 vertex[4];
    
    vertex[0] = float4(input[0].position_w + h_width * right - h_height * up, 1.0f);
    vertex[1] = float4(input[0].position_w + h_width * right + h_height * up, 1.0f);
    vertex[2] = float4(input[0].position_w - h_width * right - h_height * up, 1.0f);
    vertex[3] = float4(input[0].position_w - h_width * right + h_height * up, 1.0f);
    
    float fnx = float(nx);
    float fnxDivided = float(nxDivided);
    float fny = float(ny);
    float fnyDivided = float(nyDivided);
    
    float2 uv[4] =
    {
        float2(fnx / fnxDivided, (fny + 1) / fnyDivided),
        float2(fnx / fnxDivided, fny / fnyDivided),
        float2((fnx + 1) / fnxDivided, (fny + 1) / fnyDivided),
        float2((fnx + 1) / fnxDivided, fny / fnyDivided)

    };
    
    GS_BILLBOARD_OUTPUT output;
    [unroll]
    for (int i = 0; i < 4; i++)
    {
        vertex[i].y += h_height;
        output.position = mul(mul(vertex[i], gmtxView), gmtxProj);
        output.uv = uv[i];
        output.p_id = p_id;
        
        if (h_height >= 10.0f)
            output.textureIndex = 0;
        else
            output.textureIndex = 1;
        
        triStream.Append(output);
    }
}


float4 PS_BillBoard(GS_BILLBOARD_OUTPUT input) : SV_TARGET
{
    float4 cColor = gtxtExplosionBillBoard.Sample(gsamLinearClamp, input.uv);
    
    clip(cColor.a - 0.5f);
    
    return cColor;
}