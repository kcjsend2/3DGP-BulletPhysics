#pragma once

class CCamera;

//정점을 표현하기 위한 클래스를 선언한다.
class CVertex
{
public:
	//정점의 위치 벡터이다(모든 정점은 최소한 위치 벡터를 가져야 한다).
	XMFLOAT3 m_xmf3Position;

public:
	CVertex() { m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f); }
	CVertex(XMFLOAT3 xmf3Position) { m_xmf3Position = xmf3Position; }
	~CVertex() { }
};

class CParticleVertex : public CVertex
{
public:
	XMFLOAT3						m_xmf3Color = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3						m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3						m_xmf3Acceleration = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT2						m_xmf2Size = XMFLOAT2(0.0f, 0.0f);
	XMFLOAT2						m_xmf2AgeLifetime = XMFLOAT2(0.0f, 0.0f); //(Age, Lifetime)
	UINT							m_nType = 0;

public:
	CParticleVertex() { }
	~CParticleVertex() { }
};

class CDiffusedVertex : public CVertex
{
protected:
	//정점의 색상이다.
	XMFLOAT4 m_xmf4Diffuse;
public:
	CDiffusedVertex()
	{
		m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_xmf4Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	}
	CDiffusedVertex(float x, float y, float z, XMFLOAT4 xmf4Diffuse)
	{
		m_xmf3Position = XMFLOAT3(x, y, z); m_xmf4Diffuse = xmf4Diffuse;
	}
	CDiffusedVertex(XMFLOAT3 xmf3Position, XMFLOAT4 xmf4Diffuse)
	{
		m_xmf3Position = xmf3Position;
		m_xmf4Diffuse = xmf4Diffuse;
	}
	~CDiffusedVertex() { }
};

class CNormalVertex : public CVertex
{
protected:
	//정점의 색상이다.
	XMFLOAT3 m_xmf3Normal;
public:
	CNormalVertex()
	{
		m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_xmf3Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}
	CNormalVertex(float x, float y, float z, XMFLOAT3 xmf3Normal)
	{
		m_xmf3Position = XMFLOAT3(x, y, z);
		m_xmf3Normal = xmf3Normal;
	}
	CNormalVertex(XMFLOAT3 xmf3Position, XMFLOAT3 xmf3Normal)
	{
		m_xmf3Position = xmf3Position;
		m_xmf3Normal = xmf3Normal;
	}
	~CNormalVertex() { }
};

class CTexturedVertex : public CVertex
{
public:
	XMFLOAT2 m_xmf2TexCoord;

public:
	CTexturedVertex() { m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f); m_xmf2TexCoord = XMFLOAT2(0.0f, 0.0f); }
	CTexturedVertex(float x, float y, float z, XMFLOAT2 xmf2TexCoord) { m_xmf3Position = XMFLOAT3(x, y, z); m_xmf2TexCoord = xmf2TexCoord; }
	CTexturedVertex(XMFLOAT3 xmf3Position, XMFLOAT2 xmf2TexCoord = XMFLOAT2(0.0f, 0.0f)) { m_xmf3Position = xmf3Position; m_xmf2TexCoord = xmf2TexCoord; }
	~CTexturedVertex() { }
};

class CTexturedNormalVertex : public CNormalVertex
{
public:
	XMFLOAT2 m_xmf2TexCoord;

public:
	CTexturedNormalVertex() { m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f); m_xmf3Normal = XMFLOAT3(0.0f, 0.0f, 0.0f); m_xmf2TexCoord = XMFLOAT2(0.0f, 0.0f); }
	CTexturedNormalVertex(XMFLOAT3 xmf3Position, XMFLOAT3 xmf3Normal, XMFLOAT2 xmf2TexCoord) { m_xmf3Position = xmf3Position; m_xmf3Normal = xmf3Normal; m_xmf2TexCoord = xmf2TexCoord; }
	~CTexturedNormalVertex() { }
};

class CMesh
{
public:
	CMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual ~CMesh();

private:

public:
	virtual void ReleaseUploadBuffers();
	BoundingOrientedBox GetBoundingBox() { return(m_xmBoundingBox); }

protected:
	D3D12_PRIMITIVE_TOPOLOGY m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	UINT m_nSlot = 0;
	UINT m_nVertices = 0;
	UINT m_nStride = 0;
	UINT m_nOffset = 0;

	XMFLOAT3* m_pxmf3Positions = NULL;
	ID3D12Resource* m_pd3dPositionBuffer = NULL;
	ID3D12Resource* m_pd3dPositionUploadBuffer = NULL;

	XMFLOAT3* m_pxmf3Normals = NULL;
	ID3D12Resource* m_pd3dNormalBuffer = NULL;
	ID3D12Resource* m_pd3dNormalUploadBuffer = NULL;

	XMFLOAT2* m_pxmf2TextureCoords = NULL;
	ID3D12Resource* m_pd3dTextureCoordBuffer = NULL;
	ID3D12Resource* m_pd3dTextureCoordUploadBuffer = NULL;

	UINT m_nIndices = 0;
	UINT* m_pnIndices = NULL;
	ID3D12Resource* m_pd3dIndexBuffer = NULL;
	ID3D12Resource* m_pd3dIndexUploadBuffer = NULL;

	UINT m_nVertexBufferViews = 0;
	D3D12_VERTEX_BUFFER_VIEW* m_pd3dVertexBufferViews = NULL;

	D3D12_INDEX_BUFFER_VIEW	m_d3dIndexBufferView;

	UINT m_nStartIndex = 0;
	int	m_nBaseVertex = 0;

protected:
	BoundingOrientedBox m_xmBoundingBox;

public:
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, UINT nInstances);

	virtual void PreRender(ID3D12GraphicsCommandList* pd3dCommandList) {};
	virtual void PostRender(ID3D12GraphicsCommandList* pd3dCommandList) {};

	virtual void IncreaseCurrentPipline() {};
	virtual void ResetCurrentPipline() {};
};

class CTriangleMesh : public CMesh
{
public:
	CTriangleMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual ~CTriangleMesh() { }
};

class CCubeMesh : public CMesh
{
public:
	//직육면체의 가로, 세로, 깊이의 길이를 지정하여 직육면체 메쉬를 생성한다.
	CCubeMesh(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f);
	virtual ~CCubeMesh();
};

class CHeightMapImage
{
private:
	//높이 맵 이미지 픽셀(8-비트)들의 이차원 배열이다. 각 픽셀은 0~255의 값을 갖는다.
	BYTE *m_pHeightMapPixels;

	//높이 맵 이미지의 가로와 세로 크기이다.
	int m_nWidth;
	int m_nLength;

	//높이 맵 이미지를 실제로 몇 배 확대하여 사용할 것인가를 나타내는 스케일 벡터이다.
	XMFLOAT3 m_xmf3Scale;

public:
	CHeightMapImage(LPCTSTR pFileName, int nWidth, int nLength, XMFLOAT3 xmf3Scale);
	~CHeightMapImage(void);

	//높이 맵 이미지에서 (x, z) 위치의 픽셀 값에 기반한 지형의 높이를 반환한다.
	float GetHeight(float x, float z);

	//높이 맵 이미지에서 (x, z) 위치의 법선 벡터를 반환한다.
	XMFLOAT3 GetHeightMapNormal(int x, int z);
	XMFLOAT3 GetScale() { return(m_xmf3Scale); }
	BYTE* GetHeightMapPixels() { return(m_pHeightMapPixels); }
	int GetHeightMapWidth() { return(m_nWidth); }
	int GetHeightMapLength() { return(m_nLength); }
};

class CHeightMapGridMesh : public CMesh
{
protected:
	//격자의 크기(가로: x-방향, 세로: z-방향)이다.
	int m_nWidth;
	int m_nLength;
	/*격자의 스케일(가로: x-방향, 세로: z-방향, 높이: y-방향) 벡터이다. 실제 격자 메쉬의 각 정점의 x-좌표, y-좌표, z-좌표는 스케일 벡터의 x-좌표, y-좌표, z-좌표로 곱한 값을 갖는다. 즉, 실제 격자의 x-축 방향의 간격은 1이 아니
	라 스케일 벡터의 x-좌표가 된다. 이렇게 하면 작은 격자(적은 정점)를 사용하더라도 큰 크기의 격자(지형)를 생성할
	수 있다.*/
	XMFLOAT3 m_xmf3Scale;

	XMFLOAT2* m_pxmf2DetailTextureCoords = NULL;
	ID3D12Resource* m_pd3dDetailTextureBuffer = NULL;
	ID3D12Resource* m_pd3dDetailTextureUploadBuffer = NULL;

public:
	CHeightMapGridMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, int xStart, int zStart, int nWidth, int nLength, float& fMaxHeight, float& fMinHeight, XMFLOAT3 xmf3Scale = XMFLOAT3(1.0f, 1.0f, 1.0f), void* pContext = NULL);
	virtual ~CHeightMapGridMesh();
	XMFLOAT3 GetScale() { return(m_xmf3Scale); }
	int GetWidth() { return(m_nWidth); }
	int GetLength() { return(m_nLength); }

	//격자의 좌표가 (x, z)일 때 교점(정점)의 높이를 반환하는 함수이다.
	virtual float OnGetHeight(int x, int z, void *pContext);

	virtual XMFLOAT3 OnGetNormal(int x, int z, void* pContext);

	//격자의 좌표가 (x, z)일 때 교점(정점)의 색상을 반환하는 함수이다.
	virtual XMFLOAT4 OnGetColor(int x, int z, void *pContext);

};

class CMeshFileRead : public CMesh
{
public:
	CMeshFileRead(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, char* pstrFileName = NULL, bool bTextFile = true, XMFLOAT3 xmf3Scale = { 1.0f, 1.0f, 1.0f }, XMFLOAT3 xmf3Rotation = { 0.0f, 0.0f, 0.0f });
	virtual ~CMeshFileRead();
	virtual void ReleaseUploadBuffers();

public:
	void LoadMeshFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, char* pstrFileName, bool bTextFile, XMFLOAT3 xmf3Scale, XMFLOAT3 xmf3Rotation);
};

class CTexturedRectMesh : public CMesh
{
public:
	CTexturedRectMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, float fWidth = 20.0f, float fHeight = 20.0f, float fDepth = 20.0f, float fxPosition = 0.0f, float fyPosition = 0.0f, float fzPosition = 0.0f);
	virtual ~CTexturedRectMesh();
};

class CRectMesh : public CMesh
{
public:
	CRectMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, float fWidth = 20.0f, float fHeight = 20.0f, float fDepth = 20.0f, float fxPosition = 0.0f, float fyPosition = 0.0f, float fzPosition = 0.0f);
	virtual ~CRectMesh();
};

class CBillBoardMesh : public CMesh
{
public:
	CBillBoardMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT3* pxmf3Position, float fWidth, float fHeight, int nVertices);
	virtual ~CBillBoardMesh();

protected:
	ID3D12Resource* m_pd3dSizeBuffer = NULL;
	ID3D12Resource* m_pd3dSizeUploadBuffer = NULL;
};

class CParticleMesh : public CMesh
{
public:
	CParticleMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT3 xmf3Position, XMFLOAT3 xmf3Velocity, XMFLOAT3 xmf3Acceleration, XMFLOAT3 xmf3Color, XMFLOAT2 xmf2Size, float fLifetime, UINT nMaxParticles);
	virtual ~CParticleMesh();

	int m_nParticle;

	bool m_bStart = true;
	bool m_nCurrentPipline = 0;

	UINT m_nMaxParticles = 300;


	ID3D12Resource* m_pd3dStreamOutputBuffer = NULL;
	ID3D12Resource* m_pd3dDrawBuffer = NULL;

	ID3D12Resource* m_pd3dDefaultBufferFilledSize = NULL;
	ID3D12Resource* m_pd3dUploadBufferFilledSize = NULL;
	UINT64* m_pnUploadBufferFilledSize = NULL;

	ID3D12QueryHeap* m_pd3dSOQueryHeap = NULL;
	ID3D12Resource* m_pd3dSOQueryBuffer = NULL;
	D3D12_QUERY_DATA_SO_STATISTICS* m_pd3dSOQueryDataStatistics = NULL;

	D3D12_STREAM_OUTPUT_BUFFER_VIEW	m_d3dStreamOutputBufferView;

	virtual void CreateVertexBuffer(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT3 xmf3Position, XMFLOAT3 xmf3Velocity, XMFLOAT3 xmf3Acceleration, XMFLOAT3 xmf3Color, XMFLOAT2 xmf2Size, float fLifetime);
	virtual void CreateStreamOutputBuffer(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, UINT nMaxParticles);

	virtual void PreRender(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void PostRender(ID3D12GraphicsCommandList* pd3dCommandList);

	virtual void IncreaseCurrentPipline() { m_nCurrentPipline++; }
	virtual void ResetCurrentPipline() { m_nCurrentPipline = 0; }
};
