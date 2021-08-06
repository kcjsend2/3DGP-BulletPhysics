#include "stdafx.h"
#include "Light.h"
#include "Shader.h"

CLight::CLight(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, int type, XMFLOAT3 xmf3Strength, float fFalloffStart, XMFLOAT3 xmf3Direction, float fFalloffEnd, XMFLOAT3 xmf3Position, XMFLOAT4 xmf4Ambient, float fSpotPower)
{
	m_itype = type;
	SetPosition(xmf3Position);

	m_xmf4Ambient = xmf4Ambient;
	m_xmf3Strength = xmf3Strength;
	m_fFalloffStart = fFalloffStart;
	m_fFalloffEnd = fFalloffEnd;
	m_xmf3Direction = xmf3Direction;
	m_fSpotPower = fSpotPower;

	SetMaterial(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f), XMFLOAT3(0.6f, 0.6f, 0.6f), 0.3f);

	CMeshFileRead* pMesh = new CMeshFileRead(pd3dDevice, pd3dCommandList, "Models/Sphere.bin", false);
	SetMesh(0, pMesh);
}

CLight::~CLight()
{
}

void CLight::SetMesh(int nIndex, CMeshFileRead* pMesh)
{
	if (m_pMesh)
		m_pMesh->Release();

	m_pMesh = pMesh;

	if (pMesh)
		pMesh->AddRef();
}

void CLight::ReleaseUploadBuffers()
{
	if (m_pMesh)
	{
		m_pMesh->ReleaseUploadBuffers();
	}
}


void CLight::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	OnPrepareRender();
	UpdateShaderVariables(pd3dCommandList);
	if (m_pShader) m_pShader->Render(pd3dCommandList, pCamera);

	//게임 객체가 포함하는 모든 메쉬를 렌더링한다.
	if (m_pMesh)
	{
		m_pMesh->Render(pd3dCommandList);
	}
}