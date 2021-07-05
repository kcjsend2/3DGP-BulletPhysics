#pragma once
#include "GameObject.h"

#define SPOT_LIGHT 0x00
#define DIRECTIONAL_LIGHT 0x01
#define POINT_LIGHT 0x02

class CLight : public CGameObject
{
public:
    int m_itype;
    XMFLOAT4 m_xmf4Ambient;
    XMFLOAT4 m_xmf4Diffuse;
    XMFLOAT4 m_xmf4Specular;
    XMFLOAT3 m_xmf3Attenuation;
    float m_fRange;
    XMFLOAT3 m_xmf3Direction;
    CMeshFileRead* m_pMesh = NULL;

public:
    CLight() = default;
    CLight(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, int type, XMFLOAT3 xmf3Position, XMFLOAT4 xmf4Ambient, XMFLOAT4 xmf4Diffuse, XMFLOAT4 xmf4Specular, XMFLOAT3 xmf3Attenuation, float fRange, XMFLOAT3 xmf3Direction);
    ~CLight();
    virtual void SetMesh(int nIndex, CMeshFileRead* pMesh);
    virtual void ReleaseUploadBuffers();
    virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);
};