#include "stdafx.h"
#include "Player.h"
#include "Scene.h"
#include "Shader.h"

CPlayer::CPlayer(int nMeshes) : CGameObject(nMeshes)
{
	m_pCamera = NULL;
	m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_xmf3Gravity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_fMaxVelocityXZ = 0.0f;
	m_fMaxVelocityY = 0.0f;
	m_fFriction = 0.0f;
	m_fPitch = 0.0f;
	m_fRoll = 0.0f;
	m_fYaw = 0.0f;
	m_pPlayerUpdatedContext = NULL;
	m_pCameraUpdatedContext = NULL;
}

CPlayer::~CPlayer()
{
	ReleaseShaderVariables();
}

void CPlayer::CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	CGameObject::CreateShaderVariables(pd3dDevice, pd3dCommandList);

	if (m_pCamera)
		m_pCamera->CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CPlayer::ReleaseShaderVariables()
{
	CGameObject::ReleaseShaderVariables();
}

void CPlayer::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	CGameObject::UpdateShaderVariables(pd3dCommandList);
}

/*�÷��̾��� ��ġ�� �����ϴ� �Լ��̴�. �÷��̾��� ��ġ�� �⺻������ ����ڰ� �÷��̾ �̵��ϱ� ���� Ű���带
���� �� ����ȴ�. �÷��̾��� �̵� ����(dwDirection)�� ���� �÷��̾ fDistance ��ŭ �̵��Ѵ�.*/
void CPlayer::Move(DWORD dwDirection, float fDistance, bool bUpdateVelocity)
{
	if (dwDirection)
	{
		XMFLOAT3 xmf3Shift = XMFLOAT3(0, 0, 0);
		//ȭ��ǥ Ű ���衯�� ������ ���� z-�� �������� �̵�(����)�Ѵ�. ���顯�� ������ �ݴ� �������� �̵��Ѵ�.
		if (dwDirection & KEY_FORWARD)
			xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, fDistance);

		if (dwDirection & KEY_BACKWARD)
			xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, -fDistance);

		//ȭ��ǥ Ű ���桯�� ������ ���� x-�� �������� �̵��Ѵ�. ���硯�� ������ �ݴ� �������� �̵��Ѵ�.
		if (dwDirection & KEY_RIGHT) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, fDistance);
		if (dwDirection & KEY_LEFT) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, -fDistance);

		//��Page Up���� ������ ���� y-�� �������� �̵��Ѵ�. ��Page Down���� ������ �ݴ� �������� �̵��Ѵ�.
		if (dwDirection & KEY_UP)
			xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, fDistance);

		if (dwDirection & KEY_DOWN)
			xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, -fDistance);
		//�÷��̾ ���� ��ġ ���Ϳ��� xmf3Shift ���͸�ŭ �̵��Ѵ�.
		Move(xmf3Shift, bUpdateVelocity);
	}
}


void CPlayer::Move(XMFLOAT3& xmf3Shift, bool bUpdateVelocity)
{
	//bUpdateVelocity�� ���̸� �÷��̾ �̵����� �ʰ� �ӵ� ���͸� �����Ѵ�.
	if (bUpdateVelocity)
	{
		//�÷��̾��� �ӵ� ���͸� xmf3Shift ���͸�ŭ �����Ѵ�.
		m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, xmf3Shift);
	}
	else
	{
		//�÷��̾ ���� ��ġ ���Ϳ��� xmf3Shift ���͸�ŭ �̵��Ѵ�.
		m_xmf3Position = Vector3::Add(m_xmf3Position, xmf3Shift);

		//�÷��̾��� ��ġ�� ����Ǿ����Ƿ� ī�޶��� ��ġ�� xmf3Shift ���͸�ŭ �̵��Ѵ�.
		if (m_pCamera)
			m_pCamera->Move(xmf3Shift);
	}
}

//�÷��̾ ���� x-��, y-��, z-���� �߽����� ȸ���Ѵ�.
void CPlayer::Rotate(float x, float y, float z)
{
	DWORD nCameraMode = m_pCamera->GetMode();

	//1��Ī ī�޶� �Ǵ� 3��Ī ī�޶��� ��� �÷��̾��� ȸ���� �ణ�� ������ ������.
	if ((nCameraMode == FIRST_PERSON_CAMERA))
	{
		/*���� x-���� �߽����� ȸ���ϴ� ���� ���� �յڷ� ���̴� ���ۿ� �ش��Ѵ�. �׷��Ƿ� x-���� �߽����� ȸ���ϴ�
		������ -89.0~+89.0�� ���̷� �����Ѵ�. x�� ������ m_fPitch���� ���� ȸ���ϴ� �����̹Ƿ� x��ŭ ȸ���� ����
		Pitch�� +89�� ���� ũ�ų� -89�� ���� ������ m_fPitch�� +89�� �Ǵ� -89���� �ǵ��� ȸ������(x)�� �����Ѵ�.*/
		if (x != 0.0f)
		{
			m_fPitch += x;
			if (m_fPitch > +89.0f) { x -= (m_fPitch - 89.0f); m_fPitch = +89.0f; }if (m_fPitch < -89.0f) { x -= (m_fPitch + 89.0f); m_fPitch = -89.0f; }
		}
		if (y != 0.0f)
		{
			//���� y-���� �߽����� ȸ���ϴ� ���� ������ ������ ���̹Ƿ� ȸ�� ������ ������ ����.
			m_fYaw += y;
			if (m_fYaw > 360.0f) m_fYaw -= 360.0f;
			if (m_fYaw < 0.0f) m_fYaw += 360.0f;
		}
		if (z != 0.0f)
		{
			/*���� z-���� �߽����� ȸ���ϴ� ���� ������ �¿�� ����̴� ���̹Ƿ� ȸ�� ������ -20.0~+20.0�� ���̷� ���ѵ�
			��. z�� ������ m_fRoll���� ���� ȸ���ϴ� �����̹Ƿ� z��ŭ ȸ���� ���� m_fRoll�� +20�� ���� ũ�ų� -20������
			������ m_fRoll�� +20�� �Ǵ� -20���� �ǵ��� ȸ������(z)�� �����Ѵ�.*/
			m_fRoll += z;
			if (m_fRoll > +20.0f){ z -= (m_fRoll - 20.0f); m_fRoll = +20.0f; }
			if (m_fRoll < -20.0f) { z -= (m_fRoll + 20.0f); m_fRoll = -20.0f; }
		}
		//ī�޶� x, y, z ��ŭ ȸ���Ѵ�. �÷��̾ ȸ���ϸ� ī�޶� ȸ���ϰ� �ȴ�.
		m_pCamera->Rotate(x, y, z);

		/*�÷��̾ ȸ���Ѵ�. 1��Ī ī�޶� �Ǵ� 3��Ī ī�޶󿡼� �÷��̾��� ȸ���� ���� y-�࿡���� �Ͼ��. �÷��̾�
		�� ���� y-��(Up ����)�� �������� ���� z-��(Look ����)�� ���� x-��(Right ����)�� ȸ����Ų��. �⺻������ Up ��
		�͸� �������� ȸ���ϴ� ���� �÷��̾ �ȹٷ� ���ִ� ���� �����Ѵٴ� �ǹ��̴�.*/
		if (y != 0.0f)
		{
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Up), XMConvertToRadians(y));
			m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
			m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
		}
	}
	else if (nCameraMode == SPACESHIP_CAMERA || nCameraMode == THIRD_PERSON_CAMERA)
	{
		/*�����̽�-�� ī�޶󿡼� �÷��̾��� ȸ���� ȸ�� ������ ������ ����. �׸��� ��� ���� �߽����� ȸ���� �� �� ��
		��.*/
		m_pCamera->Rotate(x, y, z);
		if (x != 0.0f)
		{
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Right), XMConvertToRadians(x));
			m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
			m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, xmmtxRotate);
		}
		if (y != 0.0f)
		{
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Up), XMConvertToRadians(y));
			m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
			m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
		}
		if (z != 0.0f)
		{
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Look), XMConvertToRadians(z));
			m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, xmmtxRotate);
			m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
		}
	}

	/*ȸ������ ���� �÷��̾��� ���� x-��, y-��, z-���� ���� �������� ���� �� �����Ƿ� z-��(Look ����)�� ��������
	�Ͽ� ���� �����ϰ� �������Ͱ� �ǵ��� �Ѵ�.*/

	m_xmf3Look = Vector3::Normalize(m_xmf3Look);
	m_xmf3Right = Vector3::CrossProduct(m_xmf3Up, m_xmf3Look, true);
	m_xmf3Up = Vector3::CrossProduct(m_xmf3Look, m_xmf3Right, true);
}

//�� �Լ��� �� �����Ӹ��� ȣ��ȴ�. �÷��̾��� �ӵ� ���Ϳ� �߷°� ������ ���� �����Ѵ�.
void CPlayer::Update(float fTimeElapsed)
{
	/*�÷��̾��� �ӵ� ���͸� �߷� ���Ϳ� ���Ѵ�. �߷� ���Ϳ� fTimeElapsed�� ���ϴ� ���� �߷��� �ð��� ����ϵ���
	�����Ѵٴ� �ǹ��̴�.*/

	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, Vector3::ScalarProduct(m_xmf3Gravity, fTimeElapsed, false));
	/*�÷��̾��� �ӵ� ������ XZ-������ ũ�⸦ ���Ѵ�. �̰��� XZ-����� �ִ� �ӷº��� ũ�� �ӵ� ������ x�� z-����
	������ �����Ѵ�.*/

	float fLength = sqrtf(m_xmf3Velocity.x * m_xmf3Velocity.x + m_xmf3Velocity.z *m_xmf3Velocity.z);
	float fMaxVelocityXZ = m_fMaxVelocityXZ * fTimeElapsed;
	if (fLength > m_fMaxVelocityXZ)
	{
		m_xmf3Velocity.x *= (fMaxVelocityXZ / fLength);
		m_xmf3Velocity.z *= (fMaxVelocityXZ / fLength);
	}

	/*�÷��̾��� �ӵ� ������ y-������ ũ�⸦ ���Ѵ�. �̰��� y-�� ������ �ִ� �ӷº��� ũ�� �ӵ� ������ y-���� ������ �����Ѵ�.*/
	float fMaxVelocityY = m_fMaxVelocityY * fTimeElapsed;
	fLength = sqrtf(m_xmf3Velocity.y * m_xmf3Velocity.y);
	if (fLength > m_fMaxVelocityY) m_xmf3Velocity.y *= (fMaxVelocityY / fLength);

	//�÷��̾ �ӵ� ���� ��ŭ ������ �̵��Ѵ�(ī�޶� �̵��� ���̴�). 
	XMFLOAT3 xmf3Velocity = Vector3::ScalarProduct(m_xmf3Velocity, fTimeElapsed, false);
	Move(xmf3Velocity, false);

	/*�÷��̾��� ��ġ�� ����� �� �߰��� ������ �۾��� �����Ѵ�. �÷��̾��� ���ο� ��ġ�� ��ȿ�� ��ġ�� �ƴ� ����
	�ְ� �Ǵ� �÷��̾��� �浹 �˻� ���� ������ �ʿ䰡 �ִ�. �̷��� ��Ȳ���� �÷��̾��� ��ġ�� ��ȿ�� ��ġ�� �ٽ�
	������ �� �ִ�.*/
	if (m_pPlayerUpdatedContext) OnPlayerUpdateCallback(fTimeElapsed);
	DWORD nCameraMode = m_pCamera->GetMode();

	//�÷��̾��� ��ġ�� ����Ǿ����Ƿ� 3��Ī ī�޶� �����Ѵ�.
	if (nCameraMode == THIRD_PERSON_CAMERA) m_pCamera->Update(m_xmf3Position, fTimeElapsed);

	//ī�޶��� ��ġ�� ����� �� �߰��� ������ �۾��� �����Ѵ�.
	if (m_pCameraUpdatedContext) OnCameraUpdateCallback(fTimeElapsed);

	//ī�޶� 3��Ī ī�޶��̸� ī�޶� ����� �÷��̾� ��ġ�� �ٶ󺸵��� �Ѵ�.
	if (nCameraMode == THIRD_PERSON_CAMERA)
		m_pCamera->SetLookAt(m_xmf3Position);

	//ī�޶��� ī�޶� ��ȯ ����� �ٽ� �����Ѵ�.
	m_pCamera->RegenerateViewMatrix();

	/*�÷��̾��� �ӵ� ���Ͱ� ������ ������ ������ �Ǿ�� �Ѵٸ� ���� ���͸� �����Ѵ�. �ӵ� ������ �ݴ� ���� ���͸�
	���ϰ� ���� ���ͷ� �����. ���� ����� �ð��� ����ϵ��� �Ͽ� �������� ���Ѵ�. ���� ���Ϳ� �������� ���Ͽ� ��
	�� ���͸� ���Ѵ�. �ӵ� ���Ϳ� ���� ���͸� ���Ͽ� �ӵ� ���͸� ���δ�. �������� �ӷº��� ũ�� �ӷ��� 0�� �� ����
	��.*/

	fLength = Vector3::Length(m_xmf3Velocity);
	float fDeceleration = (m_fFriction * fTimeElapsed);

	if (fDeceleration > fLength)
		fDeceleration = fLength;

	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, Vector3::ScalarProduct(m_xmf3Velocity, -fDeceleration, true));
}

/*ī�޶� ������ �� ChangeCamera() �Լ����� ȣ��Ǵ� �Լ��̴�. nCurrentCameraMode�� ���� ī�޶��� ���
�̰� nNewCameraMode�� ���� ������ ī�޶� ����̴�.*/
CCamera* CPlayer::OnChangeCamera(DWORD nNewCameraMode, DWORD nCurrentCameraMode)
{
	//���ο� ī�޶��� ��忡 ���� ī�޶� ���� �����Ѵ�.
	CCamera *pNewCamera = NULL;
	switch (nNewCameraMode)
	{
	case FIRST_PERSON_CAMERA:
		pNewCamera = new CFirstPersonCamera(m_pCamera);
		break;
	case THIRD_PERSON_CAMERA:
		pNewCamera = new CThirdPersonCamera(m_pCamera);
		break;
	case SPACESHIP_CAMERA:
		pNewCamera = new CSpaceShipCamera(m_pCamera);
		break;
	}
	/*���� ī�޶��� ��尡 �����̽�-�� ����� ī�޶��̰� ���ο� ī�޶� 1��Ī �Ǵ� 3��Ī ī�޶��̸� �÷��̾���
	Up ���͸� ������ǥ���� y-�� ���� ����(0, 1, 0)�� �ǵ��� �Ѵ�. ��, �ȹٷ� ������ �Ѵ�. �׸��� �����̽�-�� ī��
	���� ��� �÷��̾��� �̵����� ������ ����. Ư��, y-�� ������ �������� �����Ӵ�. �׷��Ƿ� �÷��̾��� ��ġ�� ��
	��(��ġ ������ y-��ǥ�� 0���� ũ��)�� �� �� �ִ�. �̶� ���ο� ī�޶� 1��Ī �Ǵ� 3��Ī ī�޶��̸� �÷��̾���
	��ġ�� ������ �Ǿ�� �Ѵ�. �׷��Ƿ� �÷��̾��� Right ���Ϳ� Look ������ y ���� 0���� �����. ���� �÷��̾���
	Right ���Ϳ� Look ���ʹ� �������Ͱ� �ƴϹǷ� ����ȭ�Ѵ�.*/
	if (nCurrentCameraMode == SPACESHIP_CAMERA)
	{
		m_xmf3Right = Vector3::Normalize(XMFLOAT3(m_xmf3Right.x, 0.0f, m_xmf3Right.z));
		m_xmf3Up = Vector3::Normalize(XMFLOAT3(0.0f, 1.0f, 0.0f));
		m_xmf3Look = Vector3::Normalize(XMFLOAT3(m_xmf3Look.x, 0.0f, m_xmf3Look.z));
		m_fPitch = 0.0f;
		m_fRoll = 0.0f;
		/*Look ���Ϳ� ������ǥ���� z-��(0, 0, 1)�� �̷�� ����(����=cos)�� ����Ͽ� �÷��̾��� y-���� ȸ�� ����
		m_fYaw�� �����Ѵ�.*/
		m_fYaw = Vector3::Angle(XMFLOAT3(0.0f, 0.0f, 1.0f), m_xmf3Look);
		if (m_xmf3Look.x < 0.0f) m_fYaw = -m_fYaw;
	}

	else if ((nNewCameraMode == SPACESHIP_CAMERA) && m_pCamera)
	{
		/*���ο� ī�޶��� ��尡 �����̽�-�� ����� ī�޶��̰� ���� ī�޶� ��尡 1��Ī �Ǵ� 3��Ī ī�޶��̸� �÷���
		���� ���� ���� ���� ī�޶��� ���� ��� ���� �����.*/
		m_xmf3Right = m_pCamera->GetRightVector();
		m_xmf3Up = m_pCamera->GetUpVector();
		m_xmf3Look = m_pCamera->GetLookVector();
	}
	if (pNewCamera)
	{
		pNewCamera->SetMode(nNewCameraMode);

		//���� ī�޶� ����ϴ� �÷��̾� ��ü�� �����Ѵ�.
		pNewCamera->SetPlayer(this);
	}

	if (m_pCamera)
		delete m_pCamera;

	return(pNewCamera);
}

/*�÷��̾��� ��ġ�� ȸ�������κ��� ���� ��ȯ ����� �����ϴ� �Լ��̴�. �÷��̾��� Right ���Ͱ� ���� ��ȯ ���
�� ù ��° �� ����, Up ���Ͱ� �� ��° �� ����, Look ���Ͱ� �� ��° �� ����, �÷��̾��� ��ġ ���Ͱ� �� ��° ��
���Ͱ� �ȴ�.*/
void CPlayer::OnPrepareRender()
{
	m_xmf4x4World._11 = m_xmf3Right.x;
	m_xmf4x4World._12 = m_xmf3Right.y;
	m_xmf4x4World._13 = m_xmf3Right.z;
	m_xmf4x4World._21 = m_xmf3Up.x;
	m_xmf4x4World._22 = m_xmf3Up.y;
	m_xmf4x4World._23 = m_xmf3Up.z;
	m_xmf4x4World._31 = m_xmf3Look.x;
	m_xmf4x4World._32 = m_xmf3Look.y;
	m_xmf4x4World._33 = m_xmf3Look.z;
	m_xmf4x4World._41 = m_xmf3Position.x;
	m_xmf4x4World._42 = m_xmf3Position.y;
	m_xmf4x4World._43 = m_xmf3Position.z;
}

void CPlayer::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	DWORD nCameraMode = (pCamera) ? pCamera->GetMode() : 0x00;

	//ī�޶� ��尡 3��Ī�̸� �÷��̾� ��ü�� �������Ѵ�.
	if (nCameraMode == THIRD_PERSON_CAMERA || nCameraMode == SPACESHIP_CAMERA)
	{
		if (m_pShader) m_pShader->Render(pd3dCommandList);
		CGameObject::Render(pd3dCommandList);
	}
}

CVehiclePlayer::CVehiclePlayer(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, btAlignedObjectArray<btCollisionShape*>& btCollisionShapes, btDiscreteDynamicsWorld* pbtDynamicsWorld, CShader* pShader, int nMeshes) :
	CCubeMappingPlayer(pd3dDevice, pd3dCommandList, 256, pShader, nMeshes)
{
	int SteeringIndex = 5;
	std::shared_ptr<CMeshFileRead> pVehicleMesh = std::make_shared<CMeshFileRead>(pd3dDevice, pd3dCommandList, (char*)"Models/FlyerPlayership.bin", false);
	std::shared_ptr<CMeshFileRead> pWheelMesh = std::make_shared<CMeshFileRead>(pd3dDevice, pd3dCommandList, (char*)"Models/Tire.bin", false, XMFLOAT3{ 10.0f, 10.0f, 10.0f }, XMFLOAT3{0.0f, 0.0f, 90.0f});

	SetMesh(pVehicleMesh);

	SetMaterial(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f), XMFLOAT3(0.6f, 0.6f, 0.6f), 0.8f);

	m_pCamera = ChangeCamera(THIRD_PERSON_CAMERA, 0.0f);
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
	SetPosition(XMFLOAT3(2000.0f, 5.0f, 2050.0f));

	auto vehicleExtents = pVehicleMesh.get()[0].GetBoundingBox().Extents;
	auto wheelExtents = pWheelMesh.get()[0].GetBoundingBox().Extents;

	btCollisionShape* chassisShape = new btBoxShape(btVector3(vehicleExtents.x, vehicleExtents.y, vehicleExtents.z));
	btCollisionShapes.push_back(chassisShape);

	btTransform btCarTransform;
	btCarTransform.setIdentity();
	btCarTransform.setOrigin(btVector3(m_xmf3Position.x, m_xmf3Position.y, m_xmf3Position.z));

	m_pbtRigidBody = BulletHelper::CreateRigidBody(1000.0f, btCarTransform, chassisShape, pbtDynamicsWorld);

	btTransform btCenterOfMassTransform;
	btCenterOfMassTransform.setIdentity();
	btCenterOfMassTransform.setOrigin(btVector3(m_xmf3Position.x, m_xmf3Position.y - 4.0f, m_xmf3Position.z));
	m_pbtRigidBody->setCenterOfMassTransform(btCenterOfMassTransform);

	m_vehicleRayCaster = new btDefaultVehicleRaycaster(pbtDynamicsWorld);
	m_vehicle = new btRaycastVehicle(m_tuning, m_pbtRigidBody, m_vehicleRayCaster);

	m_pbtRigidBody->setActivationState(DISABLE_DEACTIVATION);
	pbtDynamicsWorld->addVehicle(m_vehicle);

	float connectionHeight = 0.0f;

	m_vehicle->setCoordinateSystem(0, 1, 2);

	btVector3 wheelDirectionCS0(0, -1, 0);
	btVector3 wheelAxleCS(-1, 0, 0);

	float wheelWidth = wheelExtents.x;
	float wheelRadius = wheelExtents.z;
	float wheelFriction = 500;  //BT_LARGE_FLOAT;
	float suspensionStiffness = 20.f;
	float suspensionDamping = 2.3f;
	float suspensionCompression = 4.4f;
	float rollInfluence = 0.01f;  //1.0f;

	// �չ���
	bool isFrontWheel = true;

	btVector3 connectionPointCS0(vehicleExtents.x - (0.3 * wheelWidth), connectionHeight, vehicleExtents.z - wheelRadius);
	m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, 0.6, wheelRadius, m_tuning, isFrontWheel);

	connectionPointCS0 = btVector3(-vehicleExtents.x + (0.3 * wheelWidth), connectionHeight, vehicleExtents.z - wheelRadius);
	m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, 0.6, wheelRadius, m_tuning, isFrontWheel);

	// �޹���
	isFrontWheel = false;
	
	connectionPointCS0 = btVector3(-vehicleExtents.x + (0.3 * wheelWidth), connectionHeight, -vehicleExtents.z + wheelRadius);
	m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, 0.6, wheelRadius, m_tuning, isFrontWheel);

	connectionPointCS0 = btVector3(vehicleExtents.x - (0.3 * wheelWidth), connectionHeight, -vehicleExtents.z + wheelRadius);
	m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, 0.6, wheelRadius, m_tuning, isFrontWheel);

	for (int i = 0; i < m_vehicle->getNumWheels(); i++)
	{
		btWheelInfo& wheel = m_vehicle->getWheelInfo(i);
		wheel.m_suspensionStiffness = suspensionStiffness;
		wheel.m_wheelsDampingRelaxation = suspensionDamping;
		wheel.m_wheelsDampingCompression = suspensionCompression;
		wheel.m_frictionSlip = wheelFriction;
		wheel.m_rollInfluence = rollInfluence;
	}

	pShader->CreateShader(pd3dDevice, pd3dGraphicsRootSignature);
	SetShader(pShader);

	for (int i = 0; i < 4; ++i)
	{
		m_pWheel[i] = std::make_shared<CWheel>(pWheelMesh);
		m_pWheel[i]->SetShader(pShader);
	}
}

CVehiclePlayer::~CVehiclePlayer()
{
	ReleaseShaderVariables();
	if (m_pCamera) delete m_pCamera;
}

void CVehiclePlayer::Update(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, float fTimeElapsed, btDiscreteDynamicsWorld* pbtDynamicsWorld, DWORD dwBehave)
{
	if (m_gVehicleSteering > 0)
	{
		m_gVehicleSteering -= m_steeringIncrement;
		if (m_gVehicleSteering < 0)
		{
			m_gVehicleSteering = 0;
		}
	}

	else if (m_gVehicleSteering < 0)
	{
		m_gVehicleSteering += m_steeringIncrement;
		if (m_gVehicleSteering > 0)
		{
			m_gVehicleSteering = 0;
		}
	}

	m_gBreakingForce = 0.0f;

	if (m_gEngineForce > 0.0f)
	{
		m_gEngineForce -= 100.0f;
		if (m_gEngineForce < 0.0f)
			m_gEngineForce = 0.0f;
	}


	if (dwBehave & KEY_LEFT)
	{
		m_gVehicleSteering -= m_steeringIncrement * 2;
		if (m_gVehicleSteering < -m_steeringClamp)
			m_gVehicleSteering = -m_steeringClamp;
	}

	if (dwBehave & KEY_RIGHT)
	{
		m_gVehicleSteering += m_steeringIncrement * 2;
		if (m_gVehicleSteering > m_steeringClamp)
			m_gVehicleSteering = m_steeringClamp;
	}

	if (dwBehave & KEY_FORWARD)
	{
		m_gEngineForce = m_maxEngineForce;
	}

	if (dwBehave & KEY_BACKWARD)
	{
		m_gEngineForce = -m_maxEngineForce;
	}

	if (dwBehave & KEY_SHIFT)
	{
		m_gBreakingForce = 20.0f;

		m_vehicle->getWheelInfo(2).m_frictionSlip = 20;
		m_vehicle->getWheelInfo(3).m_frictionSlip = 20;
	}
	else
	{
		m_gBreakingForce = 0.0f;

		for(int i = 0; i < 4; ++i)
			m_vehicle->getWheelInfo(i).m_frictionSlip = 500;
	}
	
	if (dwBehave & KEY_X)
	{
		if(m_pBullet == NULL)
			FireBullet(pd3dDevice, pd3dCommandList, pbtDynamicsWorld);
	}


	int wheelIndex = 2;
	m_vehicle->applyEngineForce(m_gEngineForce, wheelIndex);
	m_vehicle->setBrake(m_gBreakingForce, wheelIndex);
	wheelIndex = 3;
	m_vehicle->applyEngineForce(m_gEngineForce, wheelIndex);
	m_vehicle->setBrake(m_gBreakingForce, wheelIndex);

	wheelIndex = 0;
	m_vehicle->setSteeringValue(m_gVehicleSteering, wheelIndex);
	wheelIndex = 1;
	m_vehicle->setSteeringValue(m_gVehicleSteering, wheelIndex);


	btScalar m[16];
	btTransform btMat;
	m_vehicle->getRigidBody()->getMotionState()->getWorldTransform(btMat);
	btMat.getOpenGLMatrix(m);

	m_xmf4x4World = Matrix4x4::glMatrixToD3DMatrix(m);

	m_xmf3Position = XMFLOAT3(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43);
	m_xmf3Look = XMFLOAT3(m_xmf4x4World._31, m_xmf4x4World._32, m_xmf4x4World._33);
	m_xmf3Up = XMFLOAT3(m_xmf4x4World._21, m_xmf4x4World._22, m_xmf4x4World._23);
	m_xmf3Right = XMFLOAT3(m_xmf4x4World._11, m_xmf4x4World._12, m_xmf4x4World._13);


	m_pCamera->Update(m_xmf3Position, fTimeElapsed);


	for (int i = 0; i < 4; ++i)
	{
		m_pWheel[i]->Update(fTimeElapsed, m_vehicle, i);
	}

	if (m_pBullet)
	{
		m_pBullet->Update(fTimeElapsed, pbtDynamicsWorld);
		if (m_pBullet->GetTimeRemain() < 0.0f)
		{
			m_pBullet = NULL;
		}
	}

	//ī�޶��� ī�޶� ��ȯ ����� �ٽ� �����Ѵ�.
	m_pCamera->RegenerateViewMatrix();
}

void CVehiclePlayer::UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList)
{
	CGameObject::UpdateShaderVariables(pd3dCommandList);
}

void CVehiclePlayer::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	OnPrepareRender();
	UpdateShaderVariables(pd3dCommandList);
	if (m_pShader) m_pShader->Render(pd3dCommandList);

	CGameObject::Render(pd3dCommandList);

	for (int i = 0; i < 4; ++i)
	{
		m_pWheel[i]->Render(pd3dCommandList);
	}

	if (m_pBullet)
	{
		m_pBullet->Render(pd3dCommandList);
	}
}

void CVehiclePlayer::ReflectedRender(ID3D12GraphicsCommandList* pd3dCommandList, float mirrorZ)
{
	OnPrepareRender();
	UpdateReflectedShaderVariables(pd3dCommandList, mirrorZ);
	if (m_pShader) m_pShader->ReflectedRender(pd3dCommandList);

	for (int i = 0; i < m_vpMeshes.size(); i++)
	{
		if (m_vpMeshes[i]) m_vpMeshes[i]->Render(pd3dCommandList);
	}

	//���� ��ü�� �����ϴ� ��� �޽��� �������Ѵ�.
	for (int i = 0; i < m_vpMeshes.size(); i++)
	{
		if (m_vpMeshes[i]) m_vpMeshes[i]->Render(pd3dCommandList);
	}

	for (int i = 0; i < 4; ++i)
	{
		m_pWheel[i]->ReflectedRender(pd3dCommandList, mirrorZ);
	}
}

void CVehiclePlayer::UpdateReflectedShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList, float mirrorZ)
{
	XMVECTOR mirrorZPlane = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	XMMATRIX ZR = XMMatrixTranslation(0.0f, 0.0f, 2 * (mirrorZ - GetPosition().z));
	
	XMFLOAT4X4 xmf4x4World = m_xmf4x4World;
	
	xmf4x4World._33 = -m_xmf3Look.z;
	
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&xmf4x4World) * ZR));


	//��ü�� ���� ��ȯ ����� ��Ʈ ���(32-��Ʈ ��)�� ���Ͽ� ���̴� ����(��� ����)�� �����Ѵ�.
	pd3dCommandList->SetGraphicsRoot32BitConstants(0, 16, &xmf4x4World, 0);
	pd3dCommandList->SetGraphicsRoot32BitConstants(0, 12, &m_material, 16);
}

CCamera* CVehiclePlayer::ChangeCamera(DWORD nNewCameraMode, float fTimeElapsed)
{
	DWORD nCurrentCameraMode = (m_pCamera) ? m_pCamera->GetMode() : 0x00;

	if (nCurrentCameraMode == nNewCameraMode)
		return(m_pCamera);

	switch (nNewCameraMode)
	{
	case FIRST_PERSON_CAMERA:
		//�÷��̾��� Ư���� 1��Ī ī�޶� ��忡 �°� �����Ѵ�. �߷��� �������� �ʴ´�.
		SetFriction(200.0f);
		SetGravity(XMFLOAT3(0.0f, 0.0f, 0.0f));
		SetMaxVelocityXZ(125.0f);
		SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(FIRST_PERSON_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(0.0f);
		m_pCamera->SetOffset(XMFLOAT3(0.0f, 20.0f, 0.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
		m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
		m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
		break;
	case SPACESHIP_CAMERA:
		//�÷��̾��� Ư���� �����̽�-�� ī�޶� ��忡 �°� �����Ѵ�. �߷��� �������� �ʴ´�.
		SetFriction(125.0f);
		SetGravity(XMFLOAT3(0.0f, 0.0f, 0.0f));
		SetMaxVelocityXZ(400.0f);
		SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(SPACESHIP_CAMERA, nCurrentCameraMode);
		m_pCamera->SetTimeLag(0.0f);
		m_pCamera->SetOffset(XMFLOAT3(0.0f, 5.0f, -13.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
		m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
		m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
		break;
	case THIRD_PERSON_CAMERA:

		//�÷��̾��� Ư���� 3��Ī ī�޶� ��忡 �°� �����Ѵ�. ���� ȿ���� ī�޶� �������� �����Ѵ�.
		SetFriction(250.0f);
		SetGravity(XMFLOAT3(0.0f, 0.0f, 0.0f));
		SetMaxVelocityXZ(125.0f);
		SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(THIRD_PERSON_CAMERA, nCurrentCameraMode);

		//3��Ī ī�޶��� ���� ȿ���� �����Ѵ�. ���� 0.25f ��ſ� 0.0f�� 1.0f�� ������ ����� ���ϱ� �ٶ���.
		m_pCamera->SetTimeLag(0.25f);
		m_pCamera->SetOffset(XMFLOAT3(0.0f, 20.0f, -50.0f));
		m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
		m_pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
		m_pCamera->SetScissorRect(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
		break;
	default:
		break;
	}
	m_pCamera->SetPosition(Vector3::Add(m_xmf3Position, m_pCamera->GetOffset()));

	return(m_pCamera);
}

void CVehiclePlayer::FireBullet(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, btDiscreteDynamicsWorld* pbtDynamicsWorld)
{
	m_pBullet = std::make_shared<CBullet>(pd3dDevice, pd3dCommandList, m_xmf3Position, m_vehicle->getForwardVector(), pbtDynamicsWorld);
}

CVehiclePlayer::CWheel::CWheel(std::shared_ptr<CMeshFileRead> pWheelMesh)
{
	SetMaterial(XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), XMFLOAT3(0.3f, 0.3f, 0.3f), 0.0f);
	SetMesh(pWheelMesh);
}

CVehiclePlayer::CWheel::~CWheel()
{
}

void CVehiclePlayer::CWheel::Render(ID3D12GraphicsCommandList* pd3dCommandList)
{
	OnPrepareRender();
	UpdateShaderVariables(pd3dCommandList);

	//���� ��ü�� �����ϴ� ��� �޽��� �������Ѵ�.
	for (int i = 0; i < m_vpMeshes.size(); i++)
	{
		m_vpMeshes[i]->Render(pd3dCommandList);
	}
}

void CVehiclePlayer::CWheel::UpdateReflectedShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList, float mirrorZ)
{
	XMVECTOR mirrorZPlane = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	XMMATRIX ZR = XMMatrixTranslation(0.0f, 0.0f, 2 * (mirrorZ - GetPosition().z));

	XMFLOAT4X4 xmf4x4World = m_xmf4x4World;
;
	
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&xmf4x4World) * ZR ));

	//��ü�� ���� ��ȯ ����� ��Ʈ ���(32-��Ʈ ��)�� ���Ͽ� ���̴� ����(��� ����)�� �����Ѵ�.
	pd3dCommandList->SetGraphicsRoot32BitConstants(0, 16, &xmf4x4World, 0);
	pd3dCommandList->SetGraphicsRoot32BitConstants(0, 12, &m_material, 16);
}

void CVehiclePlayer::CWheel::ReflectedRender(ID3D12GraphicsCommandList* pd3dCommandList, float mirrorZ)
{
	OnPrepareRender();
	UpdateReflectedShaderVariables(pd3dCommandList, mirrorZ);

	//���� ��ü�� �����ϴ� ��� �޽��� �������Ѵ�.
	for (int i = 0; i < m_vpMeshes.size(); i++)
	{
		m_vpMeshes[i]->Render(pd3dCommandList);
	}
}

void CVehiclePlayer::CWheel::Update(float fTimeElapsed, btRaycastVehicle* pbtVehicle, int index)
{
	btTransform wheelTransform = pbtVehicle->getWheelTransformWS(index);

	btScalar m[16];
	wheelTransform.getOpenGLMatrix(m);
	m_xmf4x4World = Matrix4x4::glMatrixToD3DMatrix(m);
}

CCubeMappingPlayer::CCubeMappingPlayer(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, LONG nCubeMapSize, CShader* pShader, int nMeshes) : CPlayer(nMeshes)
{
	D3D12_DESCRIPTOR_HEAP_DESC d3dDescriptorHeapDesc;
	d3dDescriptorHeapDesc.NumDescriptors = 1;
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	d3dDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	d3dDescriptorHeapDesc.NodeMask = 0;
	HRESULT hResult = pd3dDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&m_pd3dDsvDescriptorHeap);

	D3D12_CPU_DESCRIPTOR_HANDLE d3dDsvCPUDescriptorHandle = m_pd3dDsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

	d3dDescriptorHeapDesc.NumDescriptors = 6;
	d3dDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	pd3dDevice->CreateDescriptorHeap(&d3dDescriptorHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&m_pd3dRtvDescriptorHeap);

	D3D12_CPU_DESCRIPTOR_HANDLE d3dRtvCPUDescriptorHandle = m_pd3dRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();


	for (int i = 0; i < 6; i++)
	{
		m_apCameras[i] = std::make_shared<CCamera>();
		m_apCameras[i]->SetViewport(0, 0, nCubeMapSize, nCubeMapSize, 0.0f, 1.0f);
		m_apCameras[i]->SetScissorRect(0, 0, nCubeMapSize, nCubeMapSize);
		m_apCameras[i]->CreateShaderVariables(pd3dDevice, pd3dCommandList);
		m_apCameras[i]->GenerateProjectionMatrix(0.1f, 5000.0f, 1.0f/*Aspect Ratio*/, 90.0f/*FOV*/);
	}

	//Depth Buffer & View
	D3D12_CLEAR_VALUE d3dDsbClearValue = { DXGI_FORMAT_D24_UNORM_S8_UINT, { 1.0f, 0 } };
	m_pd3dDepthStencilBuffer = ::CreateTexture2DResource(pd3dDevice, pd3dCommandList, nCubeMapSize, nCubeMapSize, 1, 1, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL, D3D12_RESOURCE_STATE_DEPTH_WRITE, &d3dDsbClearValue);

	m_d3dDsvCPUDescriptorHandle = d3dDsvCPUDescriptorHandle;
	pd3dDevice->CreateDepthStencilView(m_pd3dDepthStencilBuffer.Get(), NULL, m_d3dDsvCPUDescriptorHandle);

	m_pTexture = std::make_shared<CTexture>(1, RESOURCE_TEXTURE_CUBE, 0, 1);
	D3D12_CLEAR_VALUE d3dRtvClearValue = { DXGI_FORMAT_R8G8B8A8_UNORM, { 0.0f, 0.0f, 0.0f, 1.0f } };
	ID3D12Resource* pd3dResource = m_pTexture->CreateTexture(pd3dDevice, pd3dCommandList, nCubeMapSize, nCubeMapSize, 6, 1, DXGI_FORMAT_R8G8B8A8_UNORM, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ, &d3dRtvClearValue, RESOURCE_TEXTURE_CUBE, 0);

	pShader->CreateShaderResourceViews(pd3dDevice, m_pTexture, 19, 7);

	D3D12_RENDER_TARGET_VIEW_DESC d3dRTVDesc;
	d3dRTVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dRTVDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
	d3dRTVDesc.Texture2DArray.MipSlice = 0;
	d3dRTVDesc.Texture2DArray.PlaneSlice = 0;
	d3dRTVDesc.Texture2DArray.ArraySize = 1;

	for (int j = 0; j < 6; j++)
	{
		m_pd3dRtvCPUDescriptorHandles[j] = d3dRtvCPUDescriptorHandle;
		d3dRTVDesc.Texture2DArray.FirstArraySlice = j; //i-��° ���� Ÿ�� ��� �ؽ��� ť���� i-��° ���ۿ��� ����
		pd3dDevice->CreateRenderTargetView(pd3dResource, &d3dRTVDesc, m_pd3dRtvCPUDescriptorHandles[j]);
		d3dRtvCPUDescriptorHandle.ptr += pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}
}

CCubeMappingPlayer::~CCubeMappingPlayer()
{
}

void CCubeMappingPlayer::OnPreRender(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, ComPtr<ID3D12CommandQueue> pd3dCommandQueue, std::shared_ptr<CScene> pScene, ID3D12DescriptorHeap** pDescriptorHeaps, int nDescriptorHeaps, D3D12_GPU_DESCRIPTOR_HANDLE hDescriptorStart)
{
	pd3dCommandList->SetDescriptorHeaps(nDescriptorHeaps, pDescriptorHeaps);
	pd3dCommandList->SetGraphicsRootSignature(pScene->GetGraphicsRootSignature());
	pd3dCommandList->SetGraphicsRootDescriptorTable(7, hDescriptorStart);

	static XMFLOAT3 pxmf3LookAts[6] = { XMFLOAT3(+100.0f, 0.0f, 0.0f), XMFLOAT3(-100.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, +100.0f, 0.0f), XMFLOAT3(0.0f, -100.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, +100.0f), XMFLOAT3(0.0f, 0.0f, -100.0f) };
	static XMFLOAT3 pxmf3Ups[6] = { XMFLOAT3(0.0f, +1.0f, 0.0f), XMFLOAT3(0.0f, +1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, +1.0f), XMFLOAT3(0.0f, +1.0f, 0.0f), XMFLOAT3(0.0f, +1.0f, 0.0f) };

	float pfClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	pd3dCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_pTexture->GetResource(0), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_RENDER_TARGET));

	XMFLOAT3 xmf3Position = GetPosition();
	for (int i = 0; i < 6; i++)
	{
		m_apCameras[i]->SetPosition(xmf3Position);
		m_apCameras[i]->GenerateViewMatrix(xmf3Position, Vector3::Add(xmf3Position, pxmf3LookAts[i]), pxmf3Ups[i]);

		pd3dCommandList->ClearRenderTargetView(m_pd3dRtvCPUDescriptorHandles[i], pfClearColor, 0, NULL);
		pd3dCommandList->ClearDepthStencilView(m_d3dDsvCPUDescriptorHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL);

		pd3dCommandList->OMSetRenderTargets(1, &m_pd3dRtvCPUDescriptorHandles[i], TRUE, &m_d3dDsvCPUDescriptorHandle);

		pScene->Render(pd3dCommandList.Get(), m_apCameras[i].get(), RENDER_LIGHT | RENDER_INSTANCING_OBJECT | RENDER_BILLBOARD | RENDER_SKYBOX);
	}

	pd3dCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_pTexture->GetResource(0), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ));
}
