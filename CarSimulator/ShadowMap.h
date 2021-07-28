#pragma once
#include "stdafx.h"

class CShadowMap
{
public:
	CShadowMap(ID3D12Device* device, UINT width, UINT height);
	~CShadowMap() = default;

	UINT GetWidth()const { return  m_Width; }
	UINT GetHeight()const {	return m_Height; }
	ID3D12Resource* GetResource() { return m_ShadowMap; }
	D3D12_GPU_DESCRIPTOR_HANDLE GetSrv()const { return m_hGpuSrv; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetDsv()const { return m_hCpuDsv; }

	D3D12_VIEWPORT GetViewport()const { return m_Viewport; }
	D3D12_RECT GetScissorRect()const { return m_ScissorRect; }

	void BuildDescriptors(
		D3D12_CPU_DESCRIPTOR_HANDLE hCpuSrv,
		D3D12_GPU_DESCRIPTOR_HANDLE hGpuSrv,
		D3D12_CPU_DESCRIPTOR_HANDLE hCpuDsv);

private:
	void BuildDescriptors();
	void BuildResource();

private:
	ID3D12Device* m_pd3dDevice = NULL;

	D3D12_VIEWPORT m_Viewport;
	D3D12_RECT m_ScissorRect;

	UINT m_Width = 0;
	UINT m_Height = 0;
	DXGI_FORMAT mFormat = DXGI_FORMAT_R24G8_TYPELESS;

	D3D12_CPU_DESCRIPTOR_HANDLE m_hCpuSrv;
	D3D12_GPU_DESCRIPTOR_HANDLE m_hGpuSrv;
	D3D12_CPU_DESCRIPTOR_HANDLE m_hCpuDsv;

	ID3D12Resource* m_ShadowMap = NULL;
};

