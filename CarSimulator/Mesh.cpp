#include "stdafx.h"
#include "Camera.h"
#include "Mesh.h"

CMesh::CMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
}

CMesh::~CMesh()
{
	if (m_pd3dVertexBuffer) m_pd3dVertexBuffer->Release();
	if (m_pd3dVertexUploadBuffer) m_pd3dVertexUploadBuffer->Release();
	if (m_pd3dIndexBuffer) m_pd3dIndexBuffer->Release();
	if (m_pd3dIndexUploadBuffer) m_pd3dIndexUploadBuffer->Release();
}

void CMesh::ReleaseUploadBuffers()
{
	//정점 버퍼를 위한 업로드 버퍼를 소멸시킨다.
	if (m_pd3dVertexUploadBuffer)
		m_pd3dVertexUploadBuffer->Release();
	m_pd3dVertexUploadBuffer = NULL;

	if (m_pd3dIndexUploadBuffer) m_pd3dIndexUploadBuffer->Release();
	m_pd3dIndexUploadBuffer = NULL;
}

void CMesh::Render(ID3D12GraphicsCommandList* pd3dCommandList)
{
	pd3dCommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);
	pd3dCommandList->IASetVertexBuffers(m_nSlot, 1, &m_d3dVertexBufferView);
	if (m_pd3dIndexBuffer)
	{
		pd3dCommandList->IASetIndexBuffer(&m_d3dIndexBufferView);
		pd3dCommandList->DrawIndexedInstanced(m_nIndices, 1, 0, 0, 0);
		//인덱스 버퍼가 있으면 인덱스 버퍼를 파이프라인(IA: 입력 조립기)에 연결하고 인덱스를 사용하여 렌더링한다.
	}
	else
	{
		pd3dCommandList->DrawInstanced(m_nVertices, 1, m_nOffset, 0);
	}
}

void CMesh::Render(ID3D12GraphicsCommandList* pd3dCommandList, UINT nInstances)
{
	pd3dCommandList->IASetVertexBuffers(m_nSlot, 1, &m_d3dVertexBufferView);
	pd3dCommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);
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
	//삼각형 메쉬를 정의한다.
	m_nVertices = 3;
	m_nStride = sizeof(CDiffusedVertex);
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	/*정점(삼각형의 꼭지점)의 색상은 시계방향 순서대로 빨간색, 녹색, 파란색으로 지정한다. RGBA(Red, Green, Blue,
	Alpha) 4개의 파라메터를 사용하여 색상을 표현한다. 각 파라메터는 0.0~1.0 사이의 실수값을 가진다.*/
	CDiffusedVertex pVertices[3];
	pVertices[0] = CDiffusedVertex(XMFLOAT3(0.0f, 0.5f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
	pVertices[1] = CDiffusedVertex(XMFLOAT3(0.5f, -0.5f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
	pVertices[2] = CDiffusedVertex(XMFLOAT3(-0.5f, -0.5f, 0.0f), XMFLOAT4(Colors::Blue));

	//삼각형 메쉬를 리소스(정점 버퍼)로 생성한다.
	m_pd3dVertexBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, pVertices, 
	m_nStride* m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, & m_pd3dVertexUploadBuffer);

	//정점 버퍼 뷰를 생성한다.
	m_d3dVertexBufferView.BufferLocation = m_pd3dVertexBuffer->GetGPUVirtualAddress();
	m_d3dVertexBufferView.StrideInBytes = m_nStride;
	m_d3dVertexBufferView.SizeInBytes = m_nStride * m_nVertices;
}

CCubeMeshDiffused::~CCubeMeshDiffused()
{
}

CCubeMeshDiffused::CCubeMeshDiffused(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, float fWidth, float fHeight, float fDepth) : CMesh(pd3dDevice, pd3dCommandList)
{
	//직육면체는 꼭지점(정점)이 8개이다.
	m_nVertices = 8;
	m_nStride = sizeof(CNormalVertex);
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	float fx = fWidth * 0.5f, fy = fHeight * 0.5f, fz = fDepth * 0.5f;

	//정점 버퍼는 직육면체의 꼭지점 8개에 대한 정점 데이터를 가진다.
	CNormalVertex pVertices[8];
	pVertices[0] = CNormalVertex(XMFLOAT3(-fx, +fy, -fz), Vector3::Normalize(XMFLOAT3(-1.0f, 1.0f, -1.0f)));
	pVertices[1] = CNormalVertex(XMFLOAT3(+fx, +fy, -fz), Vector3::Normalize(XMFLOAT3(1.0f, 1.0f, -1.0f)));
	pVertices[2] = CNormalVertex(XMFLOAT3(+fx, +fy, +fz), Vector3::Normalize(XMFLOAT3(1.0f, 1.0f, 1.0f)));
	pVertices[3] = CNormalVertex(XMFLOAT3(-fx, +fy, +fz), Vector3::Normalize(XMFLOAT3(-1.0f, 1.0f, 1.0f)));
	pVertices[4] = CNormalVertex(XMFLOAT3(-fx, -fy, -fz), Vector3::Normalize(XMFLOAT3(-1.0f, -1.0f, -1.0f)));
	pVertices[5] = CNormalVertex(XMFLOAT3(+fx, -fy, -fz), Vector3::Normalize(XMFLOAT3(1.0f, -1.0f, -1.0f)));
	pVertices[6] = CNormalVertex(XMFLOAT3(+fx, -fy, +fz), Vector3::Normalize(XMFLOAT3(1.0f, -1.0f, 1.0f)));
	pVertices[7] = CNormalVertex(XMFLOAT3(-fx, -fy, +fz), Vector3::Normalize(XMFLOAT3(-1.0f, -1.0f, 1.0f)));
	m_pd3dVertexBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, pVertices, m_nStride * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dVertexUploadBuffer);
	m_d3dVertexBufferView.BufferLocation = m_pd3dVertexBuffer->GetGPUVirtualAddress();
	m_d3dVertexBufferView.StrideInBytes = m_nStride;
	m_d3dVertexBufferView.SizeInBytes = m_nStride * m_nVertices;
	/*인덱스 버퍼는 직육면체의 6개의 면(사각형)에 대한 기하 정보를 갖는다. 삼각형 리스트로 직육면체를 표현할 것이
	므로 각 면은 2개의 삼각형을 가지고 각 삼각형은 3개의 정점이 필요하다. 즉, 인덱스 버퍼는 전체 36(=6*2*3)개의 인
	덱스를 가져야 한다.*/
	m_nIndices = 36;
	UINT pnIndices[36];
	//ⓐ 앞면(Front) 사각형의 위쪽 삼각형
	pnIndices[0] = 3; pnIndices[1] = 1; pnIndices[2] = 0;
	//ⓑ 앞면(Front) 사각형의 아래쪽 삼각형
	pnIndices[3] = 2; pnIndices[4] = 1; pnIndices[5] = 3;
	//ⓒ 윗면(Top) 사각형의 위쪽 삼각형
	pnIndices[6] = 0; pnIndices[7] = 5; pnIndices[8] = 4;
	//ⓓ 윗면(Top) 사각형의 아래쪽 삼각형
	pnIndices[9] = 1; pnIndices[10] = 5; pnIndices[11] = 0;
	//ⓔ 뒷면(Back) 사각형의 위쪽 삼각형
	pnIndices[12] = 3; pnIndices[13] = 4; pnIndices[14] = 7;
	//ⓕ 뒷면(Back) 사각형의 아래쪽 삼각형
	pnIndices[15] = 0; pnIndices[16] = 4; pnIndices[17] = 3;
	//ⓖ 아래면(Bottom) 사각형의 위쪽 삼각형
	pnIndices[18] = 1; pnIndices[19] = 6; pnIndices[20] = 5;
	//ⓗ 아래면(Bottom) 사각형의 아래쪽 삼각형
	pnIndices[21] = 2; pnIndices[22] = 6; pnIndices[23] = 1;
	//ⓘ 옆면(Left) 사각형의 위쪽 삼각형
	pnIndices[24] = 2; pnIndices[25] = 7; pnIndices[26] = 6;
	//ⓙ 옆면(Left) 사각형의 아래쪽 삼각형
	pnIndices[27] = 3; pnIndices[28] = 7; pnIndices[29] = 2;
	//ⓚ 옆면(Right) 사각형의 위쪽 삼각형
	pnIndices[30] = 6; pnIndices[31] = 4; pnIndices[32] = 5;
	//ⓛ 옆면(Right) 사각형의 아래쪽 삼각형
	pnIndices[33] = 7; pnIndices[34] = 4; pnIndices[35] = 6;

	//인덱스 버퍼를 생성한다.
	m_pd3dIndexBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, pnIndices, sizeof(UINT)* m_nIndices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_INDEX_BUFFER, &m_pd3dIndexUploadBuffer);

	//인덱스 버퍼 뷰를 생성한다.
	m_d3dIndexBufferView.BufferLocation = m_pd3dIndexBuffer->GetGPUVirtualAddress();
	m_d3dIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	m_d3dIndexBufferView.SizeInBytes = sizeof(UINT) * m_nIndices;

	//메쉬의 바운딩 박스(모델 좌표계)를 생성한다.
	m_xmBoundingBox = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(fx, fy, fz), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
}

CHeightMapImage::CHeightMapImage(LPCTSTR pFileName, int nWidth, int nLength, XMFLOAT3 xmf3Scale)
{
	m_nWidth = nWidth;
	m_nLength = nLength;
	m_xmf3Scale = xmf3Scale;
	BYTE* pHeightMapPixels = new BYTE[m_nWidth * m_nLength];

	//파일을 열고 읽는다. 높이 맵 이미지는 파일 헤더가 없는 RAW 이미지이다.
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
	//x-좌표와 z-좌표가 높이 맵의 범위를 벗어나면 지형의 법선 벡터는 y-축 방향 벡터이다.
	if ((x < 0.0f) || (z < 0.0f) || (x >= m_nWidth) || (z >= m_nLength)) 
		return(XMFLOAT3(0.0f, 1.0f, 0.0f));

	/*높이 맵에서 (x, z) 좌표의 픽셀 값과 인접한 두 개의 점 (x+1, z), (z, z+1)에 대한 픽셀 값을 사용하여 법선 벡터를
	계산한다.*/
	int nHeightMapIndex = x + (z * m_nWidth);
	int xHeightMapAdd = (x < (m_nWidth - 1)) ? 1 : -1;
	int zHeightMapAdd = (z < (m_nLength - 1)) ? m_nWidth : -m_nWidth;

	//(x, z), (x+1, z), (z, z+1)의 픽셀에서 지형의 높이를 구한다.
	float y1 = (float)m_pHeightMapPixels[nHeightMapIndex] * m_xmf3Scale.y;
	float y2 = (float)m_pHeightMapPixels[nHeightMapIndex + xHeightMapAdd] * m_xmf3Scale.y;
	float y3 = (float)m_pHeightMapPixels[nHeightMapIndex + zHeightMapAdd] * m_xmf3Scale.y;

	//xmf3Edge1은 (0, y3, m_xmf3Scale.z) - (0, y1, 0) 벡터이다.
	XMFLOAT3 xmf3Edge1 = XMFLOAT3(0.0f, y3 - y1, m_xmf3Scale.z);

	//xmf3Edge2는 (m_xmf3Scale.x, y2, 0) - (0, y1, 0) 벡터이다.
	XMFLOAT3 xmf3Edge2 = XMFLOAT3(m_xmf3Scale.x, y2 - y1, 0.0f);

	//법선 벡터는 xmf3Edge1과 xmf3Edge2의 외적을 정규화하면 된다.
	XMFLOAT3 xmf3Normal = Vector3::CrossProduct(xmf3Edge1, xmf3Edge2, true);
	return(xmf3Normal);
}

#define _WITH_APPROXIMATE_OPPOSITE_CORNER
float CHeightMapImage::GetHeight(float fx, float fz)
{
	/*지형의 좌표 (fx, fz)는 이미지 좌표계이다. 높이 맵의 x-좌표와 z-좌표가 높이 맵의 범위를 벗어나면 지형의 높이는
	0이다.*/
	if ((fx < 0.0f) || (fz < 0.0f) || (fx >= m_nWidth) || (fz >= m_nLength))
		return(0.0f);

	//높이 맵의 좌표의 정수 부분과 소수 부분을 계산한다.
	int x = (int)fx;
	int z = (int)fz;
	float fxPercent = fx - x;
	float fzPercent = fz - z;
	float fBottomLeft = (float)m_pHeightMapPixels[x + (z * m_nWidth)];
	float fBottomRight = (float)m_pHeightMapPixels[(x + 1) + (z * m_nWidth)];
	float fTopLeft = (float)m_pHeightMapPixels[x + ((z + 1) * m_nWidth)];
	float fTopRight = (float)m_pHeightMapPixels[(x + 1) + ((z + 1) * m_nWidth)];

#ifdef _WITH_APPROXIMATE_OPPOSITE_CORNER
	//z-좌표가 1, 3, 5, ...인 경우 인덱스가 오른쪽에서 왼쪽으로 나열된다.
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
	//사각형의 네 점을 보간하여 높이(픽셀 값)를 계산한다.
	float fTopHeight = fTopLeft * (1 - fxPercent) + fTopRight * fxPercent;
	float fBottomHeight = fBottomLeft * (1 - fxPercent) + fBottomRight * fxPercent;
	float fHeight = fBottomHeight * (1 - fzPercent) + fTopHeight * fzPercent;
	return(fHeight);
}

CHeightMapGridMesh::CHeightMapGridMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, int xStart, int zStart, int nWidth, int nLength, float* pHeightmapData, float& fMaxHeight, float& fMinHeight, XMFLOAT3 xmf3Scale, void* pContext) : CMesh(pd3dDevice, pd3dCommandList)
{
	//격자의 교점(정점)의 개수는 (nWidth * nLength)이다.
	m_nVertices = nWidth * nLength;
	m_nStride = sizeof(CNormalVertex);
	//격자는 삼각형 스트립으로 구성한다.
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	m_nWidth = nWidth;
	m_nLength = nLength;
	m_xmf3Scale = xmf3Scale;
	CNormalVertex* pVertices = new CNormalVertex[m_nVertices];

	/*xStart와 zStart는 격자의 시작 위치(x-좌표와 z-좌표)를 나타낸다. 커다란 지형은 격자들의 이차원 배열로 만들 필
	요가 있기 때문에 전체 지형에서 각 격자의 시작 위치를 나타내는 정보가 필요하다.*/
	float fHeight = 0.0f;
	for (int i = 0, z = zStart; z < (zStart + nLength); z++)
	{
		for (int x = xStart; x < (xStart + nWidth); x++, i++)
		{
			//정점의 높이와 노멀을 높이 맵으로부터 구한다.
			fHeight = OnGetHeight(x, z, pContext);

			XMFLOAT3 xmf3Position = XMFLOAT3((x * m_xmf3Scale.x), fHeight, (z * m_xmf3Scale.z));
			XMFLOAT3 xmf3Normal = OnGetNormal(x, z, pContext);
			pVertices[i] = CNormalVertex(xmf3Position, xmf3Normal);

			if (fHeight / m_xmf3Scale.y < fMinHeight)
				fMinHeight = fHeight / m_xmf3Scale.y;

			if (fHeight / m_xmf3Scale.y > fMaxHeight)
				fMaxHeight = fHeight / m_xmf3Scale.y;

			pHeightmapData[z * nLength + x] = fHeight / m_xmf3Scale.y;
		}
	}
	//다음 그림은 격자의 교점(정점)을 나열하는 순서를 보여준다.
	m_pd3dVertexBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, pVertices, m_nStride * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dVertexUploadBuffer);
	m_d3dVertexBufferView.BufferLocation = m_pd3dVertexBuffer->GetGPUVirtualAddress();
	m_d3dVertexBufferView.StrideInBytes = m_nStride;
	m_d3dVertexBufferView.SizeInBytes = m_nStride * m_nVertices;
	delete[] pVertices;

	m_nIndices = ((nWidth * 2) * (nLength - 1)) + ((nLength - 1) - 1);
	UINT* pnIndices = new UINT[m_nIndices];

	for (int j = 0, z = 0; z < nLength - 1; z++)
	{
		if ((z % 2) == 0)
		{
			//홀수 번째 줄이므로(z = 0, 2, 4, ...) 인덱스의 나열 순서는 왼쪽에서 오른쪽 방향이다.
			for (int x = 0; x < nWidth; x++)
			{
				//첫 번째 줄을 제외하고 줄이 바뀔 때마다(x == 0) 첫 번째 인덱스를 추가한다.
				if ((x == 0) && (z > 0))
					pnIndices[j++] = (UINT)(x + (z * nWidth));

				//아래(x, z), 위(x, z+1)의 순서로 인덱스를 추가한다.
				pnIndices[j++] = (UINT)(x + (z * nWidth));
				pnIndices[j++] = (UINT)((x + (z * nWidth)) + nWidth);
			}
		}
		else
		{
			//짝수 번째 줄이므로(z = 1, 3, 5, ...) 인덱스의 나열 순서는 오른쪽에서 왼쪽 방향이다.
			for (int x = nWidth - 1; x >= 0; x--)
			{
				//줄이 바뀔 때마다(x == (nWidth-1)) 첫 번째 인덱스를 추가한다.
				if (x == (nWidth - 1)) pnIndices[j++] = (UINT)(x + (z * nWidth));
				//아래(x, z), 위(x, z+1)의 순서로 인덱스를 추가한다.
				pnIndices[j++] = (UINT)(x + (z * nWidth));
				pnIndices[j++] = (UINT)((x + (z * nWidth)) + nWidth);
			}
		}
	}
	m_pd3dIndexBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, pnIndices, sizeof(UINT) * m_nIndices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_INDEX_BUFFER, &m_pd3dIndexUploadBuffer);
	m_d3dIndexBufferView.BufferLocation = m_pd3dIndexBuffer->GetGPUVirtualAddress();
	m_d3dIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	m_d3dIndexBufferView.SizeInBytes = sizeof(UINT) * m_nIndices;
	delete[] pnIndices;
}

CHeightMapGridMesh::~CHeightMapGridMesh()
{
}

//높이 맵 이미지의 픽셀 값을 지형의 높이로 반환한다.
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
	//조명의 방향 벡터(정점에서 조명까지의 벡터)이다.
	XMFLOAT3 xmf3LightDirection = XMFLOAT3(-1.0f, 1.0f, 1.0f);
	xmf3LightDirection = Vector3::Normalize(xmf3LightDirection);
	CHeightMapImage* pHeightMapImage = (CHeightMapImage*)pContext;
	XMFLOAT3 xmf3Scale = pHeightMapImage->GetScale();

	//조명의 색상(세기, 밝기)이다.
	XMFLOAT4 xmf4IncidentLightColor(0.9f, 0.8f, 0.4f, 1.0f);
	float fScale = Vector3::DotProduct(pHeightMapImage->GetHeightMapNormal(x, z), xmf3LightDirection);
	fScale += Vector3::DotProduct(pHeightMapImage->GetHeightMapNormal(x + 1, z), xmf3LightDirection);
	fScale += Vector3::DotProduct(pHeightMapImage->GetHeightMapNormal(x + 1, z + 1), xmf3LightDirection);
	fScale += Vector3::DotProduct(pHeightMapImage->GetHeightMapNormal(x, z + 1), xmf3LightDirection);
	fScale = (fScale / 4.0f) + 0.05f;
	if (fScale > 1.0f) fScale = 1.0f;
	if (fScale < 0.25f) fScale = 0.25f;

	//fScale은 조명 색상(밝기)이 반사되는 비율이다.
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
};

void CMeshFileRead::Render(ID3D12GraphicsCommandList* pd3dCommandList)
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

	for (int i = 0; i < m_nVertices; ++i)
	{
		m_pxmf3Positions[i].x *= xmf3Scale.x;
		m_pxmf3Positions[i].y *= xmf3Scale.y;
		m_pxmf3Positions[i].z *= xmf3Scale.z;
	}

	m_xmBoundingBox.Extents.x *= xmf3Scale.x;
	m_xmBoundingBox.Extents.y *= xmf3Scale.y;
	m_xmBoundingBox.Extents.z *= xmf3Scale.z;

	m_pd3dPositionBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, m_pxmf3Positions, sizeof(XMFLOAT3) * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dPositionUploadBuffer);
	m_pd3dNormalBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, m_pxmf3Normals, sizeof(XMFLOAT3) * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dNormalUploadBuffer);

	m_nVertexBufferViews = 2;
	m_pd3dVertexBufferViews = new D3D12_VERTEX_BUFFER_VIEW[m_nVertexBufferViews];

	m_pd3dVertexBufferViews[0].BufferLocation = m_pd3dPositionBuffer->GetGPUVirtualAddress();
	m_pd3dVertexBufferViews[0].StrideInBytes = sizeof(XMFLOAT3);
	m_pd3dVertexBufferViews[0].SizeInBytes = sizeof(XMFLOAT3) * m_nVertices;

	m_pd3dVertexBufferViews[1].BufferLocation = m_pd3dNormalBuffer->GetGPUVirtualAddress(); 
	m_pd3dVertexBufferViews[1].StrideInBytes = sizeof(XMFLOAT3);
	m_pd3dVertexBufferViews[1].SizeInBytes = sizeof(XMFLOAT3) * m_nVertices;

	m_pd3dIndexBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, m_pnIndices, sizeof(UINT) * m_nIndices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_INDEX_BUFFER, &m_pd3dIndexUploadBuffer);

	m_d3dIndexBufferView.BufferLocation = m_pd3dIndexBuffer->GetGPUVirtualAddress();
	m_d3dIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	m_d3dIndexBufferView.SizeInBytes = sizeof(UINT) * m_nIndices;
}

