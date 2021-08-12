#pragma once
#include "Mesh.h"

class CCamera;
class CShader;

struct MATERIAL //머티리얼 정보
{
	XMFLOAT4 AmbientLight;
	XMFLOAT4 DiffuseAlbedo;
	XMFLOAT3 FresnelR0;
	float Shininess;
};

class CGameObject
{
public:
	CGameObject(int nMeshes = 1);
	virtual ~CGameObject();
public:
	int m_nReferences = 0;
	CMesh** m_ppMeshes = NULL;
	int m_nMeshes = 0;
	MATERIAL m_material = { XMFLOAT4{}, XMFLOAT4{}, XMFLOAT3{}, 0};

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }
protected:
	XMFLOAT4X4 m_xmf4x4World;
	CShader* m_pShader = NULL;
	btRigidBody* m_pbtRigidBody = NULL;

public:
	void ReleaseUploadBuffers();
	void SetMesh(int nIndex, CMesh* pMesh);
	void SetMesh(int nIndex, CCubeMeshDiffused* pMesh);

	//게임 객체를 회전(x-축, y-축, z-축)한다.
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
public:
	//상수 버퍼를 생성한다.
	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	//상수 버퍼의 내용을 갱신한다.
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();
	bool IsVisible(CCamera* pCamera = NULL);

	//게임 객체의 월드 변환 행렬에서 위치 벡터와 방향(x-축, y-축, z-축) 벡터를 반환한다.
	XMFLOAT3 GetPosition();
	XMFLOAT3 GetLook();
	XMFLOAT3 GetUp();
	XMFLOAT3 GetRight();
	XMFLOAT4X4 GetWorldTransformMatrix() { return m_xmf4x4World; }

	//게임 객체의 위치를 설정한다.
	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 xmf3Position);
	void SetMaterial(XMFLOAT4 xmf4Ambient, XMFLOAT4 xmf4DiffuseAlbedo, XMFLOAT3 xmf3FresneIR, float fShininess);

	//게임 객체를 로컬 x-축, y-축, z-축 방향으로 이동한다.
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
	CHeightMapTerrain(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, LPCTSTR pFileName, int nWidth, int nLength, int nBlockWidth, int nBlockLength, XMFLOAT3 xmf3Scale, btAlignedObjectArray<btCollisionShape*>& btCollisionShapes, btDiscreteDynamicsWorld* pbtDynamicsWorld);
	virtual ~CHeightMapTerrain();
private:
	//지형의 높이 맵으로 사용할 이미지이다.
	CHeightMapImage *m_pHeightMapImage;

	//높이 맵의 가로와 세로 크기이다.
	int m_nWidth;
	int m_nLength;

	//지형을 실제로 몇 배 확대할 것인가를 나타내는 스케일 벡터이다.
	XMFLOAT3 m_xmf3Scale;
	btHeightfieldTerrainShape* m_pbtTerrainShape;
	float* m_pfHeightmapData;
public:
	//지형의 높이를 계산하는 함수이다(월드 좌표계). 높이 맵의 높이에 스케일의 y를 곱한 값이다.
	float GetHeight(float x, float z) { return(m_pHeightMapImage->GetHeight(x / m_xmf3Scale.x, z / m_xmf3Scale.z)* m_xmf3Scale.y); }

	//지형의 법선 벡터를 계산하는 함수이다(월드 좌표계). 높이 맵의 법선 벡터를 사용한다.
	XMFLOAT3 GetNormal(float x, float z) { return(m_pHeightMapImage->GetHeightMapNormal(int(x / m_xmf3Scale.x), int(z / m_xmf3Scale.z))); }
	int GetHeightMapWidth() { return(m_pHeightMapImage->GetHeightMapWidth()); }
	int GetHeightMapLength() { return(m_pHeightMapImage->GetHeightMapLength()); }
	XMFLOAT3 GetScale() { return(m_xmf3Scale); }

	//지형의 크기(가로/세로)를 반환한다. 높이 맵의 크기에 스케일을 곱한 값이다.
	float GetWidth() { return(m_nWidth * m_xmf3Scale.x); }
	float GetLength() { return(m_nLength * m_xmf3Scale.z); }
};