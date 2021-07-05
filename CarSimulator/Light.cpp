#include "stdafx.h"
#include "Light.h"
#include "Shader.h"

CLight::CLight(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, int type, XMFLOAT3 xmf3Position, XMFLOAT4 xmf4Ambient, XMFLOAT4 xmf4Diffuse, XMFLOAT4 xmf4Specular, XMFLOAT3 xmf3Attenuation, float fRange, XMFLOAT3 xmf3Direction) : CGameObject(1)
{
	m_itype = type;
	SetPosition(xmf3Position);
	m_xmf4Ambient = xmf4Ambient;
	m_xmf4Diffuse = xmf4Diffuse;
	m_xmf4Specular = xmf4Specular;
	m_xmf3Attenuation = xmf3Attenuation;
	m_fRange = fRange;
	m_xmf3Direction = xmf3Direction;

	m_material = MATERIAL{ XMFLOAT4{1.0f, 1.0f, 1.0, 1.0f}, XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}, XMFLOAT4{1.0f, 1.0f, 1.0, 1.0f}, XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}, 20, XMFLOAT3(1.0f, 0.0f, 0.0f) };

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