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

/*플레이어의 위치를 변경하는 함수이다. 플레이어의 위치는 기본적으로 사용자가 플레이어를 이동하기 위한 키보드를
누를 때 변경된다. 플레이어의 이동 방향(dwDirection)에 따라 플레이어를 fDistance 만큼 이동한다.*/
void CPlayer::Move(DWORD dwDirection, float fDistance, bool bUpdateVelocity)
{
	if (dwDirection)
	{
		XMFLOAT3 xmf3Shift = XMFLOAT3(0, 0, 0);
		//화살표 키 ‘↑’를 누르면 로컬 z-축 방향으로 이동(전진)한다. ‘↓’를 누르면 반대 방향으로 이동한다.
		if (dwDirection & KEY_FORWARD)
			xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, fDistance);

		if (dwDirection & KEY_BACKWARD)
			xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, -fDistance);

		//화살표 키 ‘→’를 누르면 로컬 x-축 방향으로 이동한다. ‘←’를 누르면 반대 방향으로 이동한다.
		if (dwDirection & KEY_RIGHT) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, fDistance);
		if (dwDirection & KEY_LEFT) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, -fDistance);

		//‘Page Up’을 누르면 로컬 y-축 방향으로 이동한다. ‘Page Down’을 누르면 반대 방향으로 이동한다.
		if (dwDirection & KEY_UP)
			xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, fDistance);

		if (dwDirection & KEY_DOWN)
			xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, -fDistance);
		//플레이어를 현재 위치 벡터에서 xmf3Shift 벡터만큼 이동한다.
		Move(xmf3Shift, bUpdateVelocity);
	}
}


void CPlayer::Move(XMFLOAT3& xmf3Shift, bool bUpdateVelocity)
{
	//bUpdateVelocity가 참이면 플레이어를 이동하지 않고 속도 벡터를 변경한다.
	if (bUpdateVelocity)
	{
		//플레이어의 속도 벡터를 xmf3Shift 벡터만큼 변경한다.
		m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, xmf3Shift);
	}
	else
	{
		//플레이어를 현재 위치 벡터에서 xmf3Shift 벡터만큼 이동한다.
		m_xmf3Position = Vector3::Add(m_xmf3Position, xmf3Shift);

		//플레이어의 위치가 변경되었으므로 카메라의 위치도 xmf3Shift 벡터만큼 이동한다.
		if (m_pCamera)
			m_pCamera->Move(xmf3Shift);
	}
}

//플레이어를 로컬 x-축, y-축, z-축을 중심으로 회전한다.
void CPlayer::Rotate(float x, float y, float z)
{
	DWORD nCameraMode = m_pCamera->GetMode();

	//1인칭 카메라 또는 3인칭 카메라의 경우 플레이어의 회전은 약간의 제약이 따른다.
	if ((nCameraMode == FIRST_PERSON_CAMERA))
	{
		/*로컬 x-축을 중심으로 회전하는 것은 고개를 앞뒤로 숙이는 동작에 해당한다. 그러므로 x-축을 중심으로 회전하는
		각도는 -89.0~+89.0도 사이로 제한한다. x는 현재의 m_fPitch에서 실제 회전하는 각도이므로 x만큼 회전한 다음
		Pitch가 +89도 보다 크거나 -89도 보다 작으면 m_fPitch가 +89도 또는 -89도가 되도록 회전각도(x)를 수정한다.*/
		if (x != 0.0f)
		{
			m_fPitch += x;
			if (m_fPitch > +89.0f) { x -= (m_fPitch - 89.0f); m_fPitch = +89.0f; }if (m_fPitch < -89.0f) { x -= (m_fPitch + 89.0f); m_fPitch = -89.0f; }
		}
		if (y != 0.0f)
		{
			//로컬 y-축을 중심으로 회전하는 것은 몸통을 돌리는 것이므로 회전 각도의 제한이 없다.
			m_fYaw += y;
			if (m_fYaw > 360.0f) m_fYaw -= 360.0f;
			if (m_fYaw < 0.0f) m_fYaw += 360.0f;
		}
		if (z != 0.0f)
		{
			/*로컬 z-축을 중심으로 회전하는 것은 몸통을 좌우로 기울이는 것이므로 회전 각도는 -20.0~+20.0도 사이로 제한된
			다. z는 현재의 m_fRoll에서 실제 회전하는 각도이므로 z만큼 회전한 다음 m_fRoll이 +20도 보다 크거나 -20도보다
			작으면 m_fRoll이 +20도 또는 -20도가 되도록 회전각도(z)를 수정한다.*/
			m_fRoll += z;
			if (m_fRoll > +20.0f){ z -= (m_fRoll - 20.0f); m_fRoll = +20.0f; }
			if (m_fRoll < -20.0f) { z -= (m_fRoll + 20.0f); m_fRoll = -20.0f; }
		}
		//카메라를 x, y, z 만큼 회전한다. 플레이어를 회전하면 카메라가 회전하게 된다.
		m_pCamera->Rotate(x, y, z);

		/*플레이어를 회전한다. 1인칭 카메라 또는 3인칭 카메라에서 플레이어의 회전은 로컬 y-축에서만 일어난다. 플레이어
		의 로컬 y-축(Up 벡터)을 기준으로 로컬 z-축(Look 벡터)와 로컬 x-축(Right 벡터)을 회전시킨다. 기본적으로 Up 벡
		터를 기준으로 회전하는 것은 플레이어가 똑바로 서있는 것을 가정한다는 의미이다.*/
		if (y != 0.0f)
		{
			XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Up), XMConvertToRadians(y));
			m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, xmmtxRotate);
			m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, xmmtxRotate);
		}
	}
	else if (nCameraMode == SPACESHIP_CAMERA || nCameraMode == THIRD_PERSON_CAMERA)
	{
		/*스페이스-쉽 카메라에서 플레이어의 회전은 회전 각도의 제한이 없다. 그리고 모든 축을 중심으로 회전을 할 수 있
		다.*/
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

	/*회전으로 인해 플레이어의 로컬 x-축, y-축, z-축이 서로 직교하지 않을 수 있으므로 z-축(Look 벡터)을 기준으로
	하여 서로 직교하고 단위벡터가 되도록 한다.*/

	m_xmf3Look = Vector3::Normalize(m_xmf3Look);
	m_xmf3Right = Vector3::CrossProduct(m_xmf3Up, m_xmf3Look, true);
	m_xmf3Up = Vector3::CrossProduct(m_xmf3Look, m_xmf3Right, true);
}

//이 함수는 매 프레임마다 호출된다. 플레이어의 속도 벡터에 중력과 마찰력 등을 적용한다.
void CPlayer::Update(float fTimeElapsed)
{
	/*플레이어의 속도 벡터를 중력 벡터와 더한다. 중력 벡터에 fTimeElapsed를 곱하는 것은 중력을 시간에 비례하도록
	적용한다는 의미이다.*/

	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, Vector3::ScalarProduct(m_xmf3Gravity, fTimeElapsed, false));
	/*플레이어의 속도 벡터의 XZ-성분의 크기를 구한다. 이것이 XZ-평면의 최대 속력보다 크면 속도 벡터의 x와 z-방향
	성분을 조정한다.*/

	float fLength = sqrtf(m_xmf3Velocity.x * m_xmf3Velocity.x + m_xmf3Velocity.z *m_xmf3Velocity.z);
	float fMaxVelocityXZ = m_fMaxVelocityXZ * fTimeElapsed;
	if (fLength > m_fMaxVelocityXZ)
	{
		m_xmf3Velocity.x *= (fMaxVelocityXZ / fLength);
		m_xmf3Velocity.z *= (fMaxVelocityXZ / fLength);
	}

	/*플레이어의 속도 벡터의 y-성분의 크기를 구한다. 이것이 y-축 방향의 최대 속력보다 크면 속도 벡터의 y-방향 성분을 조정한다.*/
	float fMaxVelocityY = m_fMaxVelocityY * fTimeElapsed;
	fLength = sqrtf(m_xmf3Velocity.y * m_xmf3Velocity.y);
	if (fLength > m_fMaxVelocityY) m_xmf3Velocity.y *= (fMaxVelocityY / fLength);

	//플레이어를 속도 벡터 만큼 실제로 이동한다(카메라도 이동될 것이다). 
	XMFLOAT3 xmf3Velocity = Vector3::ScalarProduct(m_xmf3Velocity, fTimeElapsed, false);
	Move(xmf3Velocity, false);

	/*플레이어의 위치가 변경될 때 추가로 수행할 작업을 수행한다. 플레이어의 새로운 위치가 유효한 위치가 아닐 수도
	있고 또는 플레이어의 충돌 검사 등을 수행할 필요가 있다. 이러한 상황에서 플레이어의 위치를 유효한 위치로 다시
	변경할 수 있다.*/
	if (m_pPlayerUpdatedContext) OnPlayerUpdateCallback(fTimeElapsed);
	DWORD nCameraMode = m_pCamera->GetMode();

	//플레이어의 위치가 변경되었으므로 3인칭 카메라를 갱신한다.
	if (nCameraMode == THIRD_PERSON_CAMERA) m_pCamera->Update(m_xmf3Position, fTimeElapsed);

	//카메라의 위치가 변경될 때 추가로 수행할 작업을 수행한다.
	if (m_pCameraUpdatedContext) OnCameraUpdateCallback(fTimeElapsed);

	//카메라가 3인칭 카메라이면 카메라가 변경된 플레이어 위치를 바라보도록 한다.
	if (nCameraMode == THIRD_PERSON_CAMERA)
		m_pCamera->SetLookAt(m_xmf3Position);

	//카메라의 카메라 변환 행렬을 다시 생성한다.
	m_pCamera->RegenerateViewMatrix();

	/*플레이어의 속도 벡터가 마찰력 때문에 감속이 되어야 한다면 감속 벡터를 생성한다. 속도 벡터의 반대 방향 벡터를
	구하고 단위 벡터로 만든다. 마찰 계수를 시간에 비례하도록 하여 마찰력을 구한다. 단위 벡터에 마찰력을 곱하여 감
	속 벡터를 구한다. 속도 벡터에 감속 벡터를 더하여 속도 벡터를 줄인다. 마찰력이 속력보다 크면 속력은 0이 될 것이
	다.*/

	fLength = Vector3::Length(m_xmf3Velocity);
	float fDeceleration = (m_fFriction * fTimeElapsed);

	if (fDeceleration > fLength)
		fDeceleration = fLength;

	m_xmf3Velocity = Vector3::Add(m_xmf3Velocity, Vector3::ScalarProduct(m_xmf3Velocity, -fDeceleration, true));
}

/*카메라를 변경할 때 ChangeCamera() 함수에서 호출되는 함수이다. nCurrentCameraMode는 현재 카메라의 모드
이고 nNewCameraMode는 새로 설정할 카메라 모드이다.*/
CCamera* CPlayer::OnChangeCamera(DWORD nNewCameraMode, DWORD nCurrentCameraMode)
{
	//새로운 카메라의 모드에 따라 카메라를 새로 생성한다.
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
	/*현재 카메라의 모드가 스페이스-쉽 모드의 카메라이고 새로운 카메라가 1인칭 또는 3인칭 카메라이면 플레이어의
	Up 벡터를 월드좌표계의 y-축 방향 벡터(0, 1, 0)이 되도록 한다. 즉, 똑바로 서도록 한다. 그리고 스페이스-쉽 카메
	라의 경우 플레이어의 이동에는 제약이 없다. 특히, y-축 방향의 움직임이 자유롭다. 그러므로 플레이어의 위치는 공
	중(위치 벡터의 y-좌표가 0보다 크다)이 될 수 있다. 이때 새로운 카메라가 1인칭 또는 3인칭 카메라이면 플레이어의
	위치는 지면이 되어야 한다. 그러므로 플레이어의 Right 벡터와 Look 벡터의 y 값을 0으로 만든다. 이제 플레이어의
	Right 벡터와 Look 벡터는 단위벡터가 아니므로 정규화한다.*/
	if (nCurrentCameraMode == SPACESHIP_CAMERA)
	{
		m_xmf3Right = Vector3::Normalize(XMFLOAT3(m_xmf3Right.x, 0.0f, m_xmf3Right.z));
		m_xmf3Up = Vector3::Normalize(XMFLOAT3(0.0f, 1.0f, 0.0f));
		m_xmf3Look = Vector3::Normalize(XMFLOAT3(m_xmf3Look.x, 0.0f, m_xmf3Look.z));
		m_fPitch = 0.0f;
		m_fRoll = 0.0f;
		/*Look 벡터와 월드좌표계의 z-축(0, 0, 1)이 이루는 각도(내적=cos)를 계산하여 플레이어의 y-축의 회전 각도
		m_fYaw로 설정한다.*/
		m_fYaw = Vector3::Angle(XMFLOAT3(0.0f, 0.0f, 1.0f), m_xmf3Look);
		if (m_xmf3Look.x < 0.0f) m_fYaw = -m_fYaw;
	}

	else if ((nNewCameraMode == SPACESHIP_CAMERA) && m_pCamera)
	{
		/*새로운 카메라의 모드가 스페이스-쉽 모드의 카메라이고 현재 카메라 모드가 1인칭 또는 3인칭 카메라이면 플레이
		어의 로컬 축을 현재 카메라의 로컬 축과 같게 만든다.*/
		m_xmf3Right = m_pCamera->GetRightVector();
		m_xmf3Up = m_pCamera->GetUpVector();
		m_xmf3Look = m_pCamera->GetLookVector();
	}
	if (pNewCamera)
	{
		pNewCamera->SetMode(nNewCameraMode);

		//현재 카메라를 사용하는 플레이어 객체를 설정한다.
		pNewCamera->SetPlayer(this);
	}

	if (m_pCamera)
		delete m_pCamera;

	return(pNewCamera);
}

/*플레이어의 위치와 회전축으로부터 월드 변환 행렬을 생성하는 함수이다. 플레이어의 Right 벡터가 월드 변환 행렬
의 첫 번째 행 벡터, Up 벡터가 두 번째 행 벡터, Look 벡터가 세 번째 행 벡터, 플레이어의 위치 벡터가 네 번째 행
벡터가 된다.*/
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

	//카메라 모드가 3인칭이면 플레이어 객체를 렌더링한다.
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

	// 앞바퀴
	bool isFrontWheel = true;

	btVector3 connectionPointCS0(vehicleExtents.x - (0.3 * wheelWidth), connectionHeight, vehicleExtents.z - wheelRadius);
	m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, 0.6, wheelRadius, m_tuning, isFrontWheel);

	connectionPointCS0 = btVector3(-vehicleExtents.x + (0.3 * wheelWidth), connectionHeight, vehicleExtents.z - wheelRadius);
	m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, 0.6, wheelRadius, m_tuning, isFrontWheel);

	// 뒷바퀴
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

	//카메라의 카메라 변환 행렬을 다시 생성한다.
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

	//게임 객체가 포함하는 모든 메쉬를 렌더링한다.
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


	//객체의 월드 변환 행렬을 루트 상수(32-비트 값)를 통하여 셰이더 변수(상수 버퍼)로 복사한다.
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
		//플레이어의 특성을 1인칭 카메라 모드에 맞게 변경한다. 중력은 적용하지 않는다.
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
		//플레이어의 특성을 스페이스-쉽 카메라 모드에 맞게 변경한다. 중력은 적용하지 않는다.
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

		//플레이어의 특성을 3인칭 카메라 모드에 맞게 변경한다. 지연 효과와 카메라 오프셋을 설정한다.
		SetFriction(250.0f);
		SetGravity(XMFLOAT3(0.0f, 0.0f, 0.0f));
		SetMaxVelocityXZ(125.0f);
		SetMaxVelocityY(400.0f);
		m_pCamera = OnChangeCamera(THIRD_PERSON_CAMERA, nCurrentCameraMode);

		//3인칭 카메라의 지연 효과를 설정한다. 값을 0.25f 대신에 0.0f와 1.0f로 설정한 결과를 비교하기 바란다.
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

	//게임 객체가 포함하는 모든 메쉬를 렌더링한다.
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

	//객체의 월드 변환 행렬을 루트 상수(32-비트 값)를 통하여 셰이더 변수(상수 버퍼)로 복사한다.
	pd3dCommandList->SetGraphicsRoot32BitConstants(0, 16, &xmf4x4World, 0);
	pd3dCommandList->SetGraphicsRoot32BitConstants(0, 12, &m_material, 16);
}

void CVehiclePlayer::CWheel::ReflectedRender(ID3D12GraphicsCommandList* pd3dCommandList, float mirrorZ)
{
	OnPrepareRender();
	UpdateReflectedShaderVariables(pd3dCommandList, mirrorZ);

	//게임 객체가 포함하는 모든 메쉬를 렌더링한다.
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
		d3dRTVDesc.Texture2DArray.FirstArraySlice = j; //i-번째 렌더 타겟 뷰는 텍스쳐 큐브의 i-번째 버퍼에서 시작
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
