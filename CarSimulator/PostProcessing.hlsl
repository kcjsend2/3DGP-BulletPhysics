///////////////////////////////////////////////////////////////////////////////
//
RWTexture2D<float4> gtxtRWInput : register(u0);
RWTexture2D<float4> gtxtRWOutput : register(u1);

static float3 gf3ToLuminance = float3(0.3f, 0.59f, 0.11f);

#define _WITH_BY_LUMINANCE

#define _WITH_SOBEL_EDGE

#define _WITH_GROUPSHARED_MEMORY
#ifdef _WITH_GROUPSHARED_MEMORY
groupshared float4 gf4GroupSharedCache[32 + 2][30 + 2];

//////////////////////////////////////////////////////////////////////////////////
// Sobel Edge Detection
//
void SobelEdge(int3 n3GroupThreadID : SV_GroupThreadID, int3 n3DispatchThreadID : SV_DispatchThreadID)
{
#ifdef _WITH_BY_LUMINANCE
    float fHorizontalEdge = (-1.0f * dot(gf3ToLuminance, gf4GroupSharedCache[n3GroupThreadID.x][n3GroupThreadID.y + 1].rgb)) + (2.0f * dot(gf3ToLuminance, gf4GroupSharedCache[n3GroupThreadID.x + 1][n3GroupThreadID.y + 1].rgb)) + (-1.0f * dot(gf3ToLuminance, gf4GroupSharedCache[n3GroupThreadID.x + 2][n3GroupThreadID.y + 1].rgb));
    float fVerticalEdge = (-1.0f * dot(gf3ToLuminance, gf4GroupSharedCache[n3GroupThreadID.x + 1][n3GroupThreadID.y].rgb)) + (2.0f * dot(gf3ToLuminance, gf4GroupSharedCache[n3GroupThreadID.x + 1][n3GroupThreadID.y + 1].rgb)) + (-1.0f * dot(gf3ToLuminance, gf4GroupSharedCache[n3GroupThreadID.x + 1][n3GroupThreadID.y + 2].rgb));
#else
	float3 f3HorizontalEdge = (-1.0f * gf4GroupSharedCache[n3GroupThreadID.x][n3GroupThreadID.y+1].rgb) + (2.0f * gf4GroupSharedCache[n3GroupThreadID.x+1][n3GroupThreadID.y+1].rgb) + (-1.0f * gf4GroupSharedCache[n3GroupThreadID.x+2][n3GroupThreadID.y+1].rgb);	
	float3 f3VerticalEdge = (-1.0f * gf4GroupSharedCache[n3GroupThreadID.x+1][n3GroupThreadID.y].rgb) + (2.0f * gf4GroupSharedCache[n3GroupThreadID.x+1][n3GroupThreadID.y+1].rgb) + (-1.0f * gf4GroupSharedCache[n3GroupThreadID.x+1][n3GroupThreadID.y+2].rgb);	
#endif
#ifdef _WITH_BY_LUMINANCE
    float3 cEdgeness = sqrt(fHorizontalEdge * fHorizontalEdge + fVerticalEdge * fVerticalEdge);
    gtxtRWOutput[n3DispatchThreadID.xy] = float4(cEdgeness, 1.0f);
#else
	gtxtRWOutput[n3DispatchThreadID.xy] = float4(sqrt(f3HorizontalEdge*f3HorizontalEdge + f3VerticalEdge*f3VerticalEdge), 1.0f);
#endif
}

//////////////////////////////////////////////////////////////////////////////////
// Laplacian Edge Detection
//
static float gfLaplacians[9] = { -1.0f, -1.0f, -1.0f, -1.0f, 8.0f, -1.0f, -1.0f, -1.0f, -1.0f };
static int2 gnOffsets[9] = { { -1, -1 }, { 0, -1 }, { 1, -1 }, { -1, 0 }, { 0, 0 }, { 1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 } };

void LaplacianEdge(int3 n3GroupThreadID : SV_GroupThreadID, int3 n3DispatchThreadID : SV_DispatchThreadID)
{
    float3 cEdgeness = float3(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < 9; i++)
    {
        cEdgeness += gfLaplacians[i] * dot(gf3ToLuminance, gf4GroupSharedCache[n3GroupThreadID.x + 1 + gnOffsets[i].x][n3GroupThreadID.y + 1 + gnOffsets[i].y].xyz);
    }

    gtxtRWOutput[n3DispatchThreadID.xy] = float4(cEdgeness, 1.0f);
}
#else
//////////////////////////////////////////////////////////////////////////////////
// Sobel Edge Detection
//
void SobelEdge(int3 n3DispatchThreadID : SV_DispatchThreadID)
{
#ifdef _WITH_BY_LUMINANCE
    float fHorizontalEdge = (-1.0f * dot(gf3ToLuminance, gtxtRWOutput[int2(n3DispatchThreadID.x - 1, n3DispatchThreadID.y)].rgb)) + (2.0f * dot(gf3ToLuminance, gtxtRWOutput[int2(n3DispatchThreadID.x, n3DispatchThreadID.y)].rgb)) + (-1.0f * dot(gf3ToLuminance, gtxtRWOutput[int2(n3DispatchThreadID.x + 1, n3DispatchThreadID.y)].rgb));
    float fVerticalEdge = (-1.0f * dot(gf3ToLuminance, gtxtRWOutput[int2(n3DispatchThreadID.x, n3DispatchThreadID.y - 1)].rgb)) + (2.0f * dot(gf3ToLuminance, gtxtRWOutput[int2(n3DispatchThreadID.x, n3DispatchThreadID.y)].rgb)) + (-1.0f * dot(gf3ToLuminance, gtxtRWOutput[int2(n3DispatchThreadID.x, n3DispatchThreadID.y + 1)].rgb));
#else
	float3 f3HorizontalEdge = (-1.0f * gtxtInput[int2(n3DispatchThreadID.x-1, n3DispatchThreadID.y)].rgb) + (2.0f * gtxtInput[int2(n3DispatchThreadID.x, n3DispatchThreadID.y)].rgb) + (-1.0f * gtxtInput[int2(n3DispatchThreadID.x+1, n3DispatchThreadID.y)].rgb);	
	float3 f3VerticalEdge = (-1.0f * gtxtInput[int2(n3DispatchThreadID.x, n3DispatchThreadID.y-1)].rgb) + (2.0f * gtxtInput[int2(n3DispatchThreadID.x, n3DispatchThreadID.y)].rgb) + (-1.0f * gtxtInput[int2(n3DispatchThreadID.x, n3DispatchThreadID.y+1)].rgb);	
#endif
#ifdef _WITH_BY_LUMINANCE
	float3 cEdgeness = sqrt(fHorizontalEdge * fHorizontalEdge + fVerticalEdge * fVerticalEdge);
	gtxtRWOutput[n3DispatchThreadID.xy] = float4(cEdgeness, 1.0f);
#else
	gtxtRWOutput[n3DispatchThreadID.xy] = float4(sqrt(f3HorizontalEdge*f3HorizontalEdge + f3VerticalEdge*f3VerticalEdge), 1.0f);
#endif
}

//////////////////////////////////////////////////////////////////////////////////
// Laplacian Edge Detection
//
static float gfLaplacians[9] = { -1.0f, -1.0f, -1.0f, -1.0f, 8.0f, -1.0f, -1.0f, -1.0f, -1.0f };
static int2 gnOffsets[9] = { { -1,-1 }, { 0,-1 }, { 1,-1 }, { -1,0 }, { 0,0 }, { 1,0 }, { -1,1 }, { 0,1 }, { 1,1 } };

void LaplacianEdge(int3 n3DispatchThreadID : SV_DispatchThreadID)
{
	float3 cEdgeness = float3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 9; i++)
	{
        cEdgeness += gfLaplacians[i] * dot(gf3ToLuminance, gtxtRWOutput[int2(n3DispatchThreadID.xy) + gnOffsets[i]].xyz);
    }

	gtxtRWOutput[n3DispatchThreadID.xy] = float4(cEdgeness, 1.0f);
}
#endif

[numthreads(32, 30, 1)]
void CSEdgeDetection(int3 n3GroupThreadID : SV_GroupThreadID, int3 n3DispatchThreadID : SV_DispatchThreadID)
{
#ifdef _WITH_GROUPSHARED_MEMORY
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

#ifdef _WITH_SOBEL_EDGE
    SobelEdge(n3GroupThreadID, n3DispatchThreadID);
#else
	LaplacianEdge(n3GroupThreadID, n3DispatchThreadID);
#endif
#else
#ifdef _WITH_SOBEL_EDGE
	SobelEdge(n3DispatchThreadID);
#else
	LaplacianEdge(n3DispatchThreadID);
#endif
#endif
}
