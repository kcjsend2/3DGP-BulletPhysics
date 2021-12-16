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

	m_pInstancingShader->ReleaseShaderVariables();
	m_pInstancingShader->ReleaseObjects();

	if (m_pInstancingShader)
		delete[] m_pInstancingShader;
}

void CScene::Update(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, float fTimeElapsed, btDiscreteDynamicsWorld* pbtDynamicsWorld, std::shared_ptr<CVehiclePlayer> pPlayer)
{
	m_pInstancingShader->Update(fTimeElapsed, pbtDynamicsWorld);
	m_pInstancingShader->UpdateShaderVariables(pd3dCommandList);

	m_pLightShader->Update(fTimeElapsed, pPlayer->GetPosition());
	m_pAnimatedBillBoardShader->UpdateShaderVariables(pd3dCommandList, fTimeElapsed);

	auto pBullet = pPlayer->GetBullet();

	if (pBullet)
	{
		std::vector<std::shared_ptr<CGameObject>>& vpInstancingObjects = m_pInstancingShader[0].GetObjectVector();

		for (auto i = vpInstancingObjects.begin(); i < vpInstancingObjects.end();)
		{
			BoundingOrientedBox bulletBB = pBullet->GetBoudingBox(0);
			BoundingOrientedBox meshBB = vpInstancingObjects[0]->GetBoudingBox(0);

			XMMATRIX xmMatrix;

			xmMatrix = XMLoadFloat4x4(&i->get()->GetWorldTransformMatrix());
			meshBB.Transform(meshBB, xmMatrix);

			xmMatrix = XMLoadFloat4x4(&pBullet->GetWorldTransformMatrix());
			bulletBB.Transform(bulletBB, xmMatrix);

			if (bulletBB.Intersects(meshBB) && vpInstancingObjects.size() > 1)
			{
				if (i == vpInstancingObjects.begin())
				{
					vpInstancingObjects[1]->SetMesh(i->get()->GetMesh(0));
				}

				m_pAnimatedBillBoardShader->AddBillBoard(pd3dDevice, pd3dCommandList, i->get()->GetPosition(), 20, 5, 4, std::vector<float>(20, 0.1f));

				pbtDynamicsWorld->removeRigidBody(i->get()->GetRigidBody());
				i = vpInstancingObjects.erase(i);
				pPlayer->EraseBullet();
				pBullet = NULL;

				break;
			}
			else
			{
				i++;
			}
		}
		for (auto i = vpInstancingObjects.begin(); i < vpInstancingObjects.end(); ++i)
		{
			i->get()->SetInstanceNum(vpInstancingObjects.size());
		}
	}
}

void CScene::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, btAlignedObjectArray<btCollisionShape*>& btCollisionShapes, btDiscreteDynamicsWorld* pbtDynamicsWorld, ComPtr<ID3D12DescriptorHeap> pd3dSrvDescriptorHeap)
{
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);

	m_pLightShader = new CLightsShader;

	m_pLightShader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	m_pLightShader->BuildObjects(pd3dDevice, pd3dCommandList);

	//지형을 확대할 스케일 벡터이다. x-축과 z-축은 32배, y-축은 8배 확대한다.
	XMFLOAT3 xmf3Scale(8.0f, 1.0f, 8.0f);

	//지형을 높이 맵 이미지 파일(HeightMap.raw)을 사용하여 생성한다. 높이 맵의 크기는 가로x세로(257x257)이다.
	//지형을 하나의 격자 메쉬(257x257)로 생성한다.
	m_pTerrain = std::make_shared<CHeightMapTerrain>(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, _T("Assets/Image/Terrain/PlaneMap.raw"), 512, 512, 33, 33, xmf3Scale, btCollisionShapes, pbtDynamicsWorld, pd3dSrvDescriptorHeap);
	//m_pTerrain = std::make_shared<CHeightMapTerrain>(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, _T("Assets/Image/Terrain/HeightMap.raw"), 257, 257, 13, 13, xmf3Scale, btCollisionShapes, pbtDynamicsWorld, pd3dSrvDescriptorHeap);

	m_pInstancingShader = new CInstancingShader;
	m_pInstancingShader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	m_pInstancingShader->BuildObjects(pd3dDevice, pd3dCommandList, m_pTerrain.get(), btCollisionShapes, pbtDynamicsWorld, pd3dSrvDescriptorHeap);

	m_pSkyboxShader = new CSkyBoxShader;
	m_pSkyboxShader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	m_pSkyboxShader->BuildObjects(pd3dDevice, pd3dCommandList, pd3dSrvDescriptorHeap);

	m_pBillBoardShader = new CTreeBillBoardShader;
	m_pBillBoardShader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	m_pBillBoardShader->BuildObjects(pd3dDevice, pd3dCommandList, pd3dSrvDescriptorHeap, m_pTerrain.get());

	m_pAnimatedBillBoardShader = new CAnimatedBillBoardShader;
	m_pAnimatedBillBoardShader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	m_pAnimatedBillBoardShader->BuildObjects(pd3dDevice, pd3dCommandList, pd3dSrvDescriptorHeap);

	m_pMirrorShader = new CMirrorShader;
	m_pMirrorShader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	m_pMirrorShader->BuildObjects(pd3dDevice, pd3dCommandList, pd3dSrvDescriptorHeap);

	m_pRoomShader = new CRoomShader;
	m_pRoomShader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	m_pRoomShader->BuildObjects(pd3dDevice, pd3dCommandList);

	m_pParticleShader = new CParticleShader;
	m_pParticleShader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	m_pParticleShader->BuildObjects(pd3dDevice, pd3dCommandList, pd3dSrvDescriptorHeap);

}

void CScene::ReleaseUploadBuffers()
{
	m_pInstancingShader->ReleaseUploadBuffers();
	
	m_pTerrain->ReleaseUploadBuffers();

	m_pLightShader->ReleaseUploadBuffers();
}

void CScene::CreateParticle(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT3 xmf3Position)
{
	m_pParticleObject = std::make_shared<CParticleObject>(pd3dDevice, pd3dCommandList, xmf3Position, XMFLOAT3{0.0f, 0.0f, 0.0f}, XMFLOAT3{ 0.0f, 0.0f, 0.0f }, XMFLOAT3{ 0.0f, 0.0f, 0.0f }, XMFLOAT2{ 8.0f, 8.0f }, 100.0f, 100, m_pParticleShader);
}

void CScene::ReleaseParticle()
{
	m_pParticleObject = NULL;
}

bool CScene::ProcessInput(UCHAR* pKeysBuffer)
{
	return(false);
}

ID3D12RootSignature* CScene::GetGraphicsRootSignature()
{
	return(m_pd3dGraphicsRootSignature);
}

void CScene::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, int nRenderMode)
{
	pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	pCamera->UpdateShaderVariables(pd3dCommandList);

	if(nRenderMode & RENDER_SKYBOX)
		m_pSkyboxShader->Render(pd3dCommandList, pCamera);

	if (nRenderMode & RENDER_LIGHT)
		m_pLightShader->Render(pd3dCommandList);

	if (m_pTerrain && (nRenderMode & RENDER_TERRAIN))
		m_pTerrain->Render(pd3dCommandList);

	if (nRenderMode & RENDER_INSTANCING_OBJECT)
		m_pInstancingShader->Render(pd3dCommandList);

	if (nRenderMode & RENDER_BILLBOARD)
	{
		m_pBillBoardShader->Render(pd3dCommandList);
		m_pAnimatedBillBoardShader->Render(pd3dCommandList);
	}
	
	if(nRenderMode & RENDER_ROOM)
		m_pRoomShader->Render(pd3dCommandList);

	if (nRenderMode & RENDER_PARTICLE && m_pParticleObject)
		m_pParticleObject->Render(pd3dCommandList);
}

float CScene::RenderStencilMirror(ID3D12GraphicsCommandList* pd3dCommandList)
{
	m_pMirrorShader->StencilRender(pd3dCommandList);

	return m_pMirrorShader->GetMirrorZ();
}

void CScene::RenderMirror(ID3D12GraphicsCommandList* pd3dCommandList)
{
	m_pMirrorShader->Render(pd3dCommandList);
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
	CD3DX12_ROOT_PARAMETER pd3dRootParameters[9];

	//32비트 루트 상수
	pd3dRootParameters[0].InitAsConstants(28, 0, 0, D3D12_SHADER_VISIBILITY_ALL);
	pd3dRootParameters[1].InitAsConstants(19, 1, 0, D3D12_SHADER_VISIBILITY_ALL);
	pd3dRootParameters[2].InitAsConstants(4, 2, 0, D3D12_SHADER_VISIBILITY_ALL);

	//UploadBuffer 클래스를 이용하여 업로드
	pd3dRootParameters[3].InitAsConstantBufferView(3);
	pd3dRootParameters[4].InitAsConstantBufferView(4);

	//구조화 된 버퍼, stdafx.h의 CreateBufferResource 함수 이용해 업로드
	pd3dRootParameters[5].InitAsShaderResourceView(0, 1);
	pd3dRootParameters[6].InitAsShaderResourceView(1, 1);

	CD3DX12_DESCRIPTOR_RANGE shadowMapRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 3, 0, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND); // 3개, 0 ~ 2
	CD3DX12_DESCRIPTOR_RANGE texRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 4, 3, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND); // 4개, 3 ~ 6
	CD3DX12_DESCRIPTOR_RANGE TerrainRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 3, 7, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND); // 3개, 7 ~ 9
	CD3DX12_DESCRIPTOR_RANGE SkyboxRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 6, 10, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND); // 6개, 10 ~ 15
	CD3DX12_DESCRIPTOR_RANGE TreeBillboardRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 2, 16, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND); // 2개, 16 ~ 17
	CD3DX12_DESCRIPTOR_RANGE AnimatedBillboardRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 18, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND); // 1개, 18
	CD3DX12_DESCRIPTOR_RANGE CubeMapRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 19, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND); // 1개, 19
	CD3DX12_DESCRIPTOR_RANGE MirrorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 20, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND); // 1개, 20
	CD3DX12_DESCRIPTOR_RANGE ParticleRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 21, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND); // 1개, 21

	CD3DX12_DESCRIPTOR_RANGE descriptorRanges[9] = { shadowMapRange, texRange, TerrainRange, SkyboxRange, TreeBillboardRange, AnimatedBillboardRange, CubeMapRange, MirrorRange, ParticleRange };

	pd3dRootParameters[7].InitAsDescriptorTable(_countof(descriptorRanges), descriptorRanges);

	pd3dRootParameters[8].InitAsConstants(3, 5, 0, D3D12_SHADER_VISIBILITY_ALL);
	////디스크립터 테이블 이용하여 업로드
	//pd3dRootParameters[7].InitAsDescriptorTable(1, &shadowMapRange); //쉐도우 맵
	//pd3dRootParameters[8].InitAsDescriptorTable(1, &texRange); //텍스쳐 배열
	//pd3dRootParameters[9].InitAsDescriptorTable(1, &TerrainRange); // 베이스 텍스쳐 + 디테일 텍스쳐
	//pd3dRootParameters[10].InitAsDescriptorTable(1, &SkyboxRange); // 스카이박스 텍스쳐
	//pd3dRootParameters[11].InitAsDescriptorTable(1, &TreeBillboardRange); // 빌보드 텍스쳐
	//pd3dRootParameters[12].InitAsDescriptorTable(1, &AnimatedBillboardRange); // 애니메이션 빌보드 텍스쳐
	//pd3dRootParameters[13].InitAsDescriptorTable(1, &CubeMapRange); // 큐브 맵 텍스쳐

	auto staticSamplers = GetStaticSamplers();

	CD3DX12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc(_countof(pd3dRootParameters), pd3dRootParameters, (UINT)staticSamplers.size(), staticSamplers.data(), D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_ALLOW_STREAM_OUTPUT);

	ID3DBlob* pd3dSignatureBlob = NULL;
	ID3DBlob* pd3dErrorBlob = NULL;
	::D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);

	std::string p;
	if (pd3dErrorBlob != NULL)
	{
		auto tmp = (char*)pd3dErrorBlob->GetBufferPointer();
		p = tmp;
		p.c_str();
	}

	auto tmp = pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void**)&pd3dGraphicsRootSignature);

	if (pd3dSignatureBlob)
		pd3dSignatureBlob->Release();

	if (pd3dErrorBlob)
		pd3dErrorBlob->Release();

	return(pd3dGraphicsRootSignature);
}