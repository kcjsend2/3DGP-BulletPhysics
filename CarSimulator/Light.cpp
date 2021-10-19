#include "stdafx.h"
#include "Light.h"
#include "Shader.h"

CLight::CLight(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, int type, XMFLOAT3 xmf3Strength, float fFalloffStart, XMFLOAT3 xmf3Direction, float fFalloffEnd, XMFLOAT3 xmf3Position, float fSpotPower)
{
	m_itype = type;
	SetPosition(xmf3Position);

	m_xmf3Strength = xmf3Strength;
	m_fFalloffStart = fFalloffStart;
	m_fFalloffEnd = fFalloffEnd;
	m_xmf3Direction = xmf3Direction;
	m_fSpotPower = fSpotPower;

	SetMaterial(XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f), XMFLOAT3(0.6f, 0.6f, 0.6f), 0.3f);

	std::shared_ptr<CMeshFileRead> pMesh = std::make_shared<CMeshFileRead>(pd3dDevice, pd3dCommandList, (char*)"Models/Sphere.bin", false);
	SetMesh(pMesh);
}

CLight::~CLight()
{
}


void CLight::Update(float fTimeElapsed, XMFLOAT3 xmf3PlayerPosition)
{
	if (m_itype == DIRECTIONAL_LIGHT)
	{
		SetPosition(xmf3PlayerPosition.x - 50, 100, xmf3PlayerPosition.z - 50);
	}
}
