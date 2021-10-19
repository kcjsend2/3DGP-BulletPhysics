#include "Shader.h"
#include "Light.h"
class CCamera;

class CScene
{
public:
	CScene();
	~CScene();
	//������ ���콺�� Ű���� �޽����� ó���Ѵ�.
	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, btAlignedObjectArray<btCollisionShape*>& btCollisionShapes, btDiscreteDynamicsWorld* pbtDynamicsWorld, ComPtr<ID3D12DescriptorHeap> pd3dSrvDescriptorHeap);
	void ReleaseObjects();
	bool ProcessInput(UCHAR* pKeysBuffer);
	void Update(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, float fTimeElapsed, btDiscreteDynamicsWorld* pbtDynamicsWorld, std::shared_ptr<CVehiclePlayer> pPlayer);
	void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);
	std::array<const CD3DX12_STATIC_SAMPLER_DESC, 7> GetStaticSamplers();
	void ReleaseUploadBuffers();
	//�׷��� ��Ʈ �ñ׳��ĸ� �����Ѵ�.
	ID3D12RootSignature* CreateGraphicsRootSignature(ID3D12Device* pd3dDevice);
	ID3D12RootSignature *GetGraphicsRootSignature();

protected:
	//��ġ(Batch) ó���� �ϱ� ���Ͽ� ���� ���̴����� ����Ʈ�� ǥ���Ѵ�.
	CInstancingShader* m_pInstancingShader = NULL;
	CLightsShader* m_pLightShader = NULL;
	CSkyBoxShader* m_pSkyboxShader = NULL;
	CTreeBillBoardShader* m_pBillBoardShader = NULL;
	CAnimatedBillBoardShader* m_pAnimatedBillBoardShader = NULL;

	ID3D12RootSignature* m_pd3dGraphicsRootSignature = NULL;


protected:
	std::shared_ptr <CHeightMapTerrain> m_pTerrain = NULL;

public:
	std::shared_ptr<CHeightMapTerrain> GetTerrain() { return(m_pTerrain); }
	CInstancingShader* GetInstancingShader() { return m_pInstancingShader; }
	CLightsShader* GetLightShader() { return m_pLightShader; }
};