#include "Shader.h"
#include "Light.h"
class CCamera;

class CScene
{
public:
	CScene();
	~CScene();
	//씬에서 마우스와 키보드 메시지를 처리한다.
	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, btAlignedObjectArray<btCollisionShape*>& btCollisionShapes, btDiscreteDynamicsWorld* pbtDynamicsWorld);
	void ReleaseObjects();
	bool ProcessInput(UCHAR* pKeysBuffer);
	void Update(float fTimeElapsed, btDiscreteDynamicsWorld* pbtDynamicsWorld);
	void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);
	std::array<const CD3DX12_STATIC_SAMPLER_DESC, 7> GetStaticSamplers();
	void ReleaseUploadBuffers();
	//그래픽 루트 시그너쳐를 생성한다.
	ID3D12RootSignature* CreateGraphicsRootSignature(ID3D12Device* pd3dDevice);
	ID3D12RootSignature *GetGraphicsRootSignature();

protected:
	//배치(Batch) 처리를 하기 위하여 씬을 셰이더들의 리스트로 표현한다.
	CInstancingShader* m_pShaders = NULL;
	int m_nShaders = 0;

	CLightsShader* m_pLightShaders = NULL;
	int m_nLightShaders = 0;

	//씬은 게임 객체들의 집합이다. 게임 객체는 셰이더를 포함한다.
	CGameObject **m_ppObjects = NULL;
	int m_nObjects = 0;
	ID3D12RootSignature* m_pd3dGraphicsRootSignature = NULL;


protected:
	CHeightMapTerrain* m_pTerrain = NULL;

public:
	CHeightMapTerrain* GetTerrain() { return(m_pTerrain); }

};