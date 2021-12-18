#include "stdafx.h"
#include "Shader.h"
#include "GameObject.h"
#include "Camera.h"
CTexture::CTexture(int nTextures, UINT nTextureType, int nSamplers, int nGraphicsSrvRootParameters, int nComputeUavRootParameters, int nComputeSrvRootParameters)
{
	m_nTextureType = nTextureType;

	m_nTextures = nTextures;
	if (m_nTextures > 0)
	{
		m_ppd3dTextures = new ID3D12Resource * [m_nTextures];
		for (int i = 0; i < m_nTextures; i++) m_ppd3dTextures[i] = NULL;
		m_ppd3dTextureUploadBuffers = new ID3D12Resource * [m_nTextures];
		for (int i = 0; i < m_nTextures; i++) m_ppd3dTextureUploadBuffers[i] = NULL;

		m_pd3dSrvGpuDescriptorHandles = new D3D12_GPU_DESCRIPTOR_HANDLE[m_nTextures];
		for (int i = 0; i < m_nTextures; i++) m_pd3dSrvGpuDescriptorHandles[i].ptr = NULL;

		m_pd3dUavGpuDescriptorHandles = new D3D12_GPU_DESCRIPTOR_HANDLE[m_nTextures];
		for (int i = 0; i < m_nTextures; i++) m_pd3dUavGpuDescriptorHandles[i].ptr = NULL;

		m_pnResourceTypes = new UINT[m_nTextures];
		for (int i = 0; i < m_nTextures; i++) m_pnResourceTypes[i] = -1;

		m_pdxgiBufferFormats = new DXGI_FORMAT[m_nTextures];
		for (int i = 0; i < m_nTextures; i++) m_pnResourceTypes[i] = DXGI_FORMAT_UNKNOWN;
		m_pnBufferElements = new int[m_nTextures];
		for (int i = 0; i < m_nTextures; i++) m_pnBufferElements[i] = 0;
	}

	m_nGraphicsSrvRootParameters = nGraphicsSrvRootParameters;
	if (m_nGraphicsSrvRootParameters > 0)
	{
		m_pnGraphicsSrvRootParameterIndices = new int[m_nGraphicsSrvRootParameters];
		for (int i = 0; i < m_nGraphicsSrvRootParameters; i++) m_pnGraphicsSrvRootParameterIndices[i] = -1;
		m_pd3dGraphicsRootParameterSrvGpuDescriptorHandles = new D3D12_GPU_DESCRIPTOR_HANDLE[m_nGraphicsSrvRootParameters];
		for (int i = 0; i < m_nGraphicsSrvRootParameters; i++) m_pd3dGraphicsRootParameterSrvGpuDescriptorHandles[i].ptr = NULL;
	}

	m_nComputeUavRootParameters = nComputeUavRootParameters;
	if (m_nComputeUavRootParameters > 0)
	{
		m_pnComputeUavRootParameterIndices = new int[m_nComputeUavRootParameters];
		for (int i = 0; i < m_nComputeUavRootParameters; i++) m_pnComputeUavRootParameterIndices[i] = -1;
		m_pd3dComputeRootParameterUavGpuDescriptorHandles = new D3D12_GPU_DESCRIPTOR_HANDLE[m_nComputeUavRootParameters];
		for (int i = 0; i < m_nComputeUavRootParameters; i++) m_pd3dComputeRootParameterUavGpuDescriptorHandles[i].ptr = NULL;
	}

	m_nComputeSrvRootParameters = nComputeSrvRootParameters;
	if (m_nComputeSrvRootParameters > 0)
	{
		m_pnComputeSrvRootParameterIndices = new int[m_nComputeSrvRootParameters];
		for (int i = 0; i < m_nComputeSrvRootParameters; i++) m_pnComputeSrvRootParameterIndices[i] = -1;
		m_pd3dComputeRootParameterSrvGpuDescriptorHandles = new D3D12_GPU_DESCRIPTOR_HANDLE[m_nComputeSrvRootParameters];
		for (int i = 0; i < m_nComputeSrvRootParameters; i++) m_pd3dComputeRootParameterSrvGpuDescriptorHandles[i].ptr = NULL;
	}

	m_nSamplers = nSamplers;
	if (m_nSamplers > 0) m_pd3dSamplerGpuDescriptorHandles = new D3D12_GPU_DESCRIPTOR_HANDLE[m_nSamplers];
}

CTexture::~CTexture()
{
	if (m_ppd3dTextures)
	{
		for (int i = 0; i < m_nTextures; i++) if (m_ppd3dTextures[i]) m_ppd3dTextures[i]->Release();
		delete[] m_ppd3dTextures;
	}

	if (m_ppstrTextureNames) delete[] m_ppstrTextureNames;

	if (m_pnResourceTypes) delete[] m_pnResourceTypes;
	if (m_pdxgiBufferFormats) delete[] m_pdxgiBufferFormats;
	if (m_pnBufferElements) delete[] m_pnBufferElements;

	if (m_pnGraphicsSrvRootParameterIndices) delete[] m_pnGraphicsSrvRootParameterIndices;
	if (m_pd3dGraphicsRootParameterSrvGpuDescriptorHandles) delete[] m_pd3dGraphicsRootParameterSrvGpuDescriptorHandles;

	if (m_pnComputeUavRootParameterIndices) delete[] m_pnComputeUavRootParameterIndices;
	if (m_pd3dComputeRootParameterUavGpuDescriptorHandles) delete[] m_pd3dComputeRootParameterUavGpuDescriptorHandles;

	if (m_pnComputeSrvRootParameterIndices) delete[] m_pnComputeSrvRootParameterIndices;
	if (m_pd3dComputeRootParameterSrvGpuDescriptorHandles) delete[] m_pd3dComputeRootParameterSrvGpuDescriptorHandles;

	if (m_pd3dSrvGpuDescriptorHandles) delete[] m_pd3dSrvGpuDescriptorHandles;
	if (m_pd3dUavGpuDescriptorHandles) delete[] m_pd3dUavGpuDescriptorHandles;

	if (m_pd3dSamplerGpuDescriptorHandles) delete[] m_pd3dSamplerGpuDescriptorHandles;
}

void CTexture::SetSrvGpuDescriptorHandle(int nIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSrvGpuDescriptorHandle)
{
	m_pd3dSrvGpuDescriptorHandles[nIndex] = d3dSrvGpuDescriptorHandle;
}

void CTexture::SetUavGpuDescriptorHandle(int nIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dUavGpuDescriptorHandle)
{
	m_pd3dUavGpuDescriptorHandles[nIndex] = d3dUavGpuDescriptorHandle;
}

void CTexture::SetGraphicsSrvRootParameterIndex(int nIndex, int nRootParameterIndex, int nGpuHandleIndex)
{
	m_pnGraphicsSrvRootParameterIndices[nIndex] = nRootParameterIndex;
	m_pd3dGraphicsRootParameterSrvGpuDescriptorHandles[nIndex] = m_pd3dSrvGpuDescriptorHandles[nGpuHandleIndex];
}

void CTexture::SetComputeUavRootParameterIndex(int nIndex, int nRootParameterIndex, int nGpuHandleIndex)
{
	m_pnComputeUavRootParameterIndices[nIndex] = nRootParameterIndex;
	m_pd3dComputeRootParameterUavGpuDescriptorHandles[nIndex] = m_pd3dUavGpuDescriptorHandles[nGpuHandleIndex];
}

void CTexture::SetComputeSrvRootParameterIndex(int nIndex, int nRootParameterIndex, int nGpuHandleIndex)
{
	m_pnComputeSrvRootParameterIndices[nIndex] = nRootParameterIndex;
	m_pd3dComputeRootParameterSrvGpuDescriptorHandles[nIndex] = m_pd3dSrvGpuDescriptorHandles[nGpuHandleIndex];
}

void CTexture::SetSampler(int nIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSamplerGpuDescriptorHandle)
{
	m_pd3dSamplerGpuDescriptorHandles[nIndex] = d3dSamplerGpuDescriptorHandle;
}

void CTexture::UpdateComputeShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	for (int i = 0; i < m_nComputeSrvRootParameters; i++)
	{
		if ((m_pnComputeSrvRootParameterIndices[i] != -1) && (m_pd3dComputeRootParameterSrvGpuDescriptorHandles[i].ptr != NULL)) pd3dCommandList->SetComputeRootDescriptorTable(m_pnComputeSrvRootParameterIndices[i], m_pd3dComputeRootParameterSrvGpuDescriptorHandles[i]);
	}

	for (int i = 0; i < m_nComputeUavRootParameters; i++)
	{
		if ((m_pnComputeUavRootParameterIndices[i] != -1) && (m_pd3dComputeRootParameterUavGpuDescriptorHandles[i].ptr != NULL)) pd3dCommandList->SetComputeRootDescriptorTable(m_pnComputeUavRootParameterIndices[i], m_pd3dComputeRootParameterUavGpuDescriptorHandles[i]);
	}
}

void CTexture::UpdateGraphicsShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	for (int i = 0; i < m_nGraphicsSrvRootParameters; i++)
	{
		if ((m_pnGraphicsSrvRootParameterIndices[i] != -1) && (m_pd3dGraphicsRootParameterSrvGpuDescriptorHandles[i].ptr != NULL)) pd3dCommandList->SetGraphicsRootDescriptorTable(m_pnGraphicsSrvRootParameterIndices[i], m_pd3dGraphicsRootParameterSrvGpuDescriptorHandles[i]);
	}
}

void CTexture::ReleaseShaderVariables()
{
}

void CTexture::ReleaseUploadBuffers()
{
	if (m_ppd3dTextureUploadBuffers)
	{
		for (int i = 0; i < m_nTextures; i++) if (m_ppd3dTextureUploadBuffers[i]) m_ppd3dTextureUploadBuffers[i]->Release();
		delete[] m_ppd3dTextureUploadBuffers;
		m_ppd3dTextureUploadBuffers = NULL;
	}
}

void CTexture::LoadTextureFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, wchar_t* pszFileName, UINT nResourceType, UINT nIndex)
{
	m_pnResourceTypes[nIndex] = nResourceType;
	m_ppd3dTextures[nIndex] = ::CreateTextureResourceFromDDSFile(pd3dDevice, pd3dCommandList, pszFileName, &m_ppd3dTextureUploadBuffers[nIndex], D3D12_RESOURCE_STATE_GENERIC_READ/*D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE*/);
}

ID3D12Resource* CTexture::CreateTexture(ID3D12Device* pd3dDevice, UINT nWidth, UINT nHeight, UINT nElements, UINT nMipLevels, DXGI_FORMAT dxgiFormat, D3D12_RESOURCE_FLAGS d3dResourceFlags, D3D12_RESOURCE_STATES d3dResourceStates, D3D12_CLEAR_VALUE* pd3dClearValue, UINT nResourceType, UINT nIndex)
{
	m_pnResourceTypes[nIndex] = nResourceType;
	m_ppd3dTextures[nIndex] = ::CreateTexture2DResource(pd3dDevice, nWidth, nHeight, nElements, nMipLevels, dxgiFormat, d3dResourceFlags, d3dResourceStates, pd3dClearValue);
	return(m_ppd3dTextures[nIndex]);
}

void CTexture::LoadBuffer(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, void* pData, UINT nElements, UINT nStride, DXGI_FORMAT ndxgiFormat, UINT nIndex)
{
	m_pnResourceTypes[nIndex] = RESOURCE_BUFFER;
	m_pdxgiBufferFormats[nIndex] = ndxgiFormat;
	m_pnBufferElements[nIndex] = nElements;
	m_ppd3dTextures[nIndex] = ::CreateBufferResource(pd3dDevice, pd3dCommandList, pData, nElements * nStride, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_GENERIC_READ, &m_ppd3dTextureUploadBuffers[nIndex]);
}

D3D12_SHADER_RESOURCE_VIEW_DESC CTexture::GetShaderResourceViewDesc(int nIndex)
{
	ID3D12Resource* pShaderResource = GetResource(nIndex);
	D3D12_RESOURCE_DESC d3dResourceDesc = pShaderResource->GetDesc();

	D3D12_SHADER_RESOURCE_VIEW_DESC d3dShaderResourceViewDesc;
	d3dShaderResourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	int nTextureType = GetTextureType(nIndex);
	switch (nTextureType)
	{
	case RESOURCE_TEXTURE2D: //(d3dResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D)(d3dResourceDesc.DepthOrArraySize == 1)
	case RESOURCE_TEXTURE2D_ARRAY: //[]
		d3dShaderResourceViewDesc.Format = d3dResourceDesc.Format;
		d3dShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		d3dShaderResourceViewDesc.Texture2D.MipLevels = 1;// -1;
		d3dShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		d3dShaderResourceViewDesc.Texture2D.PlaneSlice = 0;
		d3dShaderResourceViewDesc.Texture2D.ResourceMinLODClamp = 0.0f;
		break;
	case RESOURCE_TEXTURE2DARRAY: //(d3dResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D)(d3dResourceDesc.DepthOrArraySize != 1)
		d3dShaderResourceViewDesc.Format = d3dResourceDesc.Format;
		d3dShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
		d3dShaderResourceViewDesc.Texture2DArray.MipLevels = -1;
		d3dShaderResourceViewDesc.Texture2DArray.MostDetailedMip = 0;
		d3dShaderResourceViewDesc.Texture2DArray.PlaneSlice = 0;
		d3dShaderResourceViewDesc.Texture2DArray.ResourceMinLODClamp = 0.0f;
		d3dShaderResourceViewDesc.Texture2DArray.FirstArraySlice = 0;
		d3dShaderResourceViewDesc.Texture2DArray.ArraySize = d3dResourceDesc.DepthOrArraySize;
		break;
	case RESOURCE_TEXTURE_CUBE: //(d3dResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D)(d3dResourceDesc.DepthOrArraySize == 6)
		d3dShaderResourceViewDesc.Format = d3dResourceDesc.Format;
		d3dShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		d3dShaderResourceViewDesc.TextureCube.MipLevels = 1;
		d3dShaderResourceViewDesc.TextureCube.MostDetailedMip = 0;
		d3dShaderResourceViewDesc.TextureCube.ResourceMinLODClamp = 0.0f;
		break;
	case RESOURCE_BUFFER: //(d3dResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER)
		d3dShaderResourceViewDesc.Format = m_pdxgiBufferFormats[nIndex];
		d3dShaderResourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		d3dShaderResourceViewDesc.Buffer.FirstElement = 0;
		d3dShaderResourceViewDesc.Buffer.NumElements = m_pnBufferElements[nIndex];
		d3dShaderResourceViewDesc.Buffer.StructureByteStride = 0;
		d3dShaderResourceViewDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
		break;
	}
	return(d3dShaderResourceViewDesc);
}

D3D12_UNORDERED_ACCESS_VIEW_DESC CTexture::GetUnorderedAccessViewDesc(int nIndex)
{
	ID3D12Resource* pShaderResource = GetResource(nIndex);
	D3D12_RESOURCE_DESC d3dResourceDesc = pShaderResource->GetDesc();

	D3D12_UNORDERED_ACCESS_VIEW_DESC d3dUnorderedAccessViewDesc;

	int nTextureType = GetTextureType(nIndex);
	switch (nTextureType)
	{
	case RESOURCE_TEXTURE2D: //(d3dResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D)(d3dResourceDesc.DepthOrArraySize == 1)
	case RESOURCE_TEXTURE2D_ARRAY: //[]
		d3dUnorderedAccessViewDesc.Format = d3dResourceDesc.Format;
		d3dUnorderedAccessViewDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
		d3dUnorderedAccessViewDesc.Texture2D.MipSlice = 0;
		d3dUnorderedAccessViewDesc.Texture2D.PlaneSlice = 0;
		break;
	case RESOURCE_TEXTURE2DARRAY: //(d3dResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D)(d3dResourceDesc.DepthOrArraySize != 1)
		d3dUnorderedAccessViewDesc.Format = d3dResourceDesc.Format;
		d3dUnorderedAccessViewDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
		d3dUnorderedAccessViewDesc.Texture2DArray.MipSlice = 0;
		d3dUnorderedAccessViewDesc.Texture2DArray.FirstArraySlice = 0;
		d3dUnorderedAccessViewDesc.Texture2DArray.ArraySize = d3dResourceDesc.DepthOrArraySize;
		d3dUnorderedAccessViewDesc.Texture2DArray.PlaneSlice = 0;
		break;
	case RESOURCE_BUFFER: //(d3dResourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER)
		d3dUnorderedAccessViewDesc.Format = m_pdxgiBufferFormats[nIndex];
		d3dUnorderedAccessViewDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
		d3dUnorderedAccessViewDesc.Buffer.FirstElement = 0;
		d3dUnorderedAccessViewDesc.Buffer.NumElements = 0;
		d3dUnorderedAccessViewDesc.Buffer.StructureByteStride = 0;
		d3dUnorderedAccessViewDesc.Buffer.CounterOffsetInBytes = 0;
		d3dUnorderedAccessViewDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
		break;
	}
	return(d3dUnorderedAccessViewDesc);
}


CGameObject::CGameObject(int nMeshes)
{
	m_xmf4x4World = Matrix4x4::Identity();
	m_vpMeshes.reserve(nMeshes);
}

CGameObject::~CGameObject()
{
}

void CGameObject::SetShader(CGraphicsShader* pShader)
{
	if (m_pShader)
		m_pShader->Release();

	m_pShader = pShader;

	if (m_pShader)
		m_pShader->AddRef();
}

void CGameObject::SetMesh(std::shared_ptr<CMesh> pMesh)
{
	m_vpMeshes.push_back(pMesh);
}

void CGameObject::SetMesh(std::shared_ptr<CCubeMesh> pMesh)
{
	m_vpMeshes.push_back(pMesh);
}

void CGameObject::Update(float fTimeElapsed, btDiscreteDynamicsWorld* pbtDynamicsWorld)
{
}

void CGameObject::OnPrepareRender()
{

}

void CGameObject::Render(ID3D12GraphicsCommandList* pd3dCommandList)
{
	OnPrepareRender();
	UpdateShaderVariables(pd3dCommandList);
	if (m_pShader)
		m_pShader->Render(pd3dCommandList);

	//게임 객체가 포함하는 모든 메쉬를 렌더링한다.
	for (int i = 0; i < m_vpMeshes.size(); i++)
	{
		if (m_vpMeshes[i]) m_vpMeshes[i]->Render(pd3dCommandList);
	}
}

void CGameObject::ShadowPassRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
	OnPrepareRender();
	UpdateShaderVariables(pd3dCommandList);


	//게임 객체가 포함하는 모든 메쉬를 렌더링한다.
	if (m_vpMeshes.size() > 0 && m_nInstance == 1)
	{
		for (int i = 0; i < m_vpMeshes.size(); i++)
		{
			m_vpMeshes[i]->Render(pd3dCommandList);
		}
	}
	else if (m_vpMeshes.size() > 0 && m_nInstance > 1)
	{
		for (int i = 0; i < m_vpMeshes.size(); i++)
		{
			m_vpMeshes[i]->Render(pd3dCommandList, m_nInstance);
		}
	}
}


void CGameObject::Render(ID3D12GraphicsCommandList* pd3dCommandList, UINT nInstances)
{
	OnPrepareRender();
	for (int i = 0; i < m_vpMeshes.size(); i++)
	{
		if (m_vpMeshes[i]) m_vpMeshes[i]->Render(pd3dCommandList, nInstances);
	}
}

void CGameObject::ReleaseUploadBuffers()
{
	for (int i = 0; i < m_vpMeshes.size(); i++)
	{
		m_vpMeshes[i]->ReleaseUploadBuffers();
	}
}

BoundingOrientedBox CGameObject::GetBoudingBox(int index)
{
	if (m_vpMeshes.size() >= index)
		return m_vpMeshes[index]->GetBoundingBox();

	else
		return BoundingOrientedBox();
}

void CGameObject::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CGameObject::ReleaseShaderVariables()
{
}

void CGameObject::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4World)));

	//객체의 월드 변환 행렬을 루트 상수(32-비트 값)를 통하여 셰이더 변수(상수 버퍼)로 복사한다.
	pd3dCommandList->SetGraphicsRoot32BitConstants(0, 16, &xmf4x4World, 0);
	pd3dCommandList->SetGraphicsRoot32BitConstants(0, 12, &m_material, 16);
}

void CGameObject::Rotate(XMFLOAT3* pxmf3Axis, float fAngle)
{
	XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(pxmf3Axis), XMConvertToRadians(fAngle));
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
}

void CGameObject::SetPosition(float x, float y, float z)
{
	m_xmf4x4World._41 = x;
	m_xmf4x4World._42 = y;
	m_xmf4x4World._43 = z;
}

void CGameObject::SetPosition(XMFLOAT3 xmf3Position)
{
	SetPosition(xmf3Position.x, xmf3Position.y, xmf3Position.z);
}

void CGameObject::SetMaterial(XMFLOAT4 xmf4Ambient, XMFLOAT4 xmf4DiffuseAlbedo, XMFLOAT3 xmf3FresneIR, float fShininess)
{
	m_material.AmbientLight = xmf4Ambient;
	m_material.DiffuseAlbedo = xmf4DiffuseAlbedo;
	m_material.FresnelR0 = xmf3FresneIR;
	m_material.Shininess = fShininess;
}

XMFLOAT3 CGameObject::GetPosition()
{
	return(XMFLOAT3(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43));
}

//게임 객체의 로컬 z-축 벡터를 반환한다.
XMFLOAT3 CGameObject::GetLook()
{
	return(Vector3::Normalize(XMFLOAT3(m_xmf4x4World._31, m_xmf4x4World._32,
		m_xmf4x4World._33)));
}

//게임 객체의 로컬 y-축 벡터를 반환한다.
XMFLOAT3 CGameObject::GetUp()
{
	return(Vector3::Normalize(XMFLOAT3(m_xmf4x4World._21, m_xmf4x4World._22, m_xmf4x4World._23)));
}

//게임 객체의 로컬 x-축 벡터를 반환한다.
XMFLOAT3 CGameObject::GetRight()
{
	return(Vector3::Normalize(XMFLOAT3(m_xmf4x4World._11, m_xmf4x4World._12,
		m_xmf4x4World._13)));
}

//게임 객체를 로컬 x-축 방향으로 이동한다.
void CGameObject::MoveStrafe(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Right = GetRight();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Right, fDistance);
	CGameObject::SetPosition(xmf3Position);
}

//게임 객체를 로컬 y-축 방향으로 이동한다.
void CGameObject::MoveUp(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Up = GetUp();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Up, fDistance);
	CGameObject::SetPosition(xmf3Position);
}
//게임 객체를 로컬 z-축 방향으로 이동한다.
void CGameObject::MoveForward(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Look = GetLook();
	xmf3Position = Vector3::Add(xmf3Position, xmf3Look, fDistance);
	CGameObject::SetPosition(xmf3Position);
}

//게임 객체를 주어진 각도로 회전한다.
void CGameObject::Rotate(float fPitch, float fYaw, float fRoll)
{
	XMMATRIX mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fPitch), XMConvertToRadians(fYaw), XMConvertToRadians(fRoll));
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);
}

CRotatingObject::CRotatingObject(int nMeshes) : CGameObject(nMeshes)
{
	m_xmf3RotationAxis = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_fRotationSpeed = 0.0f;
}

CRotatingObject::~CRotatingObject()
{
}

void CRotatingObject::Update(float fTimeElapsed, btDiscreteDynamicsWorld* pbtDynamicsWorld)
{
	//CGameObject::Rotate(&m_xmf3RotationAxis, m_fRotationSpeed * fTimeElapsed);
	auto CollisionObjectArray = pbtDynamicsWorld->getCollisionObjectArray();
	btScalar* m = new btScalar[16];
	CollisionObjectArray[CollisionObjectArray.findLinearSearch(m_pbtRigidBody)]->getWorldTransform().getOpenGLMatrix(m);
	m_xmf4x4World = Matrix4x4::glMatrixToD3DMatrix(m);
}

CHeightMapTerrain::CHeightMapTerrain(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, LPCTSTR pFileName, int nWidth, int nLength, int nBlockWidth, int nBlockLength, XMFLOAT3 xmf3Scale, btAlignedObjectArray<btCollisionShape*>& btCollisionShapes, btDiscreteDynamicsWorld* pbtDynamicsWorld, ComPtr<ID3D12DescriptorHeap> pd3dSrvDescriptorHeap) : CGameObject((nWidth - 1) / (nBlockWidth - 1) * (nLength - 1) / (nBlockLength - 1))
{
	//지형에 사용할 높이 맵의 가로, 세로의 크기이다.
	m_nWidth = nWidth;
	m_nLength = nLength;


	SetMaterial(XMFLOAT4(0.6f, 0.6f, 0.2f, 0.0f), XMFLOAT4(0.6f, 0.6f, 0.2f, 0.0f), XMFLOAT3(0.3f, 0.3f, 0.3f), 0.1f);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
	/*지형 객체는 격자 메쉬들의 배열로 만들 것이다. nBlockWidth, nBlockLength는 격자 메쉬 하나의 가로, 세로 크
	기이다. cxQuadsPerBlock, czQuadsPerBlock은 격자 메쉬의 가로 방향과 세로 방향 사각형의 개수이다.*/
	int cxQuadsPerBlock = nBlockWidth - 1;
	int czQuadsPerBlock = nBlockLength - 1;

	//xmf3Scale는 지형을 실제로 몇 배 확대할 것인가를 나타낸다.
	m_xmf3Scale = xmf3Scale;

	//지형에 사용할 높이 맵을 생성한다.
	m_pHeightMapImage = new CHeightMapImage(pFileName, nWidth, nLength, xmf3Scale);

	//지형에서 가로 방향, 세로 방향으로 격자 메쉬가 몇 개가 있는 가를 나타낸다.
	long cxBlocks = (m_nWidth - 1) / cxQuadsPerBlock;
	long czBlocks = (m_nLength - 1) / czQuadsPerBlock;

	//지형 전체를 표현하기 위한 격자 메쉬의 개수이다.
	//지형 전체를 표현하기 위한 격자 메쉬에 대한 포인터 배열을 생성한다.

	m_vpMeshes.reserve(cxBlocks * czBlocks + 1);
	
	std::shared_ptr<CHeightMapGridMesh> pHeightMapGridMesh = NULL;
	btHeightfieldTerrainShape* pTerrainShape = NULL;
	float* pfHeightmapData = new float[nLength * nWidth];
	float fMinHeight = FLT_MAX;
	float fMaxHeight = -FLT_MAX;

	for (int z = 0, zStart = 0; z < czBlocks; z++)
	{
		for (int x = 0, xStart = 0; x < cxBlocks; x++)
		{
			//지형의 일부분을 나타내는 격자 메쉬의 시작 위치(좌표)이다.
			xStart = x * (nBlockWidth - 1);
			zStart = z * (nBlockLength - 1);
			//지형의 일부분을 나타내는 격자 메쉬를 생성하여 지형 메쉬에 저장한다.
			pHeightMapGridMesh = std::make_shared<CHeightMapGridMesh>(pd3dDevice, pd3dCommandList, xStart, zStart, nBlockWidth, nBlockLength, fMaxHeight, fMinHeight, xmf3Scale, m_pHeightMapImage);
			SetMesh(pHeightMapGridMesh);
		}
	}

	BYTE* pHeightMapPixels = m_pHeightMapImage->GetHeightMapPixels();

	for (int z = 0, zStart = 0; z < nLength; z++)
	{
		for (int x = 0, xStart = 0; x < nWidth; x++)
		{
			pfHeightmapData[z * nLength + x] = pHeightMapPixels[z * nLength + x];
		}
	}

	pTerrainShape = new btHeightfieldTerrainShape(nWidth, nLength, pfHeightmapData, fMinHeight, fMaxHeight, 1, false);
	pTerrainShape->setLocalScaling(btVector3(xmf3Scale.x, xmf3Scale.y, xmf3Scale.z)); 
	
	//지형을 렌더링하기 위한 셰이더를 생성한다.
	CTerrainShader *pShader = new CTerrainShader();
	pShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	pShader->BuildObjects(pd3dDevice, pd3dCommandList, pd3dSrvDescriptorHeap);
	 
	btTransform btTerrainTransform;
	btTerrainTransform.setIdentity();
	btTerrainTransform.setOrigin(btVector3(nWidth * xmf3Scale.x / 2, (fMaxHeight + fMinHeight) * xmf3Scale.y / 2, nLength * xmf3Scale.z / 2));

	btScalar mass(0.0f);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		pTerrainShape->calculateLocalInertia(mass, localInertia);

	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(btTerrainTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, pTerrainShape, localInertia);
	m_pbtRigidBody = new btRigidBody(rbInfo);
	pbtDynamicsWorld->addRigidBody(m_pbtRigidBody);
	SetShader(pShader);

	m_fTessellatedHeight = new float[nWidth * nLength];

	for (int i = 0; i < nLength; ++i)
	{
		for (int j = 0; j < nWidth; ++j)
		{
			m_fTessellatedHeight[i * nLength + j] = GetTessellatedHeight(j, i);
		}
	}
}
CHeightMapTerrain::~CHeightMapTerrain(void)
{
	if (m_pHeightMapImage)
		delete m_pHeightMapImage;
}

XMFLOAT3 CHeightMapTerrain::GetCubicBezierSum(std::array<XMFLOAT3, 25>& patch, XMFLOAT2 t)
{
	std::array<float, 5> uB, vB;
	float txInv{ 1.0f - t.x };
	uB[0] = txInv * txInv * txInv * txInv;
	uB[1] = 4.0f * t.x * txInv * txInv * txInv;
	uB[2] = 6.0f * t.x * t.x * txInv * txInv;
	uB[3] = 4.0f * t.x * t.x * t.x * txInv;
	uB[4] = t.x * t.x * t.x * t.x;

	float tyInv{ 1.0f - t.y };
	vB[0] = tyInv * tyInv * tyInv * tyInv;
	vB[1] = 4.0f * t.y * tyInv * tyInv * tyInv;
	vB[2] = 6.0f * t.y * t.y * tyInv * tyInv;
	vB[3] = 4.0f * t.y * t.y * t.y * tyInv;
	vB[4] = t.y * t.y * t.y * t.y;

	XMFLOAT3 sum{ 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < 5; ++i)
	{
		XMFLOAT3 subSum{ 0.0f, 0.0f, 0.0f };
		for (int j = 0; j < 5; ++j)
		{
			XMFLOAT3 temp{ Vector3::ScalarProduct(patch[(i * 5) + j], uB[j]) };
			subSum = Vector3::Add(subSum, temp);
		}
		subSum = Vector3::ScalarProduct(subSum, vB[i]);
		sum = Vector3::Add(sum, subSum);
	}
	return sum;

}

float CHeightMapTerrain::GetTessellatedHeight(float fObjectx, float fObjectz)
{
	XMFLOAT3 LB{ GetPosition() };

	// 지형 패치 25개
	std::array<XMFLOAT3, 25> vertices;
	for (int i = 0, z = 4; z >= 0; --z)
	{
		for (int x = 0; x < 5; ++x)
		{
			vertices[i].x = LB.x + (x * m_nWidth / 4 * m_xmf3Scale.x);
			vertices[i].z = LB.z + (z * m_nLength / 4 * m_xmf3Scale.z);
			vertices[i].y = GetHeight(vertices[i].x, vertices[i].z);
			++i;
		}
	}

	// 플레이어의 위치 t
	XMFLOAT2 uv{ (fObjectx - LB.x) / (m_nWidth * m_xmf3Scale.x), 1.0f - ((fObjectz - LB.z) / (m_nLength * m_xmf3Scale.z)) };

	return GetCubicBezierSum(vertices, uv).y;
}

float CHeightMapTerrain::GetTessellatedHeightArray(int x, int z)
{
	return m_fTessellatedHeight[m_nLength * z + x];
}

CSkyBox::CSkyBox(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) : CGameObject(6)
{
	std::shared_ptr<CTexturedRectMesh> pSkyBoxMesh = std::make_shared<CTexturedRectMesh>(pd3dDevice, pd3dCommandList, 10.0f, 10.0f, 0.0f, 0.0f, 0.0f, +5.0f);
	SetMesh(pSkyBoxMesh); // 전

	pSkyBoxMesh = std::make_shared<CTexturedRectMesh>(pd3dDevice, pd3dCommandList, 10.0f, 10.0f, 0.0f, 0.0f, 0.0f, -5.0f);
	SetMesh(pSkyBoxMesh); // 후

	pSkyBoxMesh = std::make_shared<CTexturedRectMesh>(pd3dDevice, pd3dCommandList, 0.0f, 10.0f, 10.0f, -5.0f, 0.0f, 0.0f);
	SetMesh(pSkyBoxMesh); // 좌

	pSkyBoxMesh = std::make_shared<CTexturedRectMesh>(pd3dDevice, pd3dCommandList, 0.0f, 10.0f, 10.0f, +5.0f, 0.0f, 0.0f);
	SetMesh(pSkyBoxMesh); // 우

	pSkyBoxMesh = std::make_shared<CTexturedRectMesh>(pd3dDevice, pd3dCommandList, 10.0f, 0.0f, 10.0f, 0.0f, +5.0f, 0.0f);
	SetMesh(pSkyBoxMesh); // 상

	pSkyBoxMesh = std::make_shared<CTexturedRectMesh>(pd3dDevice, pd3dCommandList, 10.0f, 0.0f, 10.0f, 0.0f, -5.0f, 0.0f);
	SetMesh(pSkyBoxMesh); // 하
}

CSkyBox::~CSkyBox()
{
}

void CSkyBox::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	XMFLOAT3 xmf3CameraPos = pCamera->GetPosition();
	SetPosition(xmf3CameraPos.x, xmf3CameraPos.y, xmf3CameraPos.z);

	UpdateShaderVariables(pd3dCommandList);

	OnPrepareRender();
	for (int i = 0; i < m_vpMeshes.size(); i++)
	{
		pd3dCommandList->SetGraphicsRoot32BitConstants(2, 1, &i, 2);
		if (m_vpMeshes[i]) m_vpMeshes[i]->Render(pd3dCommandList);
	}
}

CBullet::CBullet(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT3 xmf3Position, btVector3 btf3Forward, btDiscreteDynamicsWorld* pbtDynamicsWorld) : CGameObject(1)
{
	SetMaterial(XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f), XMFLOAT3(0.6f, 0.6f, 0.6f), 0.3f);

	std::shared_ptr<CMeshFileRead> pMesh = std::make_shared<CMeshFileRead>(pd3dDevice, pd3dCommandList, (char*)"Models/Sphere.bin", false, XMFLOAT3{0.7f, 0.7f, 0.7f});

	btScalar radius = pMesh->GetBoundingBox().Extents.x * 0.7f;

	btCollisionShape* bulletShape = new btSphereShape(radius);
	SetMesh(pMesh);

	btTransform btBulletTransform;
	btBulletTransform.setIdentity();

	btVector3 btf3BulletPosition = 10 * btf3Forward;

	btBulletTransform.setOrigin(btVector3(xmf3Position.x + btf3BulletPosition.x(), xmf3Position.y + btf3BulletPosition.y(), xmf3Position.z + btf3BulletPosition.z()));
	m_pbtRigidBody = BulletHelper::CreateRigidBody(10.0f, btBulletTransform, bulletShape, pbtDynamicsWorld);
	m_pbtRigidBody->setLinearVelocity(btf3Forward * 300);
}

CBullet::~CBullet()
{
}

void CBullet::Update(float fTimeElapsed, btDiscreteDynamicsWorld* pbtDynamicsWorld)
{
	auto CollisionObjectArray = pbtDynamicsWorld->getCollisionObjectArray();
	btScalar* m = new btScalar[16];
	CollisionObjectArray[CollisionObjectArray.findLinearSearch(m_pbtRigidBody)]->getWorldTransform().getOpenGLMatrix(m);

	m_xmf4x4World = Matrix4x4::glMatrixToD3DMatrix(m);

	m_fTimeRemain -= fTimeElapsed;
}

CAnimatedBillBoard::CAnimatedBillBoard(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT3 xmf3Position , int nxDivided, int nyDivided, std::vector<float> vfFrameTime) : CGameObject(1)
{
	std::shared_ptr<CBillBoardMesh> pMesh = std::make_shared<CBillBoardMesh>(pd3dDevice, pd3dCommandList, &xmf3Position, 20, 20, 1);
	SetMesh(pMesh);
	m_nxDivided = nxDivided;
	m_nyDivided = nyDivided;
	m_vfFrameTime = vfFrameTime;
}

CAnimatedBillBoard::~CAnimatedBillBoard()
{
}

void CAnimatedBillBoard::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList, float fTimeElapesd)
{
	m_vfFrameTime[0] -= fTimeElapesd;
	if (m_vfFrameTime[0] < 0.f)
	{
		m_vfFrameTime.erase(m_vfFrameTime.begin());
		m_nCurrentFrame++;

		m_nx = m_nCurrentFrame % m_nxDivided;
		m_ny = m_nCurrentFrame / m_nxDivided;
	}
}

CParticleObject::CParticleObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT3 xmf3Position, XMFLOAT3 xmf3Velocity, XMFLOAT3 xmf3Acceleration, XMFLOAT3 xmf3Color, XMFLOAT2 xmf2Size, float fLifetime, UINT nMaxParticles, CGraphicsShader* pShader) : CGameObject()
{
	m_vpMeshes.reserve(10);

	std::shared_ptr<CParticleMesh> pMesh = std::make_shared<CParticleMesh>(pd3dDevice, pd3dCommandList, xmf3Position, xmf3Velocity, xmf3Acceleration, xmf3Color, xmf2Size, fLifetime, nMaxParticles);
	SetMesh(pMesh);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	SetShader(pShader);
}

CParticleObject::~CParticleObject()
{
}

void CParticleObject::ReleaseUploadBuffers()
{
	CGameObject::ReleaseUploadBuffers();
}

void CParticleObject::Render(ID3D12GraphicsCommandList* pd3dCommandList)
{
	OnPrepareRender();
	m_pShader->OnPrepareStreamRender(pd3dCommandList);
	UpdateShaderVariables(pd3dCommandList);

	for (int i = 0; i < m_vpMeshes.size(); i++) if (m_vpMeshes[i]) m_vpMeshes[i]->PreRender(pd3dCommandList); //Stream Output
	for (int i = 0; i < m_vpMeshes.size(); i++) if (m_vpMeshes[i]) m_vpMeshes[i]->Render(pd3dCommandList); //Stream Output

	for (int i = 0; i < m_vpMeshes.size(); i++)
	{
		if (m_vpMeshes[i])
		{
			m_vpMeshes[i]->PostRender(pd3dCommandList); //Stream Output
			m_vpMeshes[i]->IncreaseCurrentPipline();
		}
	}

	m_pShader->OnPrepareRender(pd3dCommandList);
	for (int i = 0; i < m_vpMeshes.size(); i++) if (m_vpMeshes[i]) m_vpMeshes[i]->PreRender(pd3dCommandList); //Draw
	for (int i = 0; i < m_vpMeshes.size(); i++)
	{
		if (m_vpMeshes[i])
		{
			m_vpMeshes[i]->Render(pd3dCommandList); //Draw
			m_vpMeshes[i]->ResetCurrentPipline();
		}
	}
}