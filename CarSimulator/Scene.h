#include "Shader.h"
#include "Light.h"
class CCamera;

#define RENDER_SKYBOX 0b0000001
#define RENDER_LIGHT 0b0000010
#define RENDER_TERRAIN 0b0000100
#define RENDER_INSTANCING_OBJECT 0b0001000
#define RENDER_BILLBOARD 0b0010000
#define RENDER_ROOM 0b0100000
#define RENDER_PARTICLE 0b1000000

class CScene
{
public:
	CScene();
	~CScene();
	//씬에서 마우스와 키보드 메시지를 처리한다.
	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, btAlignedObjectArray<btCollisionShape*>& btCollisionShapes, btDiscreteDynamicsWorld* pbtDynamicsWorld, ComPtr<ID3D12DescriptorHeap> pd3dSrvDescriptorHeap);
	void ReleaseObjects();
	bool ProcessInput(UCHAR* pKeysBuffer);
	void Update(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, float fTimeElapsed, btDiscreteDynamicsWorld* pbtDynamicsWorld, std::shared_ptr<CVehiclePlayer> pPlayer);
	void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, int nRenderMode);
	float RenderStencilMirror(ID3D12GraphicsCommandList* pd3dCommandList);
	void RenderMirror(ID3D12GraphicsCommandList* pd3dCommandList);
	std::array<const CD3DX12_STATIC_SAMPLER_DESC, 7> GetStaticSamplers();
	void ReleaseUploadBuffers();
	void CreateParticle(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT3 xmf3Position);
	void ReleaseParticle();
	//그래픽 루트 시그너쳐를 생성한다.
	ID3D12RootSignature* CreateGraphicsRootSignature(ID3D12Device* pd3dDevice);
	ID3D12RootSignature *GetGraphicsRootSignature();

protected:
	//배치(Batch) 처리를 하기 위하여 씬을 셰이더들의 리스트로 표현한다.
	CInstancingShader* m_pInstancingShader = NULL;
	CLightsShader* m_pLightShader = NULL;
	CSkyBoxShader* m_pSkyboxShader = NULL;
	CTreeBillBoardShader* m_pBillBoardShader = NULL;
	CAnimatedBillBoardShader* m_pAnimatedBillBoardShader = NULL;
	CMirrorShader* m_pMirrorShader = NULL;
	CRoomShader* m_pRoomShader = NULL;
	CParticleShader* m_pParticleShader = NULL;

	std::shared_ptr<CParticleObject> m_pParticleObject = NULL;

	ID3D12RootSignature* m_pd3dGraphicsRootSignature = NULL;


protected:
	std::shared_ptr <CHeightMapTerrain> m_pTerrain = NULL;

public:
	std::shared_ptr<CHeightMapTerrain> GetTerrain() { return(m_pTerrain); }
	CInstancingShader* GetInstancingShader() { return m_pInstancingShader; }
	CLightsShader* GetLightShader() { return m_pLightShader; }
	std::shared_ptr<CParticleObject> GetParticleObject() { return m_pParticleObject; }
};