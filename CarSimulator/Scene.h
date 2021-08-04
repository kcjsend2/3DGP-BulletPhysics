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
	void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, btAlignedObjectArray<btCollisionShape*>& btCollisionShapes, btDiscreteDynamicsWorld* pbtDynamicsWorld);
	void ReleaseObjects();
	bool ProcessInput(UCHAR* pKeysBuffer);
	void Update(float fTimeElapsed, btDiscreteDynamicsWorld* pbtDynamicsWorld);
	void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);
	std::array<const CD3DX12_STATIC_SAMPLER_DESC, 7> GetStaticSamplers();
	void ReleaseUploadBuffers();
	//�׷��� ��Ʈ �ñ׳��ĸ� �����Ѵ�.
	ID3D12RootSignature* CreateGraphicsRootSignature(ID3D12Device* pd3dDevice);
	ID3D12RootSignature *GetGraphicsRootSignature();

protected:
	//��ġ(Batch) ó���� �ϱ� ���Ͽ� ���� ���̴����� ����Ʈ�� ǥ���Ѵ�.
	CInstancingShader* m_pShaders = NULL;
	int m_nShaders = 0;

	CLightsShader* m_pLightShaders = NULL;
	int m_nLightShaders = 0;

	//���� ���� ��ü���� �����̴�. ���� ��ü�� ���̴��� �����Ѵ�.
	CGameObject **m_ppObjects = NULL;
	int m_nObjects = 0;
	ID3D12RootSignature* m_pd3dGraphicsRootSignature = NULL;


protected:
	CHeightMapTerrain* m_pTerrain = NULL;

public:
	CHeightMapTerrain* GetTerrain() { return(m_pTerrain); }

};