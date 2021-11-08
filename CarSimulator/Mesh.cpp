#include "stdafx.h"
#include "Camera.h"
#include "Mesh.h"

CMesh::CMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
}

CMesh::~CMesh()
{
	if (m_pd3dPositionBuffer) m_pd3dPositionBuffer->Release();
	if (m_pd3dPositionUploadBuffer) m_pd3dPositionUploadBuffer->Release();

	if (m_pd3dNormalBuffer) m_pd3dNormalBuffer->Release();
	if (m_pd3dNormalUploadBuffer) m_pd3dNormalUploadBuffer->Release();

	if (m_pd3dTextureCoordBuffer) m_pd3dTextureCoordBuffer->Release();
	if (m_pd3dTextureCoordUploadBuffer) m_pd3dTextureCoordUploadBuffer->Release();

	if (m_pd3dIndexBuffer) m_pd3dIndexBuffer->Release();
	if (m_pd3dIndexUploadBuffer) m_pd3dIndexUploadBuffer->Release();
}

void CMesh::ReleaseUploadBuffers()
{
	//���� ���۸� ���� ���ε� ���۸� �Ҹ��Ų��.
	if (m_pd3dPositionBuffer)
		m_pd3dPositionBuffer->Release();
	m_pd3dPositionUploadBuffer = NULL;

	if (m_pd3dNormalBuffer)
		m_pd3dNormalBuffer->Release();
	m_pd3dNormalUploadBuffer = NULL;

	if (m_pd3dTextureCoordBuffer)
		m_pd3dTextureCoordBuffer->Release();
	m_pd3dTextureCoordUploadBuffer = NULL;

	if (m_pd3dIndexUploadBuffer)
		m_pd3dIndexUploadBuffer->Release();
	m_pd3dIndexUploadBuffer = NULL;
}

void CMesh::Render(ID3D12GraphicsCommandList* pd3dCommandList)
{
	pd3dCommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);
	pd3dCommandList->IASetVertexBuffers(m_nSlot, m_nVertexBufferViews, m_pd3dVertexBufferViews);
	if (m_pd3dIndexBuffer)
	{
		pd3dCommandList->IASetIndexBuffer(&m_d3dIndexBufferView);
		pd3dCommandList->DrawIndexedInstanced(m_nIndices, 1, 0, 0, 0);
	}
	else
	{
		pd3dCommandList->DrawInstanced(m_nVertices, 1, m_nOffset, 0);
	}
}

void CMesh::Render(ID3D12GraphicsCommandList* pd3dCommandList, UINT nInstances)
{
	pd3dCommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);
	pd3dCommandList->IASetVertexBuffers(m_nSlot, m_nVertexBufferViews, m_pd3dVertexBufferViews);
	if (m_pd3dIndexBuffer)
	{
		pd3dCommandList->IASetIndexBuffer(&m_d3dIndexBufferView);
		pd3dCommandList->DrawIndexedInstanced(m_nIndices, nInstances, 0, 0, 0);
	}
	else
	{
		pd3dCommandList->DrawInstanced(m_nVertices, nInstances, m_nOffset, 0);
	}
}
	

CTriangleMesh::CTriangleMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList) : CMesh(pd3dDevice, pd3dCommandList)
{
	//�ﰢ�� �޽��� �����Ѵ�.
	m_nVertices = 3;
	m_nStride = sizeof(CDiffusedVertex);
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	/*����(�ﰢ���� ������)�� ������ �ð���� ������� ������, ���, �Ķ������� �����Ѵ�. RGBA(Red, Green, Blue,
	Alpha) 4���� �Ķ���͸� ����Ͽ� ������ ǥ���Ѵ�. �� �Ķ���ʹ� 0.0~1.0 ������ �Ǽ����� ������.*/
	CDiffusedVertex pVertices[3];
	pVertices[0] = CDiffusedVertex(XMFLOAT3(0.0f, 0.5f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
	pVertices[1] = CDiffusedVertex(XMFLOAT3(0.5f, -0.5f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
	pVertices[2] = CDiffusedVertex(XMFLOAT3(-0.5f, -0.5f, 0.0f), XMFLOAT4(Colors::Blue));

	m_nVertexBufferViews = 1;
	m_pd3dVertexBufferViews = new D3D12_VERTEX_BUFFER_VIEW[m_nVertexBufferViews];

	//�ﰢ�� �޽��� ���ҽ�(���� ����)�� �����Ѵ�.
	m_pd3dPositionBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, pVertices, 
	m_nStride* m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dPositionUploadBuffer);

	//���� ���� �並 �����Ѵ�.
	m_pd3dVertexBufferViews[0].BufferLocation = m_pd3dPositionBuffer->GetGPUVirtualAddress();
	m_pd3dVertexBufferViews[0].StrideInBytes = m_nStride;
	m_pd3dVertexBufferViews[0].SizeInBytes = m_nStride * m_nVertices;
}

CCubeMesh::~CCubeMesh()
{
}

CCubeMesh::CCubeMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, float fWidth, float fHeight, float fDepth) : CMesh(pd3dDevice, pd3dCommandList)
{
	m_nVertices = 36;
	m_nStride = sizeof(CTexturedNormalVertex);
	m_nOffset = 0;
	m_nSlot = 0;
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	float fx = fWidth * 0.5f, fy = fHeight * 0.5f, fz = fDepth * 0.5f;

	CTexturedNormalVertex pVertices[36];
	int i = 0;

	// �޸�
	pVertices[i++] = CTexturedNormalVertex(XMFLOAT3(-fx, +fy, -fz), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(XMFLOAT3(+fx, +fy, -fz), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(XMFLOAT3(+fx, -fy, -fz), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 1.0f));

	pVertices[i++] = CTexturedNormalVertex(XMFLOAT3(-fx, +fy, -fz), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(XMFLOAT3(+fx, -fy, -fz), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 1.0f));
	pVertices[i++] = CTexturedNormalVertex(XMFLOAT3(-fx, -fy, -fz), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 1.0f));

	//����
	pVertices[i++] = CTexturedNormalVertex(XMFLOAT3(-fx, +fy, +fz), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(XMFLOAT3(+fx, +fy, +fz), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(XMFLOAT3(+fx, +fy, -fz), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f));

	pVertices[i++] = CTexturedNormalVertex(XMFLOAT3(-fx, +fy, +fz), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(XMFLOAT3(+fx, +fy, -fz), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f));
	pVertices[i++] = CTexturedNormalVertex(XMFLOAT3(-fx, +fy, -fz), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f));

	//�ո�
	pVertices[i++] = CTexturedNormalVertex(XMFLOAT3(-fx, -fy, +fz), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(XMFLOAT3(+fx, -fy, +fz), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(XMFLOAT3(+fx, +fy, +fz), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f));

	pVertices[i++] = CTexturedNormalVertex(XMFLOAT3(-fx, -fy, +fz), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(XMFLOAT3(+fx, +fy, +fz), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f));
	pVertices[i++] = CTexturedNormalVertex(XMFLOAT3(-fx, +fy, +fz), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f));

	//�Ʒ���
	pVertices[i++] = CTexturedNormalVertex(XMFLOAT3(-fx, -fy, -fz), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(XMFLOAT3(+fx, -fy, -fz), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(XMFLOAT3(+fx, -fy, +fz), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f));

	pVertices[i++] = CTexturedNormalVertex(XMFLOAT3(-fx, -fy, -fz), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(XMFLOAT3(+fx, -fy, +fz), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f));
	pVertices[i++] = CTexturedNormalVertex(XMFLOAT3(-fx, -fy, +fz), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f));

	//���ʸ�
	pVertices[i++] = CTexturedNormalVertex(XMFLOAT3(-fx, +fy, +fz), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(XMFLOAT3(-fx, +fy, -fz), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(XMFLOAT3(-fx, -fy, -fz), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f));

	pVertices[i++] = CTexturedNormalVertex(XMFLOAT3(-fx, +fy, +fz), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(XMFLOAT3(-fx, -fy, -fz), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f));
	pVertices[i++] = CTexturedNormalVertex(XMFLOAT3(-fx, -fy, +fz), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f));

	//�����ʸ�
	pVertices[i++] = CTexturedNormalVertex(XMFLOAT3(+fx, +fy, -fz), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(XMFLOAT3(+fx, +fy, +fz), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(XMFLOAT3(+fx, -fy, +fz), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f));

	pVertices[i++] = CTexturedNormalVertex(XMFLOAT3(+fx, +fy, -fz), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f));
	pVertices[i++] = CTexturedNormalVertex(XMFLOAT3(+fx, -fy, +fz), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f));
	pVertices[i++] = CTexturedNormalVertex(XMFLOAT3(+fx, -fy, -fz), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f));

	m_nVertexBufferViews = 1;
	m_pd3dVertexBufferViews = new D3D12_VERTEX_BUFFER_VIEW[m_nVertexBufferViews];

	m_pd3dPositionBuffer = CreateBufferResource(pd3dDevice, pd3dCommandList, pVertices, m_nStride * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dPositionUploadBuffer);

	m_pd3dVertexBufferViews[0].BufferLocation = m_pd3dPositionBuffer->GetGPUVirtualAddress();
	m_pd3dVertexBufferViews[0].StrideInBytes = m_nStride;
	m_pd3dVertexBufferViews[0].SizeInBytes = m_nStride * m_nVertices;

	//�޽��� �ٿ�� �ڽ�(�� ��ǥ��)�� �����Ѵ�.
	m_xmBoundingBox = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(fx, fy, fz), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
}

CHeightMapImage::CHeightMapImage(LPCTSTR pFileName, int nWidth, int nLength, XMFLOAT3 xmf3Scale)
{
	m_nWidth = nWidth;
	m_nLength = nLength;
	m_xmf3Scale = xmf3Scale;
	BYTE* pHeightMapPixels = new BYTE[m_nWidth * m_nLength];

	//������ ���� �д´�. ���� �� �̹����� ���� ����� ���� RAW �̹����̴�.
	HANDLE hFile = ::CreateFile(pFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_READONLY, NULL);
	DWORD dwBytesRead;
	::ReadFile(hFile, pHeightMapPixels, (m_nWidth * m_nLength), &dwBytesRead, NULL);
	::CloseHandle(hFile);

	m_pHeightMapPixels = new BYTE[m_nWidth * m_nLength];
	for (int y = 0; y < m_nLength; y++)
	{
		for (int x = 0; x < m_nWidth; x++)
		{
			m_pHeightMapPixels[x + ((m_nLength - 1 - y) * m_nWidth)] = pHeightMapPixels[x + (y * m_nWidth)];
		}
	}
	if (pHeightMapPixels)
		delete[] pHeightMapPixels;
}

CHeightMapImage::~CHeightMapImage()
{
	if (m_pHeightMapPixels) delete[] m_pHeightMapPixels;
	m_pHeightMapPixels = NULL;
}

XMFLOAT3 CHeightMapImage::GetHeightMapNormal(int x, int z)
{
	//x-��ǥ�� z-��ǥ�� ���� ���� ������ ����� ������ ���� ���ʹ� y-�� ���� �����̴�.
	if ((x < 0.0f) || (z < 0.0f) || (x >= m_nWidth) || (z >= m_nLength)) 
		return(XMFLOAT3(0.0f, 1.0f, 0.0f));

	/*���� �ʿ��� (x, z) ��ǥ�� �ȼ� ���� ������ �� ���� �� (x+1, z), (z, z+1)�� ���� �ȼ� ���� ����Ͽ� ���� ���͸�
	����Ѵ�.*/
	int nHeightMapIndex = x + (z * m_nWidth);
	int xHeightMapAdd = (x < (m_nWidth - 1)) ? 1 : -1;
	int zHeightMapAdd = (z < (m_nLength - 1)) ? m_nWidth : -m_nWidth;

	//(x, z), (x+1, z), (z, z+1)�� �ȼ����� ������ ���̸� ���Ѵ�.
	float y1 = (float)m_pHeightMapPixels[nHeightMapIndex] * m_xmf3Scale.y;
	float y2 = (float)m_pHeightMapPixels[nHeightMapIndex + xHeightMapAdd] * m_xmf3Scale.y;
	float y3 = (float)m_pHeightMapPixels[nHeightMapIndex + zHeightMapAdd] * m_xmf3Scale.y;

	//xmf3Edge1�� (0, y3, m_xmf3Scale.z) - (0, y1, 0) �����̴�.
	XMFLOAT3 xmf3Edge1 = XMFLOAT3(0.0f, y3 - y1, m_xmf3Scale.z);

	//xmf3Edge2�� (m_xmf3Scale.x, y2, 0) - (0, y1, 0) �����̴�.
	XMFLOAT3 xmf3Edge2 = XMFLOAT3(m_xmf3Scale.x, y2 - y1, 0.0f);

	//���� ���ʹ� xmf3Edge1�� xmf3Edge2�� ������ ����ȭ�ϸ� �ȴ�.
	XMFLOAT3 xmf3Normal = Vector3::CrossProduct(xmf3Edge1, xmf3Edge2, true);
	return(xmf3Normal);
}

#define _WITH_APPROXIMATE_OPPOSITE_CORNER
float CHeightMapImage::GetHeight(float fx, float fz)
{
	/*������ ��ǥ (fx, fz)�� �̹��� ��ǥ���̴�. ���� ���� x-��ǥ�� z-��ǥ�� ���� ���� ������ ����� ������ ���̴�
	0�̴�.*/
	if ((fx < 0.0f) || (fz < 0.0f) || (fx >= m_nWidth) || (fz >= m_nLength))
		return(0.0f);

	//���� ���� ��ǥ�� ���� �κа� �Ҽ� �κ��� ����Ѵ�.
	int x = (int)fx;
	int z = (int)fz;
	float fxPercent = fx - x;
	float fzPercent = fz - z;
	float fBottomLeft = (float)m_pHeightMapPixels[x + (z * m_nWidth)];
	float fBottomRight = (float)m_pHeightMapPixels[(x + 1) + (z * m_nWidth)];
	float fTopLeft = (float)m_pHeightMapPixels[x + ((z + 1) * m_nWidth)];
	float fTopRight = (float)m_pHeightMapPixels[(x + 1) + ((z + 1) * m_nWidth)];

#ifdef _WITH_APPROXIMATE_OPPOSITE_CORNER
	//z-��ǥ�� 1, 3, 5, ...�� ��� �ε����� �����ʿ��� �������� �����ȴ�.
	bool bRightToLeft = ((z % 2) != 0);
	if (bRightToLeft)
	{
		if (fzPercent >= fxPercent)
			fBottomRight = fBottomLeft + (fTopRight - fTopLeft);
		else
			fTopLeft = fTopRight + (fBottomLeft - fBottomRight);
	}
	else
	{
		if (fzPercent < (1.0f - fxPercent))
			fTopRight = fTopLeft + (fBottomRight - fBottomLeft);
		else
			fBottomLeft = fTopLeft + (fBottomRight - fTopRight);
	}
#endif
	//�簢���� �� ���� �����Ͽ� ����(�ȼ� ��)�� ����Ѵ�.
	float fTopHeight = fTopLeft * (1 - fxPercent) + fTopRight * fxPercent;
	float fBottomHeight = fBottomLeft * (1 - fxPercent) + fBottomRight * fxPercent;
	float fHeight = fBottomHeight * (1 - fzPercent) + fTopHeight * fzPercent;
	return(fHeight);
}

CHeightMapGridMesh::CHeightMapGridMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, int xStart, int zStart, int nWidth, int nLength, float& fMaxHeight, float& fMinHeight, XMFLOAT3 xmf3Scale, void* pContext) : CMesh(pd3dDevice, pd3dCommandList)
{
	//������ ����(����)�� ������ (nWidth * nLength)�̴�.
	m_nVertices = 25;
	m_nStride = sizeof(XMFLOAT3);

	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST;
	m_nWidth = nWidth;
	m_nLength = nLength;
	m_xmf3Scale = xmf3Scale;
	
	m_pxmf3Positions = new XMFLOAT3[m_nVertices];
	m_pxmf3Normals = new XMFLOAT3[m_nVertices];
	m_pxmf2TextureCoords = new XMFLOAT2[m_nVertices];
	m_pxmf2DetailTextureCoords = new XMFLOAT2[m_nVertices]; 

	CHeightMapImage* pHeightMapImage = (CHeightMapImage*)pContext;
	int cxHeightMap = pHeightMapImage->GetHeightMapWidth();
	int czHeightMap = pHeightMapImage->GetHeightMapLength();

	/*xStart�� zStart�� ������ ���� ��ġ(x-��ǥ�� z-��ǥ)�� ��Ÿ����. Ŀ�ٶ� ������ ���ڵ��� ������ �迭�� ���� ��
	�䰡 �ֱ� ������ ��ü �������� �� ������ ���� ��ġ�� ��Ÿ���� ������ �ʿ��ϴ�.*/
	float fHeight = 0.0f;
	int nIncrease = 5; //(Block Size == 9) ? 2, (Block Size == 13) ? 3
	for (int i = 0, z = (zStart + nLength - 1); z >= zStart; z -= nIncrease)
	{
		for (int x = xStart; x < (xStart + nWidth); x += nIncrease, i++)
		{
			//������ ���̿� ����� ���� �����κ��� ���Ѵ�.
			fHeight = OnGetHeight(x, z, pContext);

			m_pxmf3Positions[i] = XMFLOAT3((x * m_xmf3Scale.x), fHeight, (z * m_xmf3Scale.z));
			m_pxmf3Normals[i] = OnGetNormal(x, z, pContext);

			m_pxmf2TextureCoords[i] = XMFLOAT2(float(x) / float(cxHeightMap - 1), float(czHeightMap - 1 - z) / float(czHeightMap - 1));
			m_pxmf2DetailTextureCoords[i] = XMFLOAT2(float(x) / float(m_xmf3Scale.x * 0.5f), float(z) / float(m_xmf3Scale.z * 0.5f));

			if (fHeight / m_xmf3Scale.y < fMinHeight)
				fMinHeight = fHeight / m_xmf3Scale.y;

			if (fHeight / m_xmf3Scale.y > fMaxHeight)
				fMaxHeight = fHeight / m_xmf3Scale.y;
		}
	}

	m_nVertexBufferViews = 4;
	m_pd3dVertexBufferViews = new D3D12_VERTEX_BUFFER_VIEW[m_nVertexBufferViews];

	m_pd3dPositionBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, m_pxmf3Positions, sizeof(XMFLOAT3) * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dPositionUploadBuffer);
	m_pd3dNormalBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, m_pxmf3Normals, sizeof(XMFLOAT3) * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dNormalUploadBuffer);
	m_pd3dTextureCoordBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, m_pxmf2TextureCoords, sizeof(XMFLOAT2) * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dTextureCoordUploadBuffer);
	m_pd3dDetailTextureBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, m_pxmf2DetailTextureCoords, sizeof(XMFLOAT2) * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dDetailTextureUploadBuffer);


	m_pd3dVertexBufferViews[0].BufferLocation = m_pd3dPositionBuffer->GetGPUVirtualAddress();
	m_pd3dVertexBufferViews[0].StrideInBytes = sizeof(XMFLOAT3);
	m_pd3dVertexBufferViews[0].SizeInBytes = sizeof(XMFLOAT3) * m_nVertices;

	m_pd3dVertexBufferViews[1].BufferLocation = m_pd3dNormalBuffer->GetGPUVirtualAddress();
	m_pd3dVertexBufferViews[1].StrideInBytes = sizeof(XMFLOAT3);
	m_pd3dVertexBufferViews[1].SizeInBytes = sizeof(XMFLOAT3) * m_nVertices;

	m_pd3dVertexBufferViews[2].BufferLocation = m_pd3dTextureCoordBuffer->GetGPUVirtualAddress();
	m_pd3dVertexBufferViews[2].StrideInBytes = sizeof(XMFLOAT2);
	m_pd3dVertexBufferViews[2].SizeInBytes = sizeof(XMFLOAT2) * m_nVertices;

	m_pd3dVertexBufferViews[3].BufferLocation = m_pd3dDetailTextureBuffer->GetGPUVirtualAddress();
	m_pd3dVertexBufferViews[3].StrideInBytes = sizeof(XMFLOAT2);
	m_pd3dVertexBufferViews[3].SizeInBytes = sizeof(XMFLOAT2) * m_nVertices;


	//m_nIndices = ((nWidth * 2) * (nLength - 1)) + ((nLength - 1) - 1);
	//m_pnIndices = new UINT[m_nIndices];

	//for (int j = 0, z = 0; z < nLength - 1; z++)
	//{ 
	//	if ((z % 2) == 0)
	//	{
	//		//Ȧ�� ��° ���̹Ƿ�(z = 0, 2, 4, ...) �ε����� ���� ������ ���ʿ��� ������ �����̴�.
	//		for (int x = 0; x < nWidth; x++)
	//		{
	//			//ù ��° ���� �����ϰ� ���� �ٲ� ������(x == 0) ù ��° �ε����� �߰��Ѵ�.
	//			if ((x == 0) && (z > 0))
	//				m_pnIndices[j++] = (UINT)(x + (z * nWidth));

	//			//�Ʒ�(x, z), ��(x, z+1)�� ������ �ε����� �߰��Ѵ�.
	//			m_pnIndices[j++] = (UINT)(x + (z * nWidth));
	//			m_pnIndices[j++] = (UINT)((x + (z * nWidth)) + nWidth);
	//		}
	//	}
	//	else
	//	{
	//		//¦�� ��° ���̹Ƿ�(z = 1, 3, 5, ...) �ε����� ���� ������ �����ʿ��� ���� �����̴�.
	//		for (int x = nWidth - 1; x >= 0; x--)
	//		{
	//			//���� �ٲ� ������(x == (nWidth-1)) ù ��° �ε����� �߰��Ѵ�.
	//			if (x == (nWidth - 1)) m_pnIndices[j++] = (UINT)(x + (z * nWidth));
	//			//�Ʒ�(x, z), ��(x, z+1)�� ������ �ε����� �߰��Ѵ�.
	//			m_pnIndices[j++] = (UINT)(x + (z * nWidth));
	//			m_pnIndices[j++] = (UINT)((x + (z * nWidth)) + nWidth);
	//		}
	//	}
	//}
	//m_pd3dIndexBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, m_pnIndices, sizeof(UINT) * m_nIndices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_INDEX_BUFFER, &m_pd3dIndexUploadBuffer);
	//m_d3dIndexBufferView.BufferLocation = m_pd3dIndexBuffer->GetGPUVirtualAddress();
	//m_d3dIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	//m_d3dIndexBufferView.SizeInBytes = sizeof(UINT) * m_nIndices;
}

CHeightMapGridMesh::~CHeightMapGridMesh()
{
}

//���� �� �̹����� �ȼ� ���� ������ ���̷� ��ȯ�Ѵ�.
float CHeightMapGridMesh::OnGetHeight(int x, int z, void *pContext)
{
	CHeightMapImage* pHeightMapImage = (CHeightMapImage*)pContext;
	BYTE* pHeightMapPixels = pHeightMapImage->GetHeightMapPixels();
	XMFLOAT3 xmf3Scale = pHeightMapImage->GetScale();
	int nWidth = pHeightMapImage->GetHeightMapWidth();
	float fHeight = pHeightMapPixels[x + (z * nWidth)] * xmf3Scale.y;
	return(fHeight);
}

XMFLOAT3 CHeightMapGridMesh::OnGetNormal(int x, int z, void* pContext)
{
	CHeightMapImage* pHeightMapImage = (CHeightMapImage*)pContext;
	XMFLOAT3 xmf3Normal = pHeightMapImage->GetHeightMapNormal(x, z);
	return xmf3Normal;
}

XMFLOAT4 CHeightMapGridMesh::OnGetColor(int x, int z, void* pContext)
{
	//������ ���� ����(�������� ��������� ����)�̴�.
	XMFLOAT3 xmf3LightDirection = XMFLOAT3(-1.0f, 1.0f, 1.0f);
	xmf3LightDirection = Vector3::Normalize(xmf3LightDirection);
	CHeightMapImage* pHeightMapImage = (CHeightMapImage*)pContext;
	XMFLOAT3 xmf3Scale = pHeightMapImage->GetScale();

	//������ ����(����, ���)�̴�.
	XMFLOAT4 xmf4IncidentLightColor(0.9f, 0.8f, 0.4f, 1.0f);
	float fScale = Vector3::DotProduct(pHeightMapImage->GetHeightMapNormal(x, z), xmf3LightDirection);
	fScale += Vector3::DotProduct(pHeightMapImage->GetHeightMapNormal(x + 1, z), xmf3LightDirection);
	fScale += Vector3::DotProduct(pHeightMapImage->GetHeightMapNormal(x + 1, z + 1), xmf3LightDirection);
	fScale += Vector3::DotProduct(pHeightMapImage->GetHeightMapNormal(x, z + 1), xmf3LightDirection);
	fScale = (fScale / 4.0f) + 0.05f;
	if (fScale > 1.0f) fScale = 1.0f;
	if (fScale < 0.25f) fScale = 0.25f;

	//fScale�� ���� ����(���)�� �ݻ�Ǵ� �����̴�.
	XMFLOAT4 xmf4Color = Vector4::Multiply(fScale, xmf4IncidentLightColor);
	return(xmf4Color);
}

CMeshFileRead::CMeshFileRead(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, char* pstrFileName, bool bTextFile, XMFLOAT3 xmf3Scale, XMFLOAT3 xmf3Rotation) : CMesh(pd3dDevice, pd3dCommandList)
{
	if (pstrFileName) LoadMeshFromFile(pd3dDevice, pd3dCommandList, pstrFileName, bTextFile, xmf3Scale, xmf3Rotation);
}

CMeshFileRead::~CMeshFileRead()
{
	if (m_pxmf3Positions) delete[] m_pxmf3Positions;
	if (m_pxmf3Normals) delete[] m_pxmf3Normals;
	if (m_pxmf2TextureCoords) delete[] m_pxmf2TextureCoords;

	if (m_pnIndices) delete[] m_pnIndices;

	if (m_pd3dVertexBufferViews) delete[] m_pd3dVertexBufferViews;

	if (m_pd3dPositionBuffer) m_pd3dPositionBuffer->Release();
	if (m_pd3dNormalBuffer) m_pd3dNormalBuffer->Release();
	if (m_pd3dTextureCoordBuffer) m_pd3dTextureCoordBuffer->Release();
	if (m_pd3dIndexBuffer) m_pd3dIndexBuffer->Release();
}

void CMeshFileRead::ReleaseUploadBuffers()
{
	if (m_pd3dPositionUploadBuffer) m_pd3dPositionUploadBuffer->Release();
	if (m_pd3dNormalUploadBuffer) m_pd3dNormalUploadBuffer->Release();
	if (m_pd3dTextureCoordUploadBuffer) m_pd3dTextureCoordUploadBuffer->Release();
	if (m_pd3dIndexUploadBuffer) m_pd3dIndexUploadBuffer->Release();

	m_pd3dPositionUploadBuffer = NULL;
	m_pd3dNormalUploadBuffer = NULL;
	m_pd3dTextureCoordUploadBuffer = NULL;
	m_pd3dIndexUploadBuffer = NULL;
}

void CMeshFileRead::LoadMeshFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, char* pstrFileName, bool bTextFile, XMFLOAT3 xmf3Scale, XMFLOAT3 xmf3Rotation)
{
	char pstrToken[64] = { '\0' };

	if (bTextFile)
	{
		std::ifstream InFile(pstrFileName);

		for (; ; )
		{
			InFile >> pstrToken;
			if (!InFile) break;

			if (!strcmp(pstrToken, "<Vertices>:"))
			{
				InFile >> m_nVertices;
				m_pxmf3Positions = new XMFLOAT3[m_nVertices];
				for (UINT i = 0; i < m_nVertices; i++) InFile >> m_pxmf3Positions[i].x >> m_pxmf3Positions[i].y >> m_pxmf3Positions[i].z;
			}
			else if (!strcmp(pstrToken, "<Normals>:"))
			{
				InFile >> pstrToken;
				m_pxmf3Normals = new XMFLOAT3[m_nVertices];
				for (UINT i = 0; i < m_nVertices; i++) InFile >> m_pxmf3Normals[i].x >> m_pxmf3Normals[i].y >> m_pxmf3Normals[i].z;
			}
			else if (!strcmp(pstrToken, "<TextureCoords>:"))
			{
				InFile >> pstrToken;
				m_pxmf2TextureCoords = new XMFLOAT2[m_nVertices];
				for (UINT i = 0; i < m_nVertices; i++) InFile >> m_pxmf2TextureCoords[i].x >> m_pxmf2TextureCoords[i].y;
			}
			else if (!strcmp(pstrToken, "<Indices>:"))
			{
				InFile >> m_nIndices;
				m_pnIndices = new UINT[m_nIndices];
				for (UINT i = 0; i < m_nIndices; i++) InFile >> m_pnIndices[i];
			}
		}
	}
	else
	{
		FILE* pFile = NULL;
		::fopen_s(&pFile, pstrFileName, "rb");
		::rewind(pFile);

		char pstrToken[64] = { '\0' };

		BYTE nStrLength = 0;
		UINT nReads = 0;

		nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pFile);
		nReads = (UINT)::fread(pstrToken, sizeof(char), 14, pFile); //"<BoundingBox>:"
		nReads = (UINT)::fread(&m_xmBoundingBox.Center, sizeof(float), 3, pFile);
		nReads = (UINT)::fread(&m_xmBoundingBox.Extents, sizeof(float), 3, pFile);

		nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pFile);
		nReads = (UINT)::fread(pstrToken, sizeof(char), 11, pFile); //"<Vertices>:"
		nReads = (UINT)::fread(&m_nVertices, sizeof(int), 1, pFile);
		m_pxmf3Positions = new XMFLOAT3[m_nVertices];
		nReads = (UINT)::fread(m_pxmf3Positions, sizeof(float), 3 * m_nVertices, pFile);

		nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pFile);
		nReads = (UINT)::fread(pstrToken, sizeof(char), 10, pFile); //"<Normals>:"
		nReads = (UINT)::fread(&m_nVertices, sizeof(int), 1, pFile);
		m_pxmf3Normals = new XMFLOAT3[m_nVertices];
		nReads = (UINT)::fread(m_pxmf3Normals, sizeof(float), 3 * m_nVertices, pFile);

		nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pFile);
		nReads = (UINT)::fread(pstrToken, sizeof(char), 16, pFile); //"<TextureCoords>:"
		nReads = (UINT)::fread(&m_nVertices, sizeof(int), 1, pFile);
		m_pxmf2TextureCoords = new XMFLOAT2[m_nVertices];
		nReads = (UINT)::fread(m_pxmf2TextureCoords, sizeof(float), 2 * m_nVertices, pFile);

		nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pFile);
		nReads = (UINT)::fread(pstrToken, sizeof(char), 10, pFile); //"<Indices>:"
		nReads = (UINT)::fread(&m_nIndices, sizeof(int), 1, pFile);
		m_pnIndices = new UINT[m_nIndices];
		nReads = (UINT)::fread(m_pnIndices, sizeof(UINT), m_nIndices, pFile);

		::fclose(pFile);
	}

	float xmin = std::numeric_limits<float>::max();
	float xmax = std::numeric_limits<float>::min();

	float ymin = std::numeric_limits<float>::max();
	float ymax = std::numeric_limits<float>::min();

	float zmin = std::numeric_limits<float>::max();
	float zmax = std::numeric_limits<float>::min();

	xmf3Rotation.x = BulletHelper::RadianToEuler(xmf3Rotation.x);
	xmf3Rotation.y = BulletHelper::RadianToEuler(xmf3Rotation.y);
	xmf3Rotation.z = BulletHelper::RadianToEuler(xmf3Rotation.z);

	for (int i = 0; i < m_nVertices; ++i)
	{
		m_pxmf3Positions[i] = Vector3::Rotate(m_pxmf3Positions[i], xmf3Rotation);

		m_pxmf3Positions[i].x *= xmf3Scale.x;
		m_pxmf3Positions[i].y *= xmf3Scale.y;
		m_pxmf3Positions[i].z *= xmf3Scale.z;

		if (m_pxmf3Positions[i].x > xmax)
		{
			xmax = m_pxmf3Positions[i].x;
		}
		if (m_pxmf3Positions[i].y > ymax)
		{
			ymax = m_pxmf3Positions[i].y;
		}
		if (m_pxmf3Positions[i].z > zmax)
		{
			zmax = m_pxmf3Positions[i].z;
		}

		if (m_pxmf3Positions[i].x < xmin)
		{
			xmin = m_pxmf3Positions[i].x;
		}
		if (m_pxmf3Positions[i].y < ymin)
		{
			ymin = m_pxmf3Positions[i].y;
		}
		if (m_pxmf3Positions[i].z < zmin)
		{
			zmin = m_pxmf3Positions[i].z;
		}
	}

	m_xmBoundingBox.Extents = { (xmax - xmin) / 2, (ymax - ymin) / 2, (zmax - zmin) / 2 };

	m_pd3dPositionBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, m_pxmf3Positions, sizeof(XMFLOAT3) * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dPositionUploadBuffer);
	m_pd3dNormalBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, m_pxmf3Normals, sizeof(XMFLOAT3) * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dNormalUploadBuffer);
	m_pd3dTextureCoordBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, m_pxmf2TextureCoords, sizeof(XMFLOAT2) * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dTextureCoordUploadBuffer);

	m_nVertexBufferViews = 3;
	m_pd3dVertexBufferViews = new D3D12_VERTEX_BUFFER_VIEW[m_nVertexBufferViews];

	m_pd3dVertexBufferViews[0].BufferLocation = m_pd3dPositionBuffer->GetGPUVirtualAddress();
	m_pd3dVertexBufferViews[0].StrideInBytes = sizeof(XMFLOAT3);
	m_pd3dVertexBufferViews[0].SizeInBytes = sizeof(XMFLOAT3) * m_nVertices;

	m_pd3dVertexBufferViews[1].BufferLocation = m_pd3dNormalBuffer->GetGPUVirtualAddress(); 
	m_pd3dVertexBufferViews[1].StrideInBytes = sizeof(XMFLOAT3);
	m_pd3dVertexBufferViews[1].SizeInBytes = sizeof(XMFLOAT3) * m_nVertices;

	m_pd3dVertexBufferViews[2].BufferLocation = m_pd3dTextureCoordBuffer->GetGPUVirtualAddress();
	m_pd3dVertexBufferViews[2].StrideInBytes = sizeof(XMFLOAT2);
	m_pd3dVertexBufferViews[2].SizeInBytes = sizeof(XMFLOAT2) * m_nVertices;

	m_pd3dIndexBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, m_pnIndices, sizeof(UINT) * m_nIndices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_INDEX_BUFFER, &m_pd3dIndexUploadBuffer);

	m_d3dIndexBufferView.BufferLocation = m_pd3dIndexBuffer->GetGPUVirtualAddress();
	m_d3dIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	m_d3dIndexBufferView.SizeInBytes = sizeof(UINT) * m_nIndices;
}

CTexturedRectMesh::CTexturedRectMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, float fWidth, float fHeight, float fDepth, float fxPosition, float fyPosition, float fzPosition) : CMesh(pd3dDevice, pd3dCommandList)
{
	m_nSlot = 0;
	m_nVertices = 6;
	m_nStride = sizeof(CTexturedVertex);
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	CTexturedVertex pVertices[6];

	float fx = (fWidth * 0.5f) + fxPosition, fy = (fHeight * 0.5f) + fyPosition, fz = (fDepth * 0.5f) + fzPosition;

	if (fWidth == 0.0f)
	{
		if (fxPosition > 0.0f)
		{
			pVertices[0] = CTexturedVertex(XMFLOAT3(fx, +fy, -fz), XMFLOAT2(1.0f, 0.0f));
			pVertices[1] = CTexturedVertex(XMFLOAT3(fx, -fy, -fz), XMFLOAT2(1.0f, 1.0f));
			pVertices[2] = CTexturedVertex(XMFLOAT3(fx, -fy, +fz), XMFLOAT2(0.0f, 1.0f));
			pVertices[3] = CTexturedVertex(XMFLOAT3(fx, -fy, +fz), XMFLOAT2(0.0f, 1.0f));
			pVertices[4] = CTexturedVertex(XMFLOAT3(fx, +fy, +fz), XMFLOAT2(0.0f, 0.0f));
			pVertices[5] = CTexturedVertex(XMFLOAT3(fx, +fy, -fz), XMFLOAT2(1.0f, 0.0f));
		}
		else
		{
			pVertices[0] = CTexturedVertex(XMFLOAT3(fx, +fy, +fz), XMFLOAT2(1.0f, 0.0f));
			pVertices[1] = CTexturedVertex(XMFLOAT3(fx, -fy, +fz), XMFLOAT2(1.0f, 1.0f));
			pVertices[2] = CTexturedVertex(XMFLOAT3(fx, -fy, -fz), XMFLOAT2(0.0f, 1.0f));
			pVertices[3] = CTexturedVertex(XMFLOAT3(fx, -fy, -fz), XMFLOAT2(0.0f, 1.0f));
			pVertices[4] = CTexturedVertex(XMFLOAT3(fx, +fy, -fz), XMFLOAT2(0.0f, 0.0f));
			pVertices[5] = CTexturedVertex(XMFLOAT3(fx, +fy, +fz), XMFLOAT2(1.0f, 0.0f));
		}
	}
	else if (fHeight == 0.0f)
	{
		if (fyPosition > 0.0f)
		{
			pVertices[0] = CTexturedVertex(XMFLOAT3(+fx, fy, -fz), XMFLOAT2(1.0f, 0.0f));
			pVertices[1] = CTexturedVertex(XMFLOAT3(+fx, fy, +fz), XMFLOAT2(1.0f, 1.0f));
			pVertices[2] = CTexturedVertex(XMFLOAT3(-fx, fy, +fz), XMFLOAT2(0.0f, 1.0f));
			pVertices[3] = CTexturedVertex(XMFLOAT3(-fx, fy, +fz), XMFLOAT2(0.0f, 1.0f));
			pVertices[4] = CTexturedVertex(XMFLOAT3(-fx, fy, -fz), XMFLOAT2(0.0f, 0.0f));
			pVertices[5] = CTexturedVertex(XMFLOAT3(+fx, fy, -fz), XMFLOAT2(1.0f, 0.0f));
		}
		else
		{
			pVertices[0] = CTexturedVertex(XMFLOAT3(+fx, fy, +fz), XMFLOAT2(1.0f, 0.0f));
			pVertices[1] = CTexturedVertex(XMFLOAT3(+fx, fy, -fz), XMFLOAT2(1.0f, 1.0f));
			pVertices[2] = CTexturedVertex(XMFLOAT3(-fx, fy, -fz), XMFLOAT2(0.0f, 1.0f));
			pVertices[3] = CTexturedVertex(XMFLOAT3(-fx, fy, -fz), XMFLOAT2(0.0f, 1.0f));
			pVertices[4] = CTexturedVertex(XMFLOAT3(-fx, fy, +fz), XMFLOAT2(0.0f, 0.0f));
			pVertices[5] = CTexturedVertex(XMFLOAT3(+fx, fy, +fz), XMFLOAT2(1.0f, 0.0f));
		}
	}
	else if (fDepth == 0.0f)
	{
		if (fzPosition > 0.0f)
		{
			pVertices[0] = CTexturedVertex(XMFLOAT3(+fx, +fy, fz), XMFLOAT2(1.0f, 0.0f));
			pVertices[1] = CTexturedVertex(XMFLOAT3(+fx, -fy, fz), XMFLOAT2(1.0f, 1.0f));
			pVertices[2] = CTexturedVertex(XMFLOAT3(-fx, -fy, fz), XMFLOAT2(0.0f, 1.0f));
			pVertices[3] = CTexturedVertex(XMFLOAT3(-fx, -fy, fz), XMFLOAT2(0.0f, 1.0f));
			pVertices[4] = CTexturedVertex(XMFLOAT3(-fx, +fy, fz), XMFLOAT2(0.0f, 0.0f));
			pVertices[5] = CTexturedVertex(XMFLOAT3(+fx, +fy, fz), XMFLOAT2(1.0f, 0.0f));
		}
		else
		{
			pVertices[0] = CTexturedVertex(XMFLOAT3(-fx, +fy, fz), XMFLOAT2(1.0f, 0.0f));
			pVertices[1] = CTexturedVertex(XMFLOAT3(-fx, -fy, fz), XMFLOAT2(1.0f, 1.0f));
			pVertices[2] = CTexturedVertex(XMFLOAT3(+fx, -fy, fz), XMFLOAT2(0.0f, 1.0f));
			pVertices[3] = CTexturedVertex(XMFLOAT3(+fx, -fy, fz), XMFLOAT2(0.0f, 1.0f));
			pVertices[4] = CTexturedVertex(XMFLOAT3(+fx, +fy, fz), XMFLOAT2(0.0f, 0.0f));
			pVertices[5] = CTexturedVertex(XMFLOAT3(-fx, +fy, fz), XMFLOAT2(1.0f, 0.0f));
		}
	}

	m_pd3dPositionBuffer = CreateBufferResource(pd3dDevice, pd3dCommandList, pVertices, m_nStride * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dPositionUploadBuffer);

	m_nVertexBufferViews = 1;
	m_pd3dVertexBufferViews = new D3D12_VERTEX_BUFFER_VIEW[m_nVertexBufferViews];

	m_pd3dVertexBufferViews[0].BufferLocation = m_pd3dPositionBuffer->GetGPUVirtualAddress();
	m_pd3dVertexBufferViews[0].StrideInBytes = m_nStride;
	m_pd3dVertexBufferViews[0].SizeInBytes = m_nStride * m_nVertices;
}

CTexturedRectMesh::~CTexturedRectMesh()
{
}

CBillBoardMesh::CBillBoardMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, XMFLOAT3* pxmf3Position, float fWidth, float fHeight, int nVertices) : CMesh(pd3dDevice, pd3dCommandList)
{
	m_nSlot = 0;
	m_nVertices = nVertices;

	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;

	std::unique_ptr<XMFLOAT2[]> pfSize = std::make_unique<XMFLOAT2[]>(nVertices);
	m_pxmf3Positions = new XMFLOAT3[nVertices];

	for (int i = 0; i < nVertices; ++i)
	{
		pfSize[i] = { fWidth, fHeight };
		m_pxmf3Positions[i] = pxmf3Position[i];
	}

	m_pd3dPositionBuffer = CreateBufferResource(pd3dDevice, pd3dCommandList, m_pxmf3Positions, sizeof(XMFLOAT3) * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dPositionUploadBuffer);
	m_pd3dSizeBuffer = CreateBufferResource(pd3dDevice, pd3dCommandList, pfSize.get(), sizeof(XMFLOAT2) * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dSizeUploadBuffer);

	m_nVertexBufferViews = 2;
	m_pd3dVertexBufferViews = new D3D12_VERTEX_BUFFER_VIEW[m_nVertexBufferViews];

	m_pd3dVertexBufferViews[0].BufferLocation = m_pd3dPositionBuffer->GetGPUVirtualAddress();
	m_pd3dVertexBufferViews[0].StrideInBytes = sizeof(XMFLOAT3);
	m_pd3dVertexBufferViews[0].SizeInBytes = sizeof(XMFLOAT3) * m_nVertices;

	m_pd3dVertexBufferViews[1].BufferLocation = m_pd3dSizeBuffer->GetGPUVirtualAddress();
	m_pd3dVertexBufferViews[1].StrideInBytes = sizeof(XMFLOAT2);
	m_pd3dVertexBufferViews[1].SizeInBytes = sizeof(XMFLOAT2) * m_nVertices;
}

CBillBoardMesh::~CBillBoardMesh()
{
	if (m_pd3dSizeBuffer)
		m_pd3dSizeBuffer->Release();
}
