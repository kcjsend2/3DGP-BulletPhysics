#pragma once
#include "Mesh.h"

class CCamera;
class CShader;
class CScene;

#define RESOURCE_TEXTURE2D			0x01
#define RESOURCE_TEXTURE2D_ARRAY	0x02	//[]
#define RESOURCE_TEXTURE2DARRAY		0x03
#define RESOURCE_TEXTURE_CUBE		0x04
#define RESOURCE_BUFFER				0x05

struct MATERIAL //��Ƽ���� ����
{
	XMFLOAT4 AmbientLight;
	XMFLOAT4 DiffuseAlbedo;
	XMFLOAT3 FresnelR0;
	float Shininess;
};

struct CB_ANIMATEDBILLBOARD
{
	CB_ANIMATEDBILLBOARD(int nx, int ny, int nxDivided, int nyDivided)
	{
		m_nx = nx;
		m_ny = ny;
		m_nxDivided = nxDivided;
		m_nyDivided = nyDivided;
	}
	int m_nx;
	int m_ny;
	int m_nxDivided;
	int m_nyDivided;
};

class CTexture
{
public:
	CTexture(int nTextureResources, UINT nResourceType, int nSamplers, int nRootParameters);
	virtual ~CTexture();

private:
	int m_nReferences = 0;

	UINT m_nTextureType;

	int	m_nTextures = 0;
	ID3D12Resource** m_ppd3dTextures = NULL;
	ID3D12Resource** m_ppd3dTextureUploadBuffers;

	UINT* m_pnResourceTypes = NULL;

	_TCHAR(*m_ppstrTextureNames)[64] = NULL;

	DXGI_FORMAT* m_pdxgiBufferFormats = NULL;
	int* m_pnBufferElements = NULL;

	int	m_nRootParameters = 0;
	int* m_pnRootParameterIndices = NULL;
	D3D12_GPU_DESCRIPTOR_HANDLE* m_pd3dSrvGpuDescriptorHandles = NULL;

	int	m_nSamplers = 0;
	D3D12_GPU_DESCRIPTOR_HANDLE* m_pd3dSamplerGpuDescriptorHandles = NULL;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

	void SetSampler(int nIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSamplerGpuDescriptorHandle);

	void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, int nParameterIndex, int nTextureIndex);
	void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	void ReleaseShaderVariables();

	void LoadTextureFromDDSFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, wchar_t* pszFileName, UINT nResourceType, UINT nIndex);

	void SetRootParameterIndex(int nIndex, UINT nRootParameterIndex);
	void SetGpuDescriptorHandle(int nIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSrvGpuDescriptorHandle);

	int GetRootParameters() { return(m_nRootParameters); }
	int GetTextures() { return(m_nTextures); }
	_TCHAR* GetTextureName(int nIndex) { return(m_ppstrTextureNames[nIndex]); }
	ID3D12Resource* CreateTexture(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, UINT nWidth, UINT nHeight, UINT nElements, UINT nMipLevels, DXGI_FORMAT dxgiFormat, D3D12_RESOURCE_FLAGS d3dResourceFlags, D3D12_RESOURCE_STATES d3dResourceStates, D3D12_CLEAR_VALUE* pd3dClearValue, UINT nResourceType, UINT nIndex);
	ID3D12Resource* GetResource(int nIndex) { return(m_ppd3dTextures[nIndex]); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGpuDescriptorHandle(int nIndex) { return(m_pd3dSrvGpuDescriptorHandles[nIndex]); }
	int GetRootParameter(int nIndex) { return(m_pnRootParameterIndices[nIndex]); }

	UINT GetTextureType() { return(m_nTextureType); }
	UINT GetTextureType(int nIndex) { return(m_pnResourceTypes[nIndex]); }
	DXGI_FORMAT GetBufferFormat(int nIndex) { return(m_pdxgiBufferFormats[nIndex]); }
	int GetBufferElements(int nIndex) { return(m_pnBufferElements[nIndex]); }

	D3D12_SHADER_RESOURCE_VIEW_DESC GetShaderResourceViewDesc(int nIndex);

	void ReleaseUploadBuffers();
};


class CGameObject
{
public:
	CGameObject(int nMeshes = 1);
	virtual ~CGameObject();
public:
	int m_nReferences = 0;
	std::vector<std::shared_ptr<CMesh>> m_vpMeshes;
	MATERIAL m_material = { XMFLOAT4{}, XMFLOAT4{}, XMFLOAT3{}, 0};

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }
protected:
	XMFLOAT4X4 m_xmf4x4World;
	CShader* m_pShader = NULL;
	btRigidBody* m_pbtRigidBody = NULL;
	int m_nInstance = 1;
	int m_nTextureIndex = 0;

public:
	void ReleaseUploadBuffers();
	void SetMesh(std::shared_ptr<CMesh> pMesh);
	void SetMesh(std::shared_ptr<CCubeMesh> pMesh);
	std::shared_ptr<CMesh> GetMesh(int nIndex) { return m_vpMeshes[nIndex]; }
	int GetTextureIndex() { return m_nTextureIndex; }
	void SetTextureIndex(int nIndex) { m_nTextureIndex = nIndex; }

	//���� ��ü�� ȸ��(x-��, y-��, z-��)�Ѵ�.
	void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
	void Rotate(XMFLOAT3* pxmf3Axis, float fAngle);

	virtual void SetShader(CShader* pShader);
	virtual void Update(float fTimeElapsed, btDiscreteDynamicsWorld* pbtDynamicsWorld);
	virtual void OnPrepareRender();
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ShadowPassRender(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, UINT nInstances);
	virtual btRigidBody* GetRigidBody() { return m_pbtRigidBody; }
	virtual void SetRigidBody(btRigidBody* pbtRigidBody) { m_pbtRigidBody = pbtRigidBody; }
	virtual void SetMatrix(XMFLOAT4X4 xmf4x4Matrix) { m_xmf4x4World = xmf4x4Matrix; }
	virtual BoundingOrientedBox GetBoudingBox(int index);
	virtual void OnPreRender(ID3D12GraphicsCommandList* pd3dCommandList, CScene* pScene) { }
	virtual CShader* GetShader() { return m_pShader; }

public:
	//��� ���۸� �����Ѵ�.
	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	//��� ������ ������ �����Ѵ�.
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList, float fTimeElapesd) {};
	virtual void ReleaseShaderVariables();
	void SetInstanceNum(int nInstance) { m_nInstance = nInstance; }

	//���� ��ü�� ���� ��ȯ ��Ŀ��� ��ġ ���Ϳ� ����(x-��, y-��, z-��) ���͸� ��ȯ�Ѵ�.
	XMFLOAT3 GetPosition();
	XMFLOAT3 GetLook();
	XMFLOAT3 GetUp();
	XMFLOAT3 GetRight();
	XMFLOAT4X4 GetWorldTransformMatrix() { return m_xmf4x4World; }

	//���� ��ü�� ��ġ�� �����Ѵ�.
	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 xmf3Position);
	void SetMaterial(XMFLOAT4 xmf4Ambient, XMFLOAT4 xmf4DiffuseAlbedo, XMFLOAT3 xmf3FresneIR, float fShininess);

	//���� ��ü�� ���� x-��, y-��, z-�� �������� �̵��Ѵ�.
	void MoveStrafe(float fDistance = 1.0f);
	void MoveUp(float fDistance = 1.0f);
	void MoveForward(float fDistance = 1.0f);

};

class CRotatingObject : public CGameObject
{
public:
	CRotatingObject(int nMeshes = 1);
	virtual ~CRotatingObject();
private:
	XMFLOAT3 m_xmf3RotationAxis;
	float m_fRotationSpeed;
public:
	void SetRotationSpeed(float fRotationSpeed) { m_fRotationSpeed = fRotationSpeed; }
	void SetRotationAxis(XMFLOAT3 xmf3RotationAxis)
	{
		m_xmf3RotationAxis = xmf3RotationAxis;
	}
	void SetRigidBody(btRigidBody::btRigidBodyConstructionInfo rbInfo) { m_pbtRigidBody = new btRigidBody(rbInfo); }

	virtual void Update(float fTimeElapsed, btDiscreteDynamicsWorld* pbtDynamicsWorld);
};

class CHeightMapTerrain : public CGameObject
{
public:
	CHeightMapTerrain(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, LPCTSTR pFileName, int nWidth, int nLength, int nBlockWidth, int nBlockLength, XMFLOAT3 xmf3Scale, btAlignedObjectArray<btCollisionShape*>& btCollisionShapes, btDiscreteDynamicsWorld* pbtDynamicsWorld, ComPtr<ID3D12DescriptorHeap> pd3dSrvDescriptorHeap);
	virtual ~CHeightMapTerrain();
private:
	//������ ���� ������ ����� �̹����̴�.
	CHeightMapImage *m_pHeightMapImage;

	//���� ���� ���ο� ���� ũ���̴�.
	int m_nWidth;
	int m_nLength;

	//������ ������ �� �� Ȯ���� ���ΰ��� ��Ÿ���� ������ �����̴�.
	XMFLOAT3 m_xmf3Scale;
	btHeightfieldTerrainShape* m_pbtTerrainShape;
	float* m_pfHeightmapData;
public:
	//������ ���̸� ����ϴ� �Լ��̴�(���� ��ǥ��). ���� ���� ���̿� �������� y�� ���� ���̴�.
	float GetHeight(float x, float z) { return(m_pHeightMapImage->GetHeight(x / m_xmf3Scale.x, z / m_xmf3Scale.z)* m_xmf3Scale.y); }

	//������ ���� ���͸� ����ϴ� �Լ��̴�(���� ��ǥ��). ���� ���� ���� ���͸� ����Ѵ�.
	XMFLOAT3 GetNormal(float x, float z) { return(m_pHeightMapImage->GetHeightMapNormal(int(x / m_xmf3Scale.x), int(z / m_xmf3Scale.z))); }
	int GetHeightMapWidth() { return(m_pHeightMapImage->GetHeightMapWidth()); }
	int GetHeightMapLength() { return(m_pHeightMapImage->GetHeightMapLength()); }
	XMFLOAT3 GetScale() { return(m_xmf3Scale); }

	//������ ũ��(����/����)�� ��ȯ�Ѵ�. ���� ���� ũ�⿡ �������� ���� ���̴�.
	float GetWidth() { return(m_nWidth * m_xmf3Scale.x); }
	float GetLength() { return(m_nLength * m_xmf3Scale.z); }
};

class CSkyBox : public CGameObject
{
public:
	CSkyBox(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual ~CSkyBox();

	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = NULL);
};

class CBullet : public CGameObject
{
public:
	CBullet(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT3 xmf3Position, btVector3 btf3Forward, btDiscreteDynamicsWorld* pbtDynamicsWorld);
	virtual ~CBullet();
	virtual void Update(float fTimeElapsed, btDiscreteDynamicsWorld* pbtDynamicsWorld);
	float GetTimeRemain() { return m_fTimeRemain; }

private:
	float m_fTimeRemain = 1.0f;
};

class CAnimatedBillBoard : public CGameObject
{
public:
	CAnimatedBillBoard(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT3 xmf3Position, int nxDivided, int nyDivided, std::vector<float> pfFrameTime);
	virtual ~CAnimatedBillBoard();
	CB_ANIMATEDBILLBOARD GetBillBoardInfo() { return CB_ANIMATEDBILLBOARD(m_nx, m_ny, m_nxDivided, m_nyDivided); }
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList, float fTimeElapesd);
	bool isEnd()
	{
		if (m_vfFrameTime.size() == 0)
			return true;

		return false;
	}

private:
	int m_nCurrentFrame = 0;
	int m_nxDivided;
	int m_nyDivided;

	int m_nx = 0;
	int m_ny = 0;

	std::vector<float> m_vfFrameTime;
};

class CParticle : public CGameObject
{
public:
	CParticle(float fLiveTime, XMFLOAT3 m_xmf3Velocity);
	virtual ~CParticle();
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList, float fTimeElapesd);
	virtual float GetLiveTime() { return m_fLiveTime; }
	virtual void Update(float fTimeElapsed);

private:
	XMFLOAT3 m_xmf3Velocity;
	float m_fLiveTime;
};