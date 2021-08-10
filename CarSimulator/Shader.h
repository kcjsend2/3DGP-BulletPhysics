#pragma once
#include "GameObject.h"
#include "Camera.h"
#include "Player.h"
#include "Light.h"

//게임 객체의 정보를 셰이더에게 넘겨주기 위한 구조체(상수 버퍼)이다.
struct VS_VB_INSTANCE
{
	XMFLOAT4X4 m_xmf4x4Transform;
};

struct VS_VB_LIGHT_INFO
{
	int type;
	XMFLOAT3 Strength;
	float FalloffStart; // point/spot light only
	XMFLOAT3 Direction; // directional/spot light only
	float FalloffEnd; // point/spot light only
	XMFLOAT3 Position; // point light only
	float SpotPower; // spot light only

	VS_VB_LIGHT_INFO(CLight& li)
	{
		type = li.m_itype;
		Position = li.GetPosition();
		Direction = li.m_xmf3Direction;
		Strength = li.m_xmf3Strength;
		FalloffStart = li.m_fFalloffStart;
		FalloffEnd = li.m_fFalloffEnd;
		SpotPower = li.m_fSpotPower;
	}
};

//셰이더 소스 코드를 컴파일하고 그래픽스 상태 객체를 생성한다.
class CShader
{
public:
	CShader();
	virtual ~CShader();
private:
	int m_nReferences = 0;
public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }
	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_RASTERIZER_DESC CreateRasterizerState();
	virtual D3D12_BLEND_DESC CreateBlendState();
	virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState();
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** ppd3dShaderBlob);
	D3D12_SHADER_BYTECODE CompileShaderFromFile(WCHAR* pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderProfile, ID3DBlob** ppd3dShaderBlob);
	virtual void CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature);
	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();
	virtual void UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World);
	virtual void OnPrepareRender(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);
	virtual void Update(float fTimeElapsed);

protected:
	ID3D12PipelineState* m_pd3dPipelineState = NULL;
	int m_nPipelineStates = 0;
};

class CShadowShader : public CShader
{
public:
	CShadowShader();
	virtual ~CShadowShader();
	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual void CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature);
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** ppd3dShaderBlob);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CPlayer* pPlayer);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT3 xmf3TargetPos);
	void SetLight(CLight* pLight) { m_pLight = pLight; }
	std::vector<CGameObject*> GetObjectVector() { return m_vpGameObjects; }
protected:
	CLight* m_pLight = NULL;
	std::vector<CGameObject*> m_vpGameObjects;
};

class CPlayerShader : public CShader
{
public:
	CPlayerShader();
	virtual ~CPlayerShader();
	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** ppd3dShaderBlob);
	virtual void Update(float fTimeElapsed);
};


//“CObjectsShader” 클래스는 게임 객체들을 포함하는 셰이더 객체이다.
class CObjectsShader : public CShader
{
public:
	CObjectsShader();
	virtual ~CObjectsShader();
	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, void* pContext);
	virtual void Update(float fTimeElapsed, btDiscreteDynamicsWorld* pbtDynamicsWorld);
	virtual void ReleaseObjects();
	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** ppd3dShaderBlob);
	virtual void CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature);
	virtual void ReleaseUploadBuffers();
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);
	virtual CGameObject** GetObjects() { return m_ppObjects; }
	virtual int GetObjectsNumber() { return m_nObjects; }

protected:
	CGameObject** m_ppObjects = NULL;
	int m_nObjects = 0;
};

class CInstancingShader : public CObjectsShader
{
public:
	CInstancingShader();
	virtual ~CInstancingShader();
	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** ppd3dShaderBlob);
	virtual void CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature);
	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, void* pContext, btAlignedObjectArray<btCollisionShape*>& btCollisionShapes, btDiscreteDynamicsWorld* pbtDynamicsWorld);
	virtual void ReleaseShaderVariables();
	virtual void ReleaseUploadBuffers();
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);
protected:
	//인스턴스 데이터를 포함하는 버퍼와 포인터이다.
	ID3D12Resource* m_pd3dcbGameObjects = NULL;
	VS_VB_INSTANCE* m_pcbMappedGameObjects = NULL;
};
class CTerrainShader : public CShader
{
public:
	CTerrainShader();
	virtual ~CTerrainShader();
	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** ppd3dShaderBlob);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature);
};

class CLightsShader : public CShader
{
public:
	CLightsShader();
	virtual ~CLightsShader();
	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseObjects();
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);
	virtual D3D12_INPUT_LAYOUT_DESC CreateInputLayout();
	virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** ppd3dShaderBlob);
	virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** ppd3dShaderBlob);
	virtual void ReleaseUploadBuffers();
	virtual void ReleaseShaderVariables();
	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	CLight* GetDirectionalLight() { return &m_vLight[0]; }
protected:
	ID3D12Resource* m_pd3dcbLight = NULL;
	std::vector<CLight> m_vLight;
	VS_VB_LIGHT_INFO* m_pcbMappedLights = NULL;
};