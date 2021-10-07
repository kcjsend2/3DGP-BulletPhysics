#include "stdafx.h"
#include "Camera.h"
#include "Scene.h"

CScene::CScene()
{
}

CScene::~CScene()
{
}

bool CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return(false);
}

bool CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return(false);
}

void CScene::ReleaseObjects()
{
	if (m_pd3dGraphicsRootSignature)
		m_pd3dGraphicsRootSignature->Release();

	for (int i = 0; i < m_nInstancingShaders; i++)
	{
		m_pInstancingShaders[i].ReleaseShaderVariables();
		m_pInstancingShaders[i].ReleaseObjects();
	}

	if (m_pInstancingShaders)
		delete[] m_pInstancingShaders;
	
	if (m_pTerrain)
		delete m_pTerrain;
}

void CScene::Update(ID3D12GraphicsCommandList* pd3dCommandList, float fTimeElapsed, btDiscreteDynamicsWorld* pbtDynamicsWorld, XMFLOAT3 xmf3PlayerPosition)
{
	for (int i = 0; i < m_nInstancingShaders; i++)
	{
		m_pInstancingShaders[i].Update(fTimeElapsed, pbtDynamicsWorld);
		m_pInstancingShaders[i].UpdateShaderVariables(pd3dCommandList);
	}

	for (int i = 0; i < m_nLightShaders; i++)
	{
		m_pLightShaders[i].Update(fTimeElapsed, xmf3PlayerPosition);
	}
}

void CScene::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, btAlignedObjectArray<btCollisionShape*>& btCollisionShapes, btDiscreteDynamicsWorld* pbtDynamicsWorld, ComPtr<ID3D12DescriptorHeap> pd3dSrvDescriptorHeap)
{
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);

	m_nLightShaders = 1;
	m_pLightShaders = new CLightsShader[m_nLightShaders];

	m_pLightShaders[0].CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	m_pLightShaders[0].BuildObjects(pd3dDevice, pd3dCommandList);

	//������ Ȯ���� ������ �����̴�. x-��� z-���� 32��, y-���� 8�� Ȯ���Ѵ�.
	XMFLOAT3 xmf3Scale(10.0f, 1.0f, 10.0f);

	//������ ���� �� �̹��� ����(HeightMap.raw)�� ����Ͽ� �����Ѵ�. ���� ���� ũ��� ����x����(257x257)�̴�.
	//������ �ϳ��� ���� �޽�(257x257)�� �����Ѵ�.
	m_pTerrain = new CHeightMapTerrain(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, _T("Assets/Image/Terrain/PlaneMap.raw"), 512, 512, 512, 512, xmf3Scale, btCollisionShapes, pbtDynamicsWorld, pd3dSrvDescriptorHeap);
	//m_pTerrain = new CHeightMapTerrain(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, _T("Assets/Image/Terrain/HeightMap.raw"), 217, 217, 217, 217, xmf3Scale, btCollisionShapes, pbtDynamicsWorld);

	m_nInstancingShaders = 1;
	m_pInstancingShaders = new CInstancingShader[m_nInstancingShaders];
	m_pInstancingShaders[0].CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	m_pInstancingShaders[0].BuildObjects(pd3dDevice, pd3dCommandList, m_pTerrain, btCollisionShapes, pbtDynamicsWorld, pd3dSrvDescriptorHeap);

	m_nSkyboxShaders = 1;
	m_pSkyboxShaders = new CSkyBoxShader[m_nSkyboxShaders];
	m_pSkyboxShaders[0].CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	m_pSkyboxShaders[0].BuildObjects(pd3dDevice, pd3dCommandList, pd3dSrvDescriptorHeap);

	m_nBillBoardShaders = 1;
	m_pBillBoardShaders = new CBillBoardShader[m_nBillBoardShaders];
	m_pBillBoardShaders[0].CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	m_pBillBoardShaders[0].BuildObjects(pd3dDevice, pd3dCommandList, pd3dSrvDescriptorHeap);
}

void CScene::ReleaseUploadBuffers()
{
	for (int i = 0; i < m_nInstancingShaders; i++)
		m_pInstancingShaders[i].ReleaseUploadBuffers();
	
	if (m_pTerrain)
		m_pTerrain->ReleaseUploadBuffers();

	for(int i = 0; i < m_nLightShaders; i++)
		m_pLightShaders[i].ReleaseUploadBuffers();
}

bool CScene::ProcessInput(UCHAR* pKeysBuffer)
{
	return(false);
}

ID3D12RootSignature* CScene::GetGraphicsRootSignature()
{
	return(m_pd3dGraphicsRootSignature);
}

void CScene::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	pCamera->UpdateShaderVariables(pd3dCommandList);

	for (int i = 0; i < m_nSkyboxShaders; i++)
	{
		m_pSkyboxShaders[i].Render(pd3dCommandList, pCamera);
	}

	for (int i = 0; i < m_nLightShaders; ++i)
	{
		m_pLightShaders[i].Render(pd3dCommandList);
	}

	if (m_pTerrain)
	{
		m_pTerrain->Render(pd3dCommandList);
	}

	for (int i = 0; i < m_nInstancingShaders; i++)
	{
		m_pInstancingShaders[i].Render(pd3dCommandList);
	}

	for (int i = 0; i < m_nBillBoardShaders; i++)
	{
		m_pBillBoardShaders[i].Render(pd3dCommandList);
	}

	
}

std::array<const CD3DX12_STATIC_SAMPLER_DESC, 7> CScene::GetStaticSamplers()
{
	// Applications usually only need a handful of samplers.  So just define them all up front
	// and keep them available as part of the root signature.  

	const CD3DX12_STATIC_SAMPLER_DESC pointWrap(
		0, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC pointClamp(
		1, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC linearWrap(
		2, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC linearClamp(
		3, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC anisotropicWrap(
		4, // shaderRegister
		D3D12_FILTER_ANISOTROPIC, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressW
		0.0f,                             // mipLODBias
		8);                               // maxAnisotropy

	const CD3DX12_STATIC_SAMPLER_DESC anisotropicClamp(
		5, // shaderRegister
		D3D12_FILTER_ANISOTROPIC, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressW
		0.0f,                              // mipLODBias
		8);                                // maxAnisotropy

	const CD3DX12_STATIC_SAMPLER_DESC shadow(
		6, // shaderRegister
		D3D12_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_BORDER,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_BORDER,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_BORDER,  // addressW
		0.0f,                               // mipLODBias
		16,                                 // maxAnisotropy
		D3D12_COMPARISON_FUNC_LESS_EQUAL,
		D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK);

	return {
		pointWrap, pointClamp,
		linearWrap, linearClamp,
		anisotropicWrap, anisotropicClamp,
		shadow
	};
}

ID3D12RootSignature* CScene::CreateGraphicsRootSignature(ID3D12Device* pd3dDevice)
{
	ID3D12RootSignature* pd3dGraphicsRootSignature = NULL;
	CD3DX12_ROOT_PARAMETER pd3dRootParameters[11];

	//32��Ʈ ��Ʈ ���
	pd3dRootParameters[0].InitAsConstants(28, 0, 0, D3D12_SHADER_VISIBILITY_ALL);
	pd3dRootParameters[1].InitAsConstants(19, 1, 0, D3D12_SHADER_VISIBILITY_ALL);
	pd3dRootParameters[2].InitAsConstants(3, 2, 0, D3D12_SHADER_VISIBILITY_ALL);

	//UploadBuffer Ŭ������ �̿��Ͽ� ���ε�
	pd3dRootParameters[3].InitAsConstantBufferView(3);

	//����ȭ �� ����, stdafx.h�� CreateBufferResource �Լ� �̿��� ���ε�
	pd3dRootParameters[4].InitAsShaderResourceView(0, 1);
	pd3dRootParameters[5].InitAsShaderResourceView(1, 1);

	CD3DX12_DESCRIPTOR_RANGE shadowMapRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 3, 0, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND); // 3��, 0 ~ 2
	CD3DX12_DESCRIPTOR_RANGE texRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 6, 3, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND); // 6��, 3 ~ 8
	CD3DX12_DESCRIPTOR_RANGE TerrainRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 2, 9, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND); // 2��, 9 ~ 10
	CD3DX12_DESCRIPTOR_RANGE SkyboxRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 6, 11, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND); // 6��, 11 ~ 16
	CD3DX12_DESCRIPTOR_RANGE BillboardRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 2, 17, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND); // 2��, 17 ~ 18

	//��ũ���� ���̺� �̿��Ͽ� ���ε�
	pd3dRootParameters[6].InitAsDescriptorTable(1, &shadowMapRange); //������ ��
	pd3dRootParameters[7].InitAsDescriptorTable(1, &texRange); //�ؽ��� �迭
	pd3dRootParameters[8].InitAsDescriptorTable(1, &TerrainRange); // ���̽� �ؽ��� + ������ �ؽ���
	pd3dRootParameters[9].InitAsDescriptorTable(1, &SkyboxRange); // ��ī�̹ڽ� �ؽ���
	pd3dRootParameters[10].InitAsDescriptorTable(1, &BillboardRange); // ������ �ؽ���

	auto staticSamplers = GetStaticSamplers();

	CD3DX12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc(_countof(pd3dRootParameters), pd3dRootParameters, (UINT)staticSamplers.size(), staticSamplers.data(), D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ID3DBlob* pd3dSignatureBlob = NULL;
	ID3DBlob* pd3dErrorBlob = NULL;
	::D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);

	if (pd3dErrorBlob != NULL)
	{
		auto tmp = (char*)pd3dErrorBlob->GetBufferPointer();
		std::string p = tmp;
		p.c_str();
	}

	pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void**)&pd3dGraphicsRootSignature);

	if (pd3dSignatureBlob)
		pd3dSignatureBlob->Release();

	if (pd3dErrorBlob)
		pd3dErrorBlob->Release();

	return(pd3dGraphicsRootSignature);
}