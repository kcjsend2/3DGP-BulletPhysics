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

	//지형을 확대할 스케일 벡터이다. x-축과 z-축은 32배, y-축은 8배 확대한다.
	XMFLOAT3 xmf3Scale(10.0f, 1.0f, 10.0f);

	//지형을 높이 맵 이미지 파일(HeightMap.raw)을 사용하여 생성한다. 높이 맵의 크기는 가로x세로(257x257)이다.
	//지형을 하나의 격자 메쉬(257x257)로 생성한다.
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

	//32비트 루트 상수
	pd3dRootParameters[0].InitAsConstants(28, 0, 0, D3D12_SHADER_VISIBILITY_ALL);
	pd3dRootParameters[1].InitAsConstants(19, 1, 0, D3D12_SHADER_VISIBILITY_ALL);
	pd3dRootParameters[2].InitAsConstants(3, 2, 0, D3D12_SHADER_VISIBILITY_ALL);

	//UploadBuffer 클래스를 이용하여 업로드
	pd3dRootParameters[3].InitAsConstantBufferView(3);

	//구조화 된 버퍼, stdafx.h의 CreateBufferResource 함수 이용해 업로드
	pd3dRootParameters[4].InitAsShaderResourceView(0, 1);
	pd3dRootParameters[5].InitAsShaderResourceView(1, 1);

	CD3DX12_DESCRIPTOR_RANGE shadowMapRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 3, 0, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND); // 3개, 0 ~ 2
	CD3DX12_DESCRIPTOR_RANGE texRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 6, 3, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND); // 6개, 3 ~ 8
	CD3DX12_DESCRIPTOR_RANGE TerrainRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 2, 9, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND); // 2개, 9 ~ 10
	CD3DX12_DESCRIPTOR_RANGE SkyboxRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 6, 11, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND); // 6개, 11 ~ 16
	CD3DX12_DESCRIPTOR_RANGE BillboardRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 2, 17, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND); // 2개, 17 ~ 18

	//디스크립터 테이블 이용하여 업로드
	pd3dRootParameters[6].InitAsDescriptorTable(1, &shadowMapRange); //쉐도우 맵
	pd3dRootParameters[7].InitAsDescriptorTable(1, &texRange); //텍스쳐 배열
	pd3dRootParameters[8].InitAsDescriptorTable(1, &TerrainRange); // 베이스 텍스쳐 + 디테일 텍스쳐
	pd3dRootParameters[9].InitAsDescriptorTable(1, &SkyboxRange); // 스카이박스 텍스쳐
	pd3dRootParameters[10].InitAsDescriptorTable(1, &BillboardRange); // 빌보드 텍스쳐

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