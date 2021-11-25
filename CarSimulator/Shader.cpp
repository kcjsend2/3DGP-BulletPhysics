#include "stdafx.h"
#include "Shader.h"
#include "GameObject.h"

CShader::CShader()
{
	
}

CShader::~CShader()
{
	if (m_pd3dPipelineState)
	{
		if (m_pd3dPipelineState)
			m_pd3dPipelineState->Release();
	}
}

//래스터라이저 상태를 설정하기 위한 구조체를 반환한다.
D3D12_RASTERIZER_DESC CShader::CreateRasterizerState()
{
	D3D12_RASTERIZER_DESC d3dRasterizerDesc;
	::ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
	d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	d3dRasterizerDesc.FrontCounterClockwise = FALSE;
	d3dRasterizerDesc.DepthBias = 0;
	d3dRasterizerDesc.DepthBiasClamp = 0.0f;
	d3dRasterizerDesc.SlopeScaledDepthBias = 0.0f;
	d3dRasterizerDesc.DepthClipEnable = TRUE;
	d3dRasterizerDesc.MultisampleEnable = FALSE;
	d3dRasterizerDesc.AntialiasedLineEnable = FALSE;
	d3dRasterizerDesc.ForcedSampleCount = 0;
	d3dRasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
	return(d3dRasterizerDesc);
}

//깊이-스텐실 검사를 위한 상태를 설정하기 위한 구조체를 반환한다.
D3D12_DEPTH_STENCIL_DESC CShader::CreateDepthStencilState()
{
	D3D12_DEPTH_STENCIL_DESC d3dDepthStencilDesc;
	::ZeroMemory(&d3dDepthStencilDesc, sizeof(D3D12_DEPTH_STENCIL_DESC));
	d3dDepthStencilDesc.DepthEnable = TRUE;
	d3dDepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	d3dDepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	d3dDepthStencilDesc.StencilEnable = FALSE;
	d3dDepthStencilDesc.StencilReadMask = 0x00;
	d3dDepthStencilDesc.StencilWriteMask = 0x00;
	d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
	d3dDepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
	return(d3dDepthStencilDesc);
}

//블렌딩 상태를 설정하기 위한 구조체를 반환한다.
D3D12_BLEND_DESC CShader::CreateBlendState()
{
	D3D12_BLEND_DESC d3dBlendDesc;
	::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
	d3dBlendDesc.AlphaToCoverageEnable = FALSE;
	d3dBlendDesc.IndependentBlendEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].BlendEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].LogicOpEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
	d3dBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
	d3dBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	d3dBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	d3dBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	return(d3dBlendDesc);
}

//입력 조립기에게 정점 버퍼의 구조를 알려주기 위한 구조체를 반환한다.
D3D12_INPUT_LAYOUT_DESC CShader::CreateInputLayout()
{
	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = NULL;
	d3dInputLayoutDesc.NumElements = 0;
	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CShader::CreateVertexShader(ID3DBlob** ppd3dShaderBlob)
{
	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = 0;
	d3dShaderByteCode.pShaderBytecode = NULL;
	return(d3dShaderByteCode);
}

D3D12_SHADER_BYTECODE CShader::CreatePixelShader(ID3DBlob** ppd3dShaderBlob)
{
	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = 0;
	d3dShaderByteCode.pShaderBytecode = NULL;
	return(d3dShaderByteCode);
}
D3D12_SHADER_BYTECODE CShader::CreateGeometryShader(ID3DBlob** ppd3dShaderBlob)
{
	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = 0;
	d3dShaderByteCode.pShaderBytecode = NULL;
	return(d3dShaderByteCode);
}
D3D12_SHADER_BYTECODE CShader::CreateHullShader(ID3DBlob** ppd3dShaderBlob)
{
	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = 0;
	d3dShaderByteCode.pShaderBytecode = NULL;
	return(d3dShaderByteCode);
}
D3D12_SHADER_BYTECODE CShader::CreateDomainShader(ID3DBlob** ppd3dShaderBlob)
{
	D3D12_SHADER_BYTECODE d3dShaderByteCode;
	d3dShaderByteCode.BytecodeLength = 0;
	d3dShaderByteCode.pShaderBytecode = NULL;
	return(d3dShaderByteCode);
}
//셰이더 소스 코드를 컴파일하여 바이트 코드 구조체를 반환한다.
D3D12_SHADER_BYTECODE CShader::CompileShaderFromFile(WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderProfile, ID3DBlob** ppd3dShaderBlob)
{
	UINT nCompileFlags = 0;
	ComPtr<ID3DBlob> errors;

	#if defined(_DEBUG)
	nCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	#endif

	auto tmp = ::D3DCompileFromFile(pszFileName, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, pszShaderName, pszShaderProfile, nCompileFlags, 0, ppd3dShaderBlob, &errors);
	D3D12_SHADER_BYTECODE d3dShaderByteCode;

	std::string s;
	if (errors != nullptr)
	{
		s = (char*)errors->GetBufferPointer();
		s.c_str();
	}

	d3dShaderByteCode.BytecodeLength = (*ppd3dShaderBlob)->GetBufferSize();
	d3dShaderByteCode.pShaderBytecode = (*ppd3dShaderBlob)->GetBufferPointer();
	return(d3dShaderByteCode);
}

//그래픽스 파이프라인 상태 객체를 생성한다.
void CShader::CreateShader(ID3D12Device *pd3dDevice, ID3D12RootSignature *pd3dGraphicsRootSignature)
{
	ID3DBlob* pd3dVertexShaderBlob = NULL, * pd3dPixelShaderBlob = NULL, * pd3dGeometryShaderBlob = NULL;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc;
	::ZeroMemory(&d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	d3dPipelineStateDesc.pRootSignature = pd3dGraphicsRootSignature;
	d3dPipelineStateDesc.VS = CreateVertexShader(&pd3dVertexShaderBlob);
	d3dPipelineStateDesc.PS = CreatePixelShader(&pd3dPixelShaderBlob);
	d3dPipelineStateDesc.GS = CreateGeometryShader(&pd3dGeometryShaderBlob);
	d3dPipelineStateDesc.HS = CreateHullShader(&pd3dGeometryShaderBlob);
	d3dPipelineStateDesc.DS = CreateDomainShader(&pd3dGeometryShaderBlob);
	d3dPipelineStateDesc.RasterizerState = CreateRasterizerState();
	d3dPipelineStateDesc.BlendState = CreateBlendState();
	d3dPipelineStateDesc.DepthStencilState = CreateDepthStencilState();
	d3dPipelineStateDesc.InputLayout = CreateInputLayout();
	d3dPipelineStateDesc.SampleMask = UINT_MAX;
	d3dPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	d3dPipelineStateDesc.NumRenderTargets = 1;
	d3dPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dPipelineStateDesc.SampleDesc.Count = 1;
	d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	auto tmp = pd3dDevice->CreateGraphicsPipelineState(&d3dPipelineStateDesc, __uuidof(ID3D12PipelineState), (void**)&m_pd3dPipelineState);

	if (pd3dVertexShaderBlob)
		pd3dVertexShaderBlob->Release();

	if (pd3dPixelShaderBlob)
		pd3dPixelShaderBlob->Release();

	if (pd3dGeometryShaderBlob)
		pd3dGeometryShaderBlob->Release();

	if (d3dPipelineStateDesc.InputLayout.pInputElementDescs)
		delete[] d3dPipelineStateDesc.InputLayout.pInputElementDescs;
}

void CShader::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{

}

void CShader::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{

}

void CShader::UpdateShaderVariable(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT4X4* pxmf4x4World)
{
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(pxmf4x4World)));

	pd3dCommandList->SetGraphicsRoot32BitConstants(0, 16, &xmf4x4World, 0);
}

void CShader::ReleaseShaderVariables()
{

}

void CShader::OnPrepareRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
	//파이프라인에 그래픽스 상태 객체를 설정한다.
	pd3dCommandList->SetPipelineState(m_pd3dPipelineState);
}

void CShader::Render(ID3D12GraphicsCommandList* pd3dCommandList)
{
	OnPrepareRender(pd3dCommandList);
}

void CShader::Update(float fTimeElapsed)
{
}

void CShader::CreateShaderResourceViews(ID3D12Device* pd3dDevice, std::shared_ptr<CTexture> pTexture, UINT nDescriptorHeapIndex, UINT nRootParameterStartIndex)
{
	UINT nSrvDescriptorIncrementSize = pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	m_d3dSrvCPUDescriptorHandle.ptr += (nSrvDescriptorIncrementSize * nDescriptorHeapIndex);
	m_d3dSrvGPUDescriptorHandle.ptr += (nSrvDescriptorIncrementSize * nDescriptorHeapIndex);

	int nTextures = pTexture->GetTextures();
	for (int i = 0; i < nTextures; i++)
	{
		ID3D12Resource* pShaderResource = pTexture->GetResource(i);
		D3D12_SHADER_RESOURCE_VIEW_DESC d3dShaderResourceViewDesc = pTexture->GetShaderResourceViewDesc(i);
		pd3dDevice->CreateShaderResourceView(pShaderResource, &d3dShaderResourceViewDesc, m_d3dSrvCPUDescriptorHandle);
		m_d3dSrvCPUDescriptorHandle.ptr += nSrvDescriptorIncrementSize;
		pTexture->SetGpuDescriptorHandle(i, m_d3dSrvGPUDescriptorHandle);
		m_d3dSrvGPUDescriptorHandle.ptr += nSrvDescriptorIncrementSize;
	}
	int nRootParameters = pTexture->GetRootParameters();
	for (int i = 0; i < nRootParameters; i++) pTexture->SetRootParameterIndex(i, nRootParameterStartIndex + i);
}

void CShader::SetSrvDescriptorHeapHandle(ComPtr<ID3D12DescriptorHeap> pd3dSrvDescriptorHeap)
{
	m_d3dSrvCPUDescriptorHandle = pd3dSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	m_d3dSrvGPUDescriptorHandle = pd3dSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
}

CPlayerShader::CPlayerShader()
{
}

CPlayerShader::~CPlayerShader()
{
}

void CPlayerShader::CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	ID3DBlob* pd3dVertexShaderBlob = NULL, * pd3dPixelShaderBlob = NULL, * pd3dGeometryShaderBlob = NULL;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc;
	::ZeroMemory(&d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	d3dPipelineStateDesc.pRootSignature = pd3dGraphicsRootSignature;
	d3dPipelineStateDesc.VS = CreateVertexShader(&pd3dVertexShaderBlob);
	d3dPipelineStateDesc.PS = CreatePixelShader(&pd3dPixelShaderBlob);
	d3dPipelineStateDesc.GS = CreateGeometryShader(&pd3dGeometryShaderBlob);
	d3dPipelineStateDesc.HS = CreateHullShader(&pd3dGeometryShaderBlob);
	d3dPipelineStateDesc.DS = CreateDomainShader(&pd3dGeometryShaderBlob);
	d3dPipelineStateDesc.RasterizerState = CreateRasterizerState();
	d3dPipelineStateDesc.BlendState = CreateBlendState();
	d3dPipelineStateDesc.DepthStencilState = CreateDepthStencilState();
	d3dPipelineStateDesc.InputLayout = CreateInputLayout();
	d3dPipelineStateDesc.SampleMask = UINT_MAX;
	d3dPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	d3dPipelineStateDesc.NumRenderTargets = 1;
	d3dPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dPipelineStateDesc.SampleDesc.Count = 1;
	d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	pd3dDevice->CreateGraphicsPipelineState(&d3dPipelineStateDesc, __uuidof(ID3D12PipelineState), (void**)&m_pd3dPipelineState);

	d3dPipelineStateDesc.DepthStencilState.DepthEnable = TRUE;
	d3dPipelineStateDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	d3dPipelineStateDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	d3dPipelineStateDesc.DepthStencilState.StencilEnable = TRUE;
	d3dPipelineStateDesc.DepthStencilState.StencilReadMask = 0xff;
	d3dPipelineStateDesc.DepthStencilState.StencilWriteMask = 0xff;

	d3dPipelineStateDesc.DepthStencilState.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dPipelineStateDesc.DepthStencilState.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	d3dPipelineStateDesc.DepthStencilState.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dPipelineStateDesc.DepthStencilState.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_EQUAL;

	d3dPipelineStateDesc.DepthStencilState.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dPipelineStateDesc.DepthStencilState.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	d3dPipelineStateDesc.DepthStencilState.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dPipelineStateDesc.DepthStencilState.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_EQUAL;

	d3dPipelineStateDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
	d3dPipelineStateDesc.RasterizerState.FrontCounterClockwise = TRUE;

	pd3dDevice->CreateGraphicsPipelineState(&d3dPipelineStateDesc, __uuidof(ID3D12PipelineState), (void**)&m_pd3dReflectedPipeline);

	if (pd3dVertexShaderBlob)
		pd3dVertexShaderBlob->Release();

	if (pd3dPixelShaderBlob)
		pd3dPixelShaderBlob->Release();

	if (pd3dGeometryShaderBlob)
		pd3dGeometryShaderBlob->Release();

	if (d3dPipelineStateDesc.InputLayout.pInputElementDescs)
		delete[] d3dPipelineStateDesc.InputLayout.pInputElementDescs;
}

void CPlayerShader::ReflectedRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
	pd3dCommandList->SetPipelineState(m_pd3dReflectedPipeline);
}

D3D12_INPUT_LAYOUT_DESC CPlayerShader::CreateInputLayout()
{
	UINT nInputElementDescs = 3;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];

	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[2] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 2, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CPlayerShader::CreateVertexShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"CubemapPlayer.hlsl", "VS_CMPlayer", "vs_5_1", ppd3dShaderBlob));
}
D3D12_SHADER_BYTECODE CPlayerShader::CreatePixelShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"CubemapPlayer.hlsl", "PS_CMPlayer", "ps_5_1", ppd3dShaderBlob));
}

CObjectsShader::CObjectsShader()
{
}

CObjectsShader::~CObjectsShader()
{
}

D3D12_INPUT_LAYOUT_DESC CObjectsShader::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];
	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;
	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CObjectsShader::CreateVertexShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Default.hlsl", "VS_Default", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CObjectsShader::CreatePixelShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Default.hlsl", "PS_Default", "ps_5_1", ppd3dShaderBlob));
}

void CObjectsShader::CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
}

void CObjectsShader::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, void* pContext)
{
}
void CObjectsShader::ReleaseObjects()
{
}

void CObjectsShader::Update(float fTimeElapsed, btDiscreteDynamicsWorld* pbtDynamicsWorld)
{
	for (int i = 0; i < m_vpObjects.size(); i++)
	{
		m_vpObjects[i]->Update(fTimeElapsed, pbtDynamicsWorld);
	}
}

void CObjectsShader::ReleaseUploadBuffers()
{
	for (int i = 0; i < m_vpObjects.size(); i++)
		m_vpObjects[i]->ReleaseUploadBuffers();
}

void CObjectsShader::Render(ID3D12GraphicsCommandList* pd3dCommandList)
{
	CShader::Render(pd3dCommandList);

	for (int i = 0; i < m_vpObjects.size(); i++)
	{
		m_vpObjects[i]->Render(pd3dCommandList);
	}
	UpdateShaderVariables(pd3dCommandList);
}

CInstancingShader::CInstancingShader()
{

}

CInstancingShader::~CInstancingShader()
{

}

D3D12_INPUT_LAYOUT_DESC CInstancingShader::CreateInputLayout()
{
	UINT nInputElementDescs = 3;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];
	//정점 정보를 위한 입력 원소이다.
	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[2] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CInstancingShader::CreateVertexShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Instancing.hlsl", "VS_Instancing", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CInstancingShader::CreatePixelShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Instancing.hlsl", "PS_Instancing", "ps_5_1", ppd3dShaderBlob));
}

void CInstancingShader::CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
}

void CInstancingShader::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	//인스턴스 정보를 저장할 정점 버퍼를 업로드 힙 유형으로 생성한다.
	m_pd3dcbGameObjects = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, sizeof(VS_VB_INSTANCE) * m_vpObjects.size(), D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ, NULL);

	//정점 버퍼(업로드 힙)에 대한 포인터를 저장한다.
	m_pd3dcbGameObjects->Map(0, NULL, (void**)&m_pcbMappedGameObjects);
}

void CInstancingShader::ReleaseShaderVariables()
{
	if (m_pd3dcbGameObjects)
		m_pd3dcbGameObjects->Unmap(0, NULL);

	if (m_pd3dcbGameObjects)
		m_pd3dcbGameObjects->Release();
}

void CInstancingShader::ReleaseUploadBuffers()
{
	m_vpObjects[0]->ReleaseUploadBuffers();
}

//인스턴싱 정보(객체의 월드 변환 행렬과 색상)를 정점 버퍼에 복사한다.
void CInstancingShader::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	pd3dCommandList->SetGraphicsRootShaderResourceView(5, m_pd3dcbGameObjects->GetGPUVirtualAddress());
	for (int i = 0; i < m_vpObjects.size(); i++)
	{
		XMStoreFloat4x4(&m_pcbMappedGameObjects[i].m_xmf4x4Transform, XMMatrixTranspose(XMLoadFloat4x4(&m_vpObjects[i]->GetWorldTransformMatrix())));
		m_pcbMappedGameObjects[i].m_nTextrueIndex = m_vpObjects[i]->GetTextureIndex();
		m_vpObjects[i]->UpdateShaderVariables(pd3dCommandList);
	}
}

void CInstancingShader::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, void* pContext, btAlignedObjectArray<btCollisionShape*>& btCollisionShapes, btDiscreteDynamicsWorld* pbtDynamicsWorld, ComPtr<ID3D12DescriptorHeap> pd3dSrvDescriptorHeap)
{
	float fHeight = 10.0f, fLength = 10.0f, fDepth = 10.0f;

	UINT nSrvDescriptorIncrementSize = pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	m_d3dSrvCPUDescriptorHandle = pd3dSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	m_d3dSrvGPUDescriptorHandle = pd3dSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();

	m_pTexture = std::make_shared<CTexture>(4, RESOURCE_TEXTURE2D_ARRAY, 0, 1);

	m_pTexture->LoadTextureFromDDSFile(pd3dDevice, pd3dCommandList, L"Assets/Image/Textures/Stone01.dds", RESOURCE_TEXTURE2D_ARRAY, 0);
	m_pTexture->LoadTextureFromDDSFile(pd3dDevice, pd3dCommandList, L"Assets/Image/Textures/Metal01.dds", RESOURCE_TEXTURE2D_ARRAY, 1);
	m_pTexture->LoadTextureFromDDSFile(pd3dDevice, pd3dCommandList, L"Assets/Image/Textures/Metal02.dds", RESOURCE_TEXTURE2D_ARRAY, 2);
	m_pTexture->LoadTextureFromDDSFile(pd3dDevice, pd3dCommandList, L"Assets/Image/Textures/Rock01.dds", RESOURCE_TEXTURE2D_ARRAY, 3);

	CreateShaderResourceViews(pd3dDevice, m_pTexture, 3, 8);

	btCollisionShape* CubeShape = new btBoxShape(btVector3(btScalar(fLength / 2), btScalar(fHeight / 2), btScalar(fDepth / 2)));

	btCollisionShapes.push_back(CubeShape);

	CHeightMapTerrain* pTerrain = (CHeightMapTerrain*)pContext;
	float fTerrainWidth = pTerrain->GetWidth(), fTerrainLength = pTerrain->GetLength();

	//직육면체를 지형 표면에 그리고 지형보다 높은 위치에 일정한 간격으로 배치한다.
	int xObjects = 3, yObjects = 1, zObjects = 3;
	m_nObjects = xObjects * yObjects * zObjects;

	m_vpObjects.reserve(m_nObjects);

	std::shared_ptr<CRotatingObject> pRotatingObject = NULL;
	for (int i = 0, x = 0; x < xObjects; x++)
	{
		for (int z = 0; z < zObjects; z++)
		{
			for (int y = 0; y < yObjects; y++)
			{
				pRotatingObject = std::make_shared<CRotatingObject>();

				float xPosition = 100 + x * 10;
				float zPosition = 100 + z * 10;

				btTransform btCubeTransform;
				btCubeTransform.setIdentity();

				btScalar mass(5000.0f);

				//rigidbody is dynamic if and only if mass is non zero, otherwise static
				bool isDynamic = (mass != 0.f);

				btVector3 localInertia(0, 0, 0);
				if (isDynamic)
					CubeShape->calculateLocalInertia(mass, localInertia);

				pRotatingObject->SetMaterial(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3(0.6f, 0.6f, 0.6f), 0.3f);
				pRotatingObject->SetPosition(xPosition, 50.0f, zPosition);

				pRotatingObject->SetTextureIndex(z % 4);

				XMFLOAT3 xmf3ObjPosition = pRotatingObject->GetPosition();
				btCubeTransform.setOrigin(btVector3(xmf3ObjPosition.x, xmf3ObjPosition.y, xmf3ObjPosition.z));

				//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
				btDefaultMotionState* myMotionState = new btDefaultMotionState(btCubeTransform);
				btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, CubeShape, localInertia);

				pRotatingObject->SetRigidBody(rbInfo);
				pbtDynamicsWorld->addRigidBody(pRotatingObject->GetRigidBody());

				pRotatingObject->SetInstanceNum(m_nObjects);

				m_vpObjects.push_back(pRotatingObject);
			}
		}
	}
	
	
	//인스턴싱을 사용하여 렌더링하기 위하여 하나의 게임 객체만 메쉬를 가진다.
	std::shared_ptr<CCubeMesh> pCubeMesh = std::make_shared<CCubeMesh>(pd3dDevice, pd3dCommandList, fLength, fHeight, fDepth);
	m_vpObjects[0]->SetMesh(pCubeMesh);


	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CInstancingShader::Render(ID3D12GraphicsCommandList* pd3dCommandList)
{
	CShader::Render(pd3dCommandList);
	//모든 게임 객체의 인스턴싱 데이터를 버퍼에 저장한다.
	UpdateShaderVariables(pd3dCommandList);

	//하나의 정점 데이터를 사용하여 모든 게임 객체(인스턴스)들을 렌더링한다.
	m_vpObjects[0]->Render(pd3dCommandList, m_vpObjects.size());
}

CTerrainShader::CTerrainShader()
{
}

CTerrainShader::~CTerrainShader()
{
}

void CTerrainShader::CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	ID3DBlob* pd3dVertexShaderBlob = NULL, * pd3dPixelShaderBlob = NULL, * pd3dGeometryShaderBlob = NULL;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc;
	::ZeroMemory(&d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	d3dPipelineStateDesc.pRootSignature = pd3dGraphicsRootSignature;
	d3dPipelineStateDesc.VS = CreateVertexShader(&pd3dVertexShaderBlob);
	d3dPipelineStateDesc.PS = CreatePixelShader(&pd3dPixelShaderBlob);
	d3dPipelineStateDesc.HS = CreateHullShader(&pd3dGeometryShaderBlob);
	d3dPipelineStateDesc.DS = CreateDomainShader(&pd3dGeometryShaderBlob);
	d3dPipelineStateDesc.RasterizerState = CreateRasterizerState();
	d3dPipelineStateDesc.BlendState = CreateBlendState();
	d3dPipelineStateDesc.DepthStencilState = CreateDepthStencilState();
	d3dPipelineStateDesc.InputLayout = CreateInputLayout();
	d3dPipelineStateDesc.SampleMask = UINT_MAX;
	d3dPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
	d3dPipelineStateDesc.NumRenderTargets = 1;
	d3dPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dPipelineStateDesc.SampleDesc.Count = 1;
	d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	pd3dDevice->CreateGraphicsPipelineState(&d3dPipelineStateDesc, __uuidof(ID3D12PipelineState), (void**)&m_pd3dWireframePipeline);

	d3dPipelineStateDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	pd3dDevice->CreateGraphicsPipelineState(&d3dPipelineStateDesc, __uuidof(ID3D12PipelineState), (void**)&m_pd3dPipelineState);

	if (pd3dVertexShaderBlob)
		pd3dVertexShaderBlob->Release();

	if (pd3dPixelShaderBlob)
		pd3dPixelShaderBlob->Release();

	if (pd3dGeometryShaderBlob)
		pd3dGeometryShaderBlob->Release();

	if (d3dPipelineStateDesc.InputLayout.pInputElementDescs)
		delete[] d3dPipelineStateDesc.InputLayout.pInputElementDescs;
}

D3D12_INPUT_LAYOUT_DESC CTerrainShader::CreateInputLayout()
{
	UINT nInputElementDescs = 4;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];
	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[2] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 2, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[3] = { "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 3, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CTerrainShader::CreateVertexShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Terrain.hlsl", "VS_Terrain", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CTerrainShader::CreatePixelShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Terrain.hlsl", "PS_Terrain", "ps_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CTerrainShader::CreateHullShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Terrain.hlsl", "HSTerrainTessellation", "hs_5_1", ppd3dShaderBlob));
}
D3D12_SHADER_BYTECODE CTerrainShader::CreateDomainShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Terrain.hlsl", "DSTerrainTessellation", "ds_5_1", ppd3dShaderBlob));
}
void CTerrainShader::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CTerrainShader::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ComPtr<ID3D12DescriptorHeap> pd3dSrvDescriptorHeap)
{
	m_d3dSrvCPUDescriptorHandle = pd3dSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	m_d3dSrvGPUDescriptorHandle = pd3dSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();

	m_pTexture = std::make_shared<CTexture>(3, RESOURCE_TEXTURE2D_ARRAY, 0, 1);

	m_pTexture->LoadTextureFromDDSFile(pd3dDevice, pd3dCommandList, L"Assets/Image/Textures/Base_Texture.dds", RESOURCE_TEXTURE2D_ARRAY, 0);
	m_pTexture->LoadTextureFromDDSFile(pd3dDevice, pd3dCommandList, L"Assets/Image/Textures/Detail_Texture_7.dds", RESOURCE_TEXTURE2D_ARRAY, 1);
	m_pTexture->LoadTextureFromDDSFile(pd3dDevice, pd3dCommandList, L"Assets/Image/Textures/Terrain_Road.dds", RESOURCE_TEXTURE2D_ARRAY, 2);

	CreateShaderResourceViews(pd3dDevice, m_pTexture, 7, 8);
}

D3D12_RASTERIZER_DESC CTerrainShader::CreateRasterizerState()
{
	D3D12_RASTERIZER_DESC d3dRasterizerDesc;
	::ZeroMemory(&d3dRasterizerDesc, sizeof(D3D12_RASTERIZER_DESC));
	d3dRasterizerDesc.FillMode = D3D12_FILL_MODE_WIREFRAME;
	d3dRasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	d3dRasterizerDesc.FrontCounterClockwise = FALSE;
	d3dRasterizerDesc.DepthBias = 0;
	d3dRasterizerDesc.DepthBiasClamp = 0.0f;
	d3dRasterizerDesc.SlopeScaledDepthBias = 0.0f;
	d3dRasterizerDesc.DepthClipEnable = TRUE;
	d3dRasterizerDesc.MultisampleEnable = FALSE;
	d3dRasterizerDesc.AntialiasedLineEnable = FALSE;
	d3dRasterizerDesc.ForcedSampleCount = 0;
	d3dRasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
	return(d3dRasterizerDesc);
}

void CTerrainShader::Render(ID3D12GraphicsCommandList* pd3dCommandList)
{
	if(m_bRenderMode)
		CShader::Render(pd3dCommandList);
	else
		pd3dCommandList->SetPipelineState(m_pd3dWireframePipeline);

	UpdateShaderVariables(pd3dCommandList);
}

void CTerrainShader::ChangeRendermode()
{
	m_bRenderMode = 1 - m_bRenderMode;
}

CLightsShader::CLightsShader()
{
}

CLightsShader::~CLightsShader()
{
}

void CLightsShader::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	m_vLight.reserve(2);

	//ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, int type, XMFLOAT3 xmf3Strength, float fFalloffStart, XMFLOAT3 xmf3Direction, float fFalloffEnd, XMFLOAT3 xmf3Position, float fSpotPower

	m_vLight.emplace_back(pd3dDevice, pd3dCommandList, DIRECTIONAL_LIGHT, XMFLOAT3{ 0.5f, 0.5f, 0.5f }, NULL, XMFLOAT3{ 0.0f, -1.0f, 0.0f }, NULL, XMFLOAT3{ 1950.0f, 100.0f, 1950.0f }, NULL);
	

	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}
 
void CLightsShader::ReleaseObjects()
{
	if (m_vLight.size() > 0)
	{
		m_vLight.clear();
	}
}

void CLightsShader::Render(ID3D12GraphicsCommandList* pd3dCommandList)
{
	CShader::Render(pd3dCommandList);
	//모든 게임 객체의 인스턴싱 데이터를 버퍼에 저장한다.
	UpdateShaderVariables(pd3dCommandList);

	//하나의 정점 데이터를 사용하여 모든 게임 객체(인스턴스)들을 렌더링한다.
	for (auto i = m_vLight.begin(); i != m_vLight.end(); ++i)
	{
		i->Render(pd3dCommandList);
	}
}

D3D12_INPUT_LAYOUT_DESC CLightsShader::CreateInputLayout()
{
	UINT nInputElementDescs = 3;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];
	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[2] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 2, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_SHADER_BYTECODE CLightsShader::CreateVertexShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Default.hlsl", "VS_Default", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CLightsShader::CreatePixelShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Default.hlsl", "PS_Default", "ps_5_1", ppd3dShaderBlob));
}

void CLightsShader::ReleaseUploadBuffers()
{
	if (m_vLight.size() > 0)
	{
		for (int j = 0; j < m_vLight.size(); j++) m_vLight[j].ReleaseUploadBuffers();
	}
}

void CLightsShader::ReleaseShaderVariables()
{
	if (m_pd3dcbLight)
	{
		m_pd3dcbLight->Unmap(0, nullptr);
		m_pd3dcbLight->Release();
	}
}

void CLightsShader::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	const UINT buffeSize{ static_cast<UINT>(sizeof(VS_VB_LIGHT_INFO) * m_vLight.size()) };
	m_pd3dcbLight = CreateBufferResource(pd3dDevice, pd3dCommandList, nullptr, buffeSize, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr);
	m_pd3dcbLight->Map(0, nullptr, (void**)&m_pcbMappedLights);
}

void CLightsShader::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	for (int i = 0; i < m_vLight.size(); ++i)
	{
		m_pcbMappedLights[i] = VS_VB_LIGHT_INFO(m_vLight[i]);
	}
	
	size_t vSize = m_vLight.size();
	pd3dCommandList->SetGraphicsRootShaderResourceView(6, m_pd3dcbLight->GetGPUVirtualAddress());
	pd3dCommandList->SetGraphicsRoot32BitConstants(2, 1, &vSize, 0);
}

void CLightsShader::Update(float fTimeElapsed, XMFLOAT3 xmf3PlayerPosition)
{
	for (int i = 0; i < m_vLight.size(); i++)
	{
		m_vLight[i].Update(fTimeElapsed, xmf3PlayerPosition);
	}
}

CShadowShader::CShadowShader()
{
}

CShadowShader::~CShadowShader()
{
}

D3D12_INPUT_LAYOUT_DESC CShadowShader::CreateInputLayout()
{
	UINT nInputElementDescs = 3;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];
	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[2] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 2, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

void CShadowShader::CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	m_ubShadowCB = new UploadBuffer<CB_SHADOW>(pd3dDevice, 1, true);

	ID3DBlob* pd3dVertexShaderBlob = NULL, * pd3dPixelShaderBlob = NULL;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc;
	::ZeroMemory(&d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	d3dPipelineStateDesc.pRootSignature = pd3dGraphicsRootSignature;
	d3dPipelineStateDesc.VS = CShader::CompileShaderFromFile(L"Shadow.hlsl", "VS", "vs_5_1", &pd3dVertexShaderBlob);
	d3dPipelineStateDesc.PS = CShader::CompileShaderFromFile(L"Shadow.hlsl", "PS", "ps_5_1", &pd3dPixelShaderBlob);
	d3dPipelineStateDesc.RasterizerState = CreateRasterizerState();

	d3dPipelineStateDesc.RasterizerState.DepthBias = 10000;
	d3dPipelineStateDesc.RasterizerState.DepthBiasClamp = 0.0f;
	d3dPipelineStateDesc.RasterizerState.SlopeScaledDepthBias = 1.0f;

	d3dPipelineStateDesc.BlendState = CreateBlendState();
	d3dPipelineStateDesc.DepthStencilState = CreateDepthStencilState();
	d3dPipelineStateDesc.InputLayout = CreateInputLayout();
	d3dPipelineStateDesc.SampleMask = UINT_MAX;
	d3dPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	d3dPipelineStateDesc.NumRenderTargets = 0;
	d3dPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_UNKNOWN;
	d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dPipelineStateDesc.SampleDesc.Count = 1;
	d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	pd3dDevice->CreateGraphicsPipelineState(&d3dPipelineStateDesc, __uuidof(ID3D12PipelineState), (void**)&m_pd3dPipelineState);


	d3dPipelineStateDesc.VS = CShader::CompileShaderFromFile(L"ShadowInstancing.hlsl", "VS", "vs_5_1", &pd3dVertexShaderBlob);
	d3dPipelineStateDesc.PS = CShader::CompileShaderFromFile(L"ShadowInstancing.hlsl", "PS", "ps_5_1", &pd3dPixelShaderBlob);

	pd3dDevice->CreateGraphicsPipelineState(&d3dPipelineStateDesc, __uuidof(ID3D12PipelineState), (void**)&m_pd3dInstancingPipelineState);


	if (pd3dVertexShaderBlob)
		pd3dVertexShaderBlob->Release();

	if (pd3dPixelShaderBlob)
		pd3dPixelShaderBlob->Release();

	if (d3dPipelineStateDesc.InputLayout.pInputElementDescs)
		delete[] d3dPipelineStateDesc.InputLayout.pInputElementDescs;
}

D3D12_SHADER_BYTECODE CShadowShader::CreateVertexShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shadow.hlsl", "VS", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CShadowShader::CreatePixelShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Shadow.hlsl", "PS", "ps_5_1", ppd3dShaderBlob));
}

void CShadowShader::Render(ID3D12GraphicsCommandList* pd3dCommandList, CPlayer* pPlayer, float fBoundary, int nShadowIndex)
{
	pd3dCommandList->SetPipelineState(m_pd3dPipelineState);
	UpdateShaderVariables(pd3dCommandList, pPlayer->GetPosition(), fBoundary, nShadowIndex);

	for (std::shared_ptr<CGameObject> o : m_vpGameObjects)
	{
		if (o != NULL)
		{
			o->ShadowPassRender(pd3dCommandList);
		}
	}
	pd3dCommandList->SetPipelineState(m_pd3dInstancingPipelineState);
	for (std::shared_ptr<CGameObject> o : m_vpInstancingGameObjects)
	{
		if (o != NULL)
		{
			o->ShadowPassRender(pd3dCommandList);
		}
	}
}

void CShadowShader::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT3 xmf3TargetPos, float fBoundary, int nShadowIndex)
{
	XMVECTOR lightPos = XMLoadFloat3(&m_pLight->GetPosition());
	XMVECTOR TargetPos = XMLoadFloat3(&xmf3TargetPos);
	XMVECTOR lightUp = XMLoadFloat3(&m_pLight->GetUp());

	XMVECTOR lightDir = XMVectorSubtract(lightPos, TargetPos);
	float lightLength = XMVector3Length(lightDir).m128_f32[0];

	XMMATRIX lightView = XMMatrixLookAtLH(lightPos, TargetPos, lightUp);

	// Transform bounding sphere to light space.
	XMFLOAT3 xmf3CenterLS;
	XMStoreFloat3(&xmf3CenterLS, XMVector3TransformCoord(XMLoadFloat3(&xmf3TargetPos), lightView));

	// Ortho frustum in light space encloses scene.
	float l = xmf3CenterLS.x - fBoundary;
	float b = xmf3CenterLS.y - fBoundary;
	float n = xmf3CenterLS.z - fBoundary;
	float r = xmf3CenterLS.x + fBoundary;
	float t = xmf3CenterLS.y + fBoundary;
	float f = xmf3CenterLS.z + fBoundary;

	XMMATRIX lightProj;
	lightProj = XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);

	// Transform NDC space [-1,+1]^2 to texture space [0,1]^2
	XMMATRIX T(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);

	XMMATRIX S = lightView * lightProj;

	XMStoreFloat4x4(&m_xmf4x4LightViewProj[nShadowIndex], XMMatrixTranspose(S));

	S = S * T;

	XMStoreFloat4x4(&m_xmf4x4ShadowTransform[nShadowIndex], XMMatrixTranspose(S));

	CB_SHADOW cbShadow{ m_xmf4x4ShadowTransform, m_xmf4x4LightViewProj, m_pLight->GetPosition() };

	m_ubShadowCB->CopyData(0, cbShadow);
	pd3dCommandList->SetGraphicsRootConstantBufferView(3, m_ubShadowCB->Resource()->GetGPUVirtualAddress());
	pd3dCommandList->SetGraphicsRoot32BitConstants(2, 1, &nShadowIndex, 1);
}

CSkyBoxShader::CSkyBoxShader()
{
}

CSkyBoxShader::~CSkyBoxShader()
{
}

D3D12_DEPTH_STENCIL_DESC CSkyBoxShader::CreateDepthStencilState()
{
	D3D12_DEPTH_STENCIL_DESC d3dDepthStencilDesc;
	d3dDepthStencilDesc.DepthEnable = FALSE;
	d3dDepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	d3dDepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_NEVER;
	d3dDepthStencilDesc.StencilEnable = TRUE;
	d3dDepthStencilDesc.StencilReadMask = 0xff;
	d3dDepthStencilDesc.StencilWriteMask = 0xff;
	d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_GREATER;
	d3dDepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	return(d3dDepthStencilDesc);
}

D3D12_SHADER_BYTECODE CSkyBoxShader::CreateVertexShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Skybox.hlsl", "VS_SkyBox", "vs_5_1", ppd3dShaderBlob));
}


D3D12_SHADER_BYTECODE CSkyBoxShader::CreatePixelShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Skybox.hlsl", "PS_SkyBox", "ps_5_1", ppd3dShaderBlob));
}

void CSkyBoxShader::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

D3D12_INPUT_LAYOUT_DESC CSkyBoxShader::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];
	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

void CSkyBoxShader::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ComPtr<ID3D12DescriptorHeap> pd3dSrvDescriptorHeap)
{
	m_d3dSrvCPUDescriptorHandle = pd3dSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	m_d3dSrvGPUDescriptorHandle = pd3dSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();

	m_pTexture = std::make_shared<CTexture>(6, RESOURCE_TEXTURE2D_ARRAY, 0, 1);

	m_pTexture->LoadTextureFromDDSFile(pd3dDevice, pd3dCommandList, L"Assets/Image/Textures/SkyBox_Front_0.dds", RESOURCE_TEXTURE2D_ARRAY, 0);
	m_pTexture->LoadTextureFromDDSFile(pd3dDevice, pd3dCommandList, L"Assets/Image/Textures/SkyBox_Back_0.dds", RESOURCE_TEXTURE2D_ARRAY, 1);
	m_pTexture->LoadTextureFromDDSFile(pd3dDevice, pd3dCommandList, L"Assets/Image/Textures/SkyBox_Left_0.dds", RESOURCE_TEXTURE2D_ARRAY, 2);
	m_pTexture->LoadTextureFromDDSFile(pd3dDevice, pd3dCommandList, L"Assets/Image/Textures/SkyBox_Right_0.dds", RESOURCE_TEXTURE2D_ARRAY, 3);
	m_pTexture->LoadTextureFromDDSFile(pd3dDevice, pd3dCommandList, L"Assets/Image/Textures/SkyBox_Top_0.dds", RESOURCE_TEXTURE2D_ARRAY, 4);
	m_pTexture->LoadTextureFromDDSFile(pd3dDevice, pd3dCommandList, L"Assets/Image/Textures/SkyBox_Bottom_0.dds", RESOURCE_TEXTURE2D_ARRAY, 5);

	CreateShaderResourceViews(pd3dDevice, m_pTexture, 10, 8);

	m_pSkybox = std::make_unique<CSkyBox>(pd3dDevice, pd3dCommandList);
}

void CSkyBoxShader::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	CShader::Render(pd3dCommandList);

	UpdateShaderVariables(pd3dCommandList);

	m_pSkybox->Render(pd3dCommandList, pCamera);
}

CTreeBillBoardShader::CTreeBillBoardShader()
{

}

CTreeBillBoardShader::~CTreeBillBoardShader()
{
}

D3D12_SHADER_BYTECODE CTreeBillBoardShader::CreatePixelShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"BillBoardTree.hlsl", "PS_BillBoard", "ps_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CTreeBillBoardShader::CreateVertexShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"BillBoardTree.hlsl", "VS_BillBoard", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CTreeBillBoardShader::CreateGeometryShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"BillBoardTree.hlsl", "GS_BillBoard", "gs_5_1", ppd3dShaderBlob));
}

void CTreeBillBoardShader::CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	ID3DBlob* pd3dVertexShaderBlob = NULL, * pd3dPixelShaderBlob = NULL, * pd3dGeometryShaderBlob = NULL;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc;
	::ZeroMemory(&d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	d3dPipelineStateDesc.pRootSignature = pd3dGraphicsRootSignature;
	d3dPipelineStateDesc.VS = CreateVertexShader(&pd3dVertexShaderBlob);
	d3dPipelineStateDesc.PS = CreatePixelShader(&pd3dPixelShaderBlob);
	d3dPipelineStateDesc.GS = CreateGeometryShader(&pd3dGeometryShaderBlob);
	d3dPipelineStateDesc.RasterizerState = CreateRasterizerState();
	d3dPipelineStateDesc.BlendState = CreateBlendState();
	d3dPipelineStateDesc.DepthStencilState = CreateDepthStencilState();
	d3dPipelineStateDesc.InputLayout = CreateInputLayout();
	d3dPipelineStateDesc.SampleMask = UINT_MAX;
	d3dPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	d3dPipelineStateDesc.NumRenderTargets = 1;
	d3dPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dPipelineStateDesc.SampleDesc.Count = 1;
	d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	auto tmp = pd3dDevice->CreateGraphicsPipelineState(&d3dPipelineStateDesc, __uuidof(ID3D12PipelineState), (void**)&m_pd3dPipelineState);

	if (pd3dVertexShaderBlob)
		pd3dVertexShaderBlob->Release();

	if (pd3dPixelShaderBlob)
		pd3dPixelShaderBlob->Release();

	if (pd3dGeometryShaderBlob)
		pd3dGeometryShaderBlob->Release();

	if (d3dPipelineStateDesc.InputLayout.pInputElementDescs)
		delete[] d3dPipelineStateDesc.InputLayout.pInputElementDescs;
}

D3D12_INPUT_LAYOUT_DESC CTreeBillBoardShader::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];
	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

void CTreeBillBoardShader::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ComPtr<ID3D12DescriptorHeap> pd3dSrvDescriptorHeap)
{
	m_d3dSrvCPUDescriptorHandle = pd3dSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	m_d3dSrvGPUDescriptorHandle = pd3dSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();

	m_pTexture = std::make_shared<CTexture>(2, RESOURCE_TEXTURE2DARRAY, 0, 1);

	m_pTexture->LoadTextureFromDDSFile(pd3dDevice, pd3dCommandList, L"Assets/Image/Textures/treearray.dds", RESOURCE_TEXTURE2DARRAY, 0);
	m_pTexture->LoadTextureFromDDSFile(pd3dDevice, pd3dCommandList, L"Assets/Image/Textures/grassarray.dds", RESOURCE_TEXTURE2DARRAY, 1);

	CreateShaderResourceViews(pd3dDevice, m_pTexture, 16, 8);

	XMFLOAT3* pxmf3GrassPosition = new XMFLOAT3[10000];

	for (int i = 0; i < 100; ++i)
	{
		for (int j = 0; j < 100; ++j)
		{
			pxmf3GrassPosition[(i * 100) + j] = XMFLOAT3{float(i * 20), 0.0f, float(j * 20) };
		}
	}

	std::shared_ptr<CBillBoardMesh> pGrassMesh = std::make_shared<CBillBoardMesh>(pd3dDevice, pd3dCommandList, pxmf3GrassPosition, 5, 5, 10000);

	XMFLOAT3* pxmf3TreePosition = new XMFLOAT3[100];

	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			pxmf3TreePosition[(i * 10) + j] = XMFLOAT3{ float(i * 100) + 5.0f, 0.0f, float(j * 100) + 5.0f };
		}
	}

	std::shared_ptr<CBillBoardMesh> pTreeMesh = std::make_shared <CBillBoardMesh>(pd3dDevice, pd3dCommandList, pxmf3TreePosition, 20, 20, 1000);

	m_pBillBoard = new CGameObject(2);
	m_pBillBoard->SetMesh(pGrassMesh);
	m_pBillBoard->SetMesh(pTreeMesh);
}

void CTreeBillBoardShader::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
}

void CTreeBillBoardShader::Render(ID3D12GraphicsCommandList* pd3dCommandList)
{
	CShader::Render(pd3dCommandList);
	UpdateShaderVariables(pd3dCommandList);
	m_pBillBoard->Render(pd3dCommandList);
}

CAnimatedBillBoardShader::CAnimatedBillBoardShader()
{
}

CAnimatedBillBoardShader::~CAnimatedBillBoardShader()
{
}


D3D12_SHADER_BYTECODE CAnimatedBillBoardShader::CreatePixelShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"AnimatedBillBoard.hlsl", "PS_BillBoard", "ps_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CAnimatedBillBoardShader::CreateVertexShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"AnimatedBillBoard.hlsl", "VS_BillBoard", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CAnimatedBillBoardShader::CreateGeometryShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"AnimatedBillBoard.hlsl", "GS_BillBoard", "gs_5_1", ppd3dShaderBlob));
}

void CAnimatedBillBoardShader::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ComPtr<ID3D12DescriptorHeap> pd3dSrvDescriptorHeap)
{
	m_vpAnimatedBillBoard.reserve(50);

	m_ubAnimatedBillBoard = new UploadBuffer<CB_ANIMATEDBILLBOARD>(pd3dDevice, 50, true);

	m_d3dSrvCPUDescriptorHandle = pd3dSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	m_d3dSrvGPUDescriptorHandle = pd3dSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();

	m_pTexture = std::make_shared<CTexture>(1, RESOURCE_TEXTURE2D, 0, 1);

	m_pTexture->LoadTextureFromDDSFile(pd3dDevice, pd3dCommandList, L"Assets/Image/Textures/Explosion_Effect.dds", RESOURCE_TEXTURE2D, 0);

	CreateShaderResourceViews(pd3dDevice, m_pTexture, 18, 8);
}

void CAnimatedBillBoardShader::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList, float fTimeElapsed)
{
	for (auto i = m_vpAnimatedBillBoard.begin(); i < m_vpAnimatedBillBoard.end();)
	{
		i->UpdateShaderVariables(pd3dCommandList, fTimeElapsed);
		if (i->isEnd())
		{
			i = m_vpAnimatedBillBoard.erase(i);
		}
		else
		{
			i++;
		}
	}
}

void CAnimatedBillBoardShader::Render(ID3D12GraphicsCommandList* pd3dCommandList)
{
	CShader::Render(pd3dCommandList);
	int CBIndex = 0;
	for (auto i = m_vpAnimatedBillBoard.begin(); i < m_vpAnimatedBillBoard.end(); ++i)
	{
		m_ubAnimatedBillBoard->CopyData(CBIndex, i->GetBillBoardInfo());
		pd3dCommandList->SetGraphicsRootConstantBufferView(4, m_ubAnimatedBillBoard->Resource()->GetGPUVirtualAddress() + CBIndex * m_ubAnimatedBillBoard->GetElementSize());
		CBIndex++;
		i->Render(pd3dCommandList);
	} 
}

void CAnimatedBillBoardShader::AddBillBoard(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT3 xmf3Position, int nTotalFrame, int nxDivided, int nyDivided, std::vector<float> vfFrameTime)
{
	m_vpAnimatedBillBoard.emplace_back(pd3dDevice, pd3dCommandList, xmf3Position, nxDivided, nyDivided, vfFrameTime);
}

CMirrorShader::CMirrorShader()
{
}

CMirrorShader::~CMirrorShader()
{
}

D3D12_SHADER_BYTECODE CMirrorShader::CreateVertexShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Mirror.hlsl", "VS_Mirror", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CMirrorShader::CreatePixelShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Mirror.hlsl", "PS_Mirror", "ps_5_1", ppd3dShaderBlob));
}


D3D12_INPUT_LAYOUT_DESC CMirrorShader::CreateInputLayout()
{
	UINT nInputElementDescs = 2;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];
	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
	pd3dInputElementDescs[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

D3D12_BLEND_DESC CMirrorShader::CreateBlendState()
{
	D3D12_BLEND_DESC d3dBlendDesc;
	::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
	d3dBlendDesc.AlphaToCoverageEnable = TRUE;
	d3dBlendDesc.IndependentBlendEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].BlendEnable = TRUE;
	d3dBlendDesc.RenderTarget[0].LogicOpEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ZERO;
	d3dBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
	d3dBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	d3dBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ONE;
	d3dBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	return(d3dBlendDesc);
}

//깊이-스텐실 검사를 위한 상태를 설정하기 위한 구조체를 반환한다.
D3D12_DEPTH_STENCIL_DESC CMirrorShader::CreateDepthStencilState()
{
	D3D12_DEPTH_STENCIL_DESC d3dDepthStencilDesc;
	::ZeroMemory(&d3dDepthStencilDesc, sizeof(D3D12_DEPTH_STENCIL_DESC));
	d3dDepthStencilDesc.DepthEnable = TRUE;
	d3dDepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	d3dDepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	d3dDepthStencilDesc.StencilEnable = FALSE;
	d3dDepthStencilDesc.StencilReadMask = 0x00;
	d3dDepthStencilDesc.StencilWriteMask = 0x00;
	d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
	d3dDepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
	return(d3dDepthStencilDesc);
}

void CMirrorShader::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ComPtr<ID3D12DescriptorHeap> pd3dSrvDescriptorHeap)
{
	m_pMirror = std::make_unique<CGameObject>();

	std::shared_ptr<CTexturedRectMesh> pMesh = std::make_shared<CTexturedRectMesh>(pd3dDevice, pd3dCommandList, 500, 500, 0, 0, 0, 0.0);
	m_pMirror->SetPosition(2000, 250, 2160);
	m_pMirror->SetMesh(pMesh);

	m_d3dSrvCPUDescriptorHandle = pd3dSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	m_d3dSrvGPUDescriptorHandle = pd3dSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();

	m_pTexture = std::make_shared<CTexture>(1, RESOURCE_TEXTURE2D, 0, 1);
	m_pTexture->LoadTextureFromDDSFile(pd3dDevice, pd3dCommandList, L"Assets/Image/Textures/ice.dds", RESOURCE_TEXTURE2D, 0);
	CreateShaderResourceViews(pd3dDevice, m_pTexture, 20, 8);
}


void CMirrorShader::Render(ID3D12GraphicsCommandList* pd3dCommandList)
{
	CShader::Render(pd3dCommandList);
	m_pMirror->Render(pd3dCommandList);
}

void CMirrorShader::StencilRender(ID3D12GraphicsCommandList* pd3dCommandList)
{
	pd3dCommandList->SetPipelineState(m_pd3StencilPipeline);
	m_pMirror->Render(pd3dCommandList);
}

void CMirrorShader::CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	CShader::CreateShader(pd3dDevice, pd3dGraphicsRootSignature);

	ID3DBlob* pd3dVertexShaderBlob = NULL, * pd3dPixelShaderBlob = NULL, * pd3dGeometryShaderBlob = NULL;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc;
	::ZeroMemory(&d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	d3dPipelineStateDesc.pRootSignature = pd3dGraphicsRootSignature;
	d3dPipelineStateDesc.VS = CreateVertexShader(&pd3dVertexShaderBlob);
	d3dPipelineStateDesc.PS = CreatePixelShader(&pd3dPixelShaderBlob);
	d3dPipelineStateDesc.GS = CreateGeometryShader(&pd3dGeometryShaderBlob);
	d3dPipelineStateDesc.HS = CreateHullShader(&pd3dGeometryShaderBlob);
	d3dPipelineStateDesc.DS = CreateDomainShader(&pd3dGeometryShaderBlob);
	d3dPipelineStateDesc.RasterizerState = CreateRasterizerState();

	d3dPipelineStateDesc.BlendState = CShader::CreateBlendState();
	d3dPipelineStateDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = 0;

	d3dPipelineStateDesc.DepthStencilState = CreateDepthStencilState();

	d3dPipelineStateDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	d3dPipelineStateDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	d3dPipelineStateDesc.DepthStencilState.StencilEnable = true;
	d3dPipelineStateDesc.DepthStencilState.StencilReadMask = 0xff;
	d3dPipelineStateDesc.DepthStencilState.StencilWriteMask = 0xff;
	d3dPipelineStateDesc.DepthStencilState.FrontFace.StencilPassOp = D3D12_STENCIL_OP_REPLACE;
	d3dPipelineStateDesc.DepthStencilState.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	d3dPipelineStateDesc.DepthStencilState.BackFace.StencilPassOp = D3D12_STENCIL_OP_REPLACE;
	d3dPipelineStateDesc.DepthStencilState.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	d3dPipelineStateDesc.InputLayout = CreateInputLayout();
	d3dPipelineStateDesc.SampleMask = UINT_MAX;
	d3dPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	d3dPipelineStateDesc.NumRenderTargets = 1;
	d3dPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dPipelineStateDesc.SampleDesc.Count = 1;
	d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	auto tmp = pd3dDevice->CreateGraphicsPipelineState(&d3dPipelineStateDesc, __uuidof(ID3D12PipelineState), (void**)&m_pd3StencilPipeline);


	if (pd3dVertexShaderBlob)
		pd3dVertexShaderBlob->Release();

	if (pd3dPixelShaderBlob)
		pd3dPixelShaderBlob->Release();

	if (pd3dGeometryShaderBlob)
		pd3dGeometryShaderBlob->Release();

	if (d3dPipelineStateDesc.InputLayout.pInputElementDescs)
		delete[] d3dPipelineStateDesc.InputLayout.pInputElementDescs;
}

D3D12_SHADER_BYTECODE CRoomShader::CreateVertexShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Default_No_Light.hlsl", "VS_Default", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CRoomShader::CreatePixelShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Default_No_Light.hlsl", "PS_Default", "ps_5_1", ppd3dShaderBlob));
}

D3D12_INPUT_LAYOUT_DESC CRoomShader::CreateInputLayout()
{
	UINT nInputElementDescs = 1;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];
	pd3dInputElementDescs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

void CRoomShader::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	std::shared_ptr<CGameObject> pObject = std::make_shared<CGameObject>(4);

	std::shared_ptr<CRectMesh> pMesh = std::make_shared<CRectMesh>(pd3dDevice, pd3dCommandList, 500.0f, 500.0f, 0.0f, 0.0f, 0.0f, -250.0f);
	pObject->SetMesh(pMesh); // 후

	pMesh = std::make_shared<CRectMesh>(pd3dDevice, pd3dCommandList, 0.0f, 500.0f, 500.0f, -250.0f, 0.0f, 0.0f);
	pObject->SetMesh(pMesh); // 좌

	pMesh = std::make_shared<CRectMesh>(pd3dDevice, pd3dCommandList, 0.0f, 500.0f, 500.0f, +250.0f, 0.0f, 0.0f);
	pObject->SetMesh(pMesh); // 우

	pMesh = std::make_shared<CRectMesh>(pd3dDevice, pd3dCommandList, 500.0f, 0.0f, 500.0f, 0.0f, +250.0f, 0.0f);
	pObject->SetMesh(pMesh); // 상

	pMesh = std::make_shared<CRectMesh>(pd3dDevice, pd3dCommandList, 500.0f, 0.0f, 500.0f, 0.0f, -249.99f, 0.0f);
	pObject->SetMesh(pMesh); // 하

	pObject->SetPosition(XMFLOAT3{ 2000, 250, 1910 });
	pObject->SetMaterial(XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f), XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f), XMFLOAT3(0.6f, 0.6f, 0.6f), 0.8f);

	m_vpObjects.push_back(pObject);
}

D3D12_SHADER_BYTECODE CPaticleBillBoardShader::CreatePixelShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Particle_Instancing.hlsl", "PS_BillBoard", "ps_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CPaticleBillBoardShader::CreateVertexShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Particle_Instancing.hlsl", "VS_BillBoard", "vs_5_1", ppd3dShaderBlob));
}

D3D12_SHADER_BYTECODE CPaticleBillBoardShader::CreateGeometryShader(ID3DBlob** ppd3dShaderBlob)
{
	return(CShader::CompileShaderFromFile(L"Particle_Instancing.hlsl", "GS_BillBoard", "gs_5_1", ppd3dShaderBlob));
}


D3D12_BLEND_DESC CPaticleBillBoardShader::CreateBlendState()
{
	D3D12_BLEND_DESC d3dBlendDesc;
	::ZeroMemory(&d3dBlendDesc, sizeof(D3D12_BLEND_DESC));
	d3dBlendDesc.AlphaToCoverageEnable = TRUE;
	d3dBlendDesc.IndependentBlendEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].BlendEnable = TRUE;
	d3dBlendDesc.RenderTarget[0].LogicOpEnable = FALSE;
	d3dBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
	d3dBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
	d3dBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ZERO;
	d3dBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ONE;
	d3dBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	d3dBlendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	return(d3dBlendDesc);
}

D3D12_INPUT_LAYOUT_DESC CPaticleBillBoardShader::CreateInputLayout()
{
	UINT nInputElementDescs = 1;
	D3D12_INPUT_ELEMENT_DESC* pd3dInputElementDescs = new D3D12_INPUT_ELEMENT_DESC[nInputElementDescs];
	pd3dInputElementDescs[0] = { "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

	D3D12_INPUT_LAYOUT_DESC d3dInputLayoutDesc;
	d3dInputLayoutDesc.pInputElementDescs = pd3dInputElementDescs;
	d3dInputLayoutDesc.NumElements = nInputElementDescs;

	return(d3dInputLayoutDesc);
}

//깊이-스텐실 검사를 위한 상태를 설정하기 위한 구조체를 반환한다.
D3D12_DEPTH_STENCIL_DESC CPaticleBillBoardShader::CreateDepthStencilState()
{
	D3D12_DEPTH_STENCIL_DESC d3dDepthStencilDesc;
	::ZeroMemory(&d3dDepthStencilDesc, sizeof(D3D12_DEPTH_STENCIL_DESC));
	d3dDepthStencilDesc.DepthEnable = FALSE;
	d3dDepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	d3dDepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	d3dDepthStencilDesc.StencilEnable = FALSE;
	d3dDepthStencilDesc.StencilReadMask = 0x00;
	d3dDepthStencilDesc.StencilWriteMask = 0x00;
	d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
	d3dDepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_NEVER;
	return(d3dDepthStencilDesc);
}

//그래픽스 파이프라인 상태 객체를 생성한다.
void CPaticleBillBoardShader::CreateShader(ID3D12Device* pd3dDevice, ID3D12RootSignature* pd3dGraphicsRootSignature)
{
	ID3DBlob* pd3dVertexShaderBlob = NULL, * pd3dPixelShaderBlob = NULL, * pd3dGeometryShaderBlob = NULL;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc;
	::ZeroMemory(&d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	d3dPipelineStateDesc.pRootSignature = pd3dGraphicsRootSignature;
	d3dPipelineStateDesc.VS = CreateVertexShader(&pd3dVertexShaderBlob);
	d3dPipelineStateDesc.PS = CreatePixelShader(&pd3dPixelShaderBlob);
	d3dPipelineStateDesc.GS = CreateGeometryShader(&pd3dGeometryShaderBlob);
	d3dPipelineStateDesc.HS = CreateHullShader(&pd3dGeometryShaderBlob);
	d3dPipelineStateDesc.DS = CreateDomainShader(&pd3dGeometryShaderBlob);
	d3dPipelineStateDesc.RasterizerState = CreateRasterizerState();
	d3dPipelineStateDesc.BlendState = CreateBlendState();
	d3dPipelineStateDesc.DepthStencilState = CreateDepthStencilState();
	d3dPipelineStateDesc.InputLayout = CreateInputLayout();
	d3dPipelineStateDesc.SampleMask = UINT_MAX;
	d3dPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	d3dPipelineStateDesc.NumRenderTargets = 1;
	d3dPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dPipelineStateDesc.SampleDesc.Count = 1;
	d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	auto tmp = pd3dDevice->CreateGraphicsPipelineState(&d3dPipelineStateDesc, __uuidof(ID3D12PipelineState), (void**)&m_pd3dPipelineState);

	if (pd3dVertexShaderBlob)
		pd3dVertexShaderBlob->Release();

	if (pd3dPixelShaderBlob)
		pd3dPixelShaderBlob->Release();

	if (pd3dGeometryShaderBlob)
		pd3dGeometryShaderBlob->Release();

	if (d3dPipelineStateDesc.InputLayout.pInputElementDescs)
		delete[] d3dPipelineStateDesc.InputLayout.pInputElementDescs;
}

void CPaticleBillBoardShader::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ComPtr<ID3D12DescriptorHeap> pd3dSrvDescriptorHeap)
{
	m_vpParticleBillBoard.reserve(1000);

	m_d3dSrvCPUDescriptorHandle = pd3dSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	m_d3dSrvGPUDescriptorHandle = pd3dSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();

	m_pTexture = std::make_shared<CTexture>(1, RESOURCE_TEXTURE2D, 0, 1);

	m_pTexture->LoadTextureFromDDSFile(pd3dDevice, pd3dCommandList, L"Assets/Image/Textures/Fire_Particle.dds", RESOURCE_TEXTURE2D, 0);

	CreateShaderResourceViews(pd3dDevice, m_pTexture, 21, 8);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CPaticleBillBoardShader::Render(ID3D12GraphicsCommandList* pd3dCommandList)
{
	CShader::Render(pd3dCommandList);

	//하나의 정점 데이터를 사용하여 모든 게임 객체(인스턴스)들을 렌더링한다.
	if(m_vpParticleBillBoard.size() > 0)
		m_vpParticleBillBoard[0]->Render(pd3dCommandList, m_vpParticleBillBoard.size());
}

void CPaticleBillBoardShader::AddBillBoard(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT3 xmf3Position)
{
	if (m_vpParticleBillBoard.size() < 1000)
	{
		CParticle* pObject = new CParticle(8.0f, { urd(dre), 1.0f, urd(dre) });
		std::shared_ptr<CMesh> pMesh = std::make_shared<CParticleMesh>(pd3dDevice, pd3dCommandList, 1.0f, 1.0f);

		pObject->SetMesh(pMesh);
		pObject->SetPosition(xmf3Position);

		m_vpParticleBillBoard.push_back(pObject);
	}
}

void CPaticleBillBoardShader::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	//인스턴스 정보를 저장할 정점 버퍼를 업로드 힙 유형으로 생성한다.
	//파티클의 최대 갯수는 1000개이다.
	m_pd3dcbParticles = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, sizeof(XMFLOAT4X4) * 1000, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ, NULL);

	//정점 버퍼(업로드 힙)에 대한 포인터를 저장한다.
	m_pd3dcbParticles->Map(0, NULL, (void**)&m_pcbMappedParticles);
}

void CPaticleBillBoardShader::UpdateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, float fTimeElapsed)
{
	if (m_bRender)
	{
		m_fCreationCycle -= fTimeElapsed;
		if (m_fCreationCycle < 0.0f)
		{
			m_fCreationCycle = 0.3f;
			AddBillBoard(pd3dDevice, pd3dCommandList, m_xmf3BasePosition);
		}
	}

	pd3dCommandList->SetGraphicsRootShaderResourceView(7, m_pd3dcbParticles->GetGPUVirtualAddress());
	int cnt = 0;
	for (auto i = m_vpParticleBillBoard.begin(); i != m_vpParticleBillBoard.end();)
	{
		if ((*i)->GetLiveTime() < 0.0f)
		{
			i = m_vpParticleBillBoard.erase(i);
			continue;
		}
		(*i)->Update(fTimeElapsed);
		XMStoreFloat4x4(&m_pcbMappedParticles[cnt], XMMatrixTranspose(XMLoadFloat4x4(&m_vpParticleBillBoard[cnt]->GetWorldTransformMatrix())));
		cnt++;
		i++;
	}
}
