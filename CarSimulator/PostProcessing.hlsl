///////////////////////////////////////////////////////////////////////////////
//
RWTexture2D<float4> gtxtRWInput : register(u0);
RWTexture2D<float4> gtxtRWOutput : register(u1);

static float3 gf3ToLuminance = float3(0.3f, 0.59f, 0.11f);

#define _WITH_SOBEL_EDGE

#define _WITH_GROUPSHARED_MEMORY

groupshared float4 gf4GroupSharedCache[32 + 2][30 + 2];

void GausianBlur(int3 n3GroupThreadID : SV_GroupThreadID, int3 n3DispatchThreadID : SV_DispatchThreadID)
{
    float3 f3HorizontalEdge = (-0.3f * gf4GroupSharedCache[n3GroupThreadID.x][n3GroupThreadID.y + 1].rgb) + (0.3f * gf4GroupSharedCache[n3GroupThreadID.x + 1][n3GroupThreadID.y + 1].rgb) + (-0.3f * gf4GroupSharedCache[n3GroupThreadID.x + 2][n3GroupThreadID.y + 1].rgb);
    float3 f3VerticalEdge = (-0.3f * gf4GroupSharedCache[n3GroupThreadID.x + 1][n3GroupThreadID.y].rgb) + (0.3f * gf4GroupSharedCache[n3GroupThreadID.x + 1][n3GroupThreadID.y + 1].rgb) + (-0.3f * gf4GroupSharedCache[n3GroupThreadID.x + 1][n3GroupThreadID.y + 2].rgb);

	gtxtRWOutput[n3DispatchThreadID.xy] = float4(sqrt(f3HorizontalEdge*f3HorizontalEdge + f3VerticalEdge*f3VerticalEdge), 1.0f);

}

[numthreads(32, 30, 1)]
void CSGausianBlur(int3 n3GroupThreadID : SV_GroupThreadID, int3 n3DispatchThreadID : SV_DispatchThreadID)
{
    gf4GroupSharedCache[n3GroupThreadID.x + 1][n3GroupThreadID.y + 1] = gtxtRWInput[int2(n3DispatchThreadID.x, n3DispatchThreadID.y)];
    if ((n3GroupThreadID.x == 0) && (n3GroupThreadID.y == 0))
        gf4GroupSharedCache[0][0] = gtxtRWInput[int2(n3DispatchThreadID.x - 1, n3DispatchThreadID.y - 1)];
    else if ((n3GroupThreadID.x == 31) && (n3GroupThreadID.y == 0))
        gf4GroupSharedCache[33][0] = gtxtRWInput[int2(n3DispatchThreadID.x + 1, n3DispatchThreadID.y - 1)];
    else if ((n3GroupThreadID.x == 0) && (n3GroupThreadID.y == 29))
        gf4GroupSharedCache[0][31] = gtxtRWInput[int2(n3DispatchThreadID.x - 1, n3DispatchThreadID.y + 1)];
    else if ((n3GroupThreadID.x == 31) && (n3GroupThreadID.y == 29))
        gf4GroupSharedCache[33][31] = gtxtRWInput[int2(n3DispatchThreadID.x + 1, n3DispatchThreadID.y + 1)];
    if (n3GroupThreadID.x == 0)
        gf4GroupSharedCache[0][n3GroupThreadID.y + 1] = gtxtRWInput[int2(n3DispatchThreadID.x - 1, n3DispatchThreadID.y)];
    if (n3GroupThreadID.y == 0)
        gf4GroupSharedCache[n3GroupThreadID.x + 1][0] = gtxtRWInput[int2(n3DispatchThreadID.x, n3DispatchThreadID.y - 1)];
    if (n3GroupThreadID.x == 31)    
        gf4GroupSharedCache[33][n3GroupThreadID.y + 1] = gtxtRWInput[int2(n3DispatchThreadID.x + 1, n3DispatchThreadID.y)];
    if (n3GroupThreadID.y == 29)
        gf4GroupSharedCache[n3GroupThreadID.x + 1][31] = gtxtRWInput[int2(n3DispatchThreadID.x, n3DispatchThreadID.y + 1)];

    GroupMemoryBarrierWithGroupSync();

    GausianBlur(n3GroupThreadID, n3DispatchThreadID);
}
