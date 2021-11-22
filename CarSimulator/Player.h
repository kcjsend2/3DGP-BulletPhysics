#pragma once

#define KEY_FORWARD 0x01
#define KEY_BACKWARD 0x02
#define KEY_LEFT 0x04
#define KEY_RIGHT 0x08
#define KEY_UP 0x10
#define KEY_DOWN 0x20
#define KEY_SHIFT 0x40
#define KEY_X 0x80


#include "GameObject.h"
#include "Camera.h"


class CPlayer : public CGameObject
{
protected:
	//�÷��̾��� ��ġ ����, x-��(Right), y-��(Up), z-��(Look) �����̴�.
	XMFLOAT3 m_xmf3Position;
	XMFLOAT3 m_xmf3Right;
	XMFLOAT3 m_xmf3Up;
	XMFLOAT3 m_xmf3Look;

	//�÷��̾ ���� x-��(Right), y-��(Up), z-��(Look)���� �󸶸�ŭ ȸ���ߴ°��� ��Ÿ����.
	float m_fPitch;
	float m_fYaw;
	float m_fRoll;

	//�÷��̾��� �̵� �ӵ��� ��Ÿ���� �����̴�.
	XMFLOAT3 m_xmf3Velocity;

	//�÷��̾ �ۿ��ϴ� �߷��� ��Ÿ���� �����̴�.
	XMFLOAT3 m_xmf3Gravity;

	//xz-��鿡�� (�� ������ ����) �÷��̾��� �̵� �ӷ��� �ִ밪�� ��Ÿ����.
	float m_fMaxVelocityXZ;

	//y-�� �������� (�� ������ ����) �÷��̾��� �̵� �ӷ��� �ִ밪�� ��Ÿ����.
	float m_fMaxVelocityY;

	//�÷��̾ �ۿ��ϴ� �������� ��Ÿ����.
	float m_fFriction;

	//�÷��̾��� �ִ� �ӷ�
	float m_fMaxVelocity = 5.0f;

	//�÷��̾��� ��ġ�� �ٲ� ������ ȣ��Ǵ� OnPlayerUpdateCallback() �Լ����� ����ϴ� �������̴�.
	LPVOID m_pPlayerUpdatedContext;

	//ī�޶��� ��ġ�� �ٲ� ������ ȣ��Ǵ� OnCameraUpdateCallback() �Լ����� ����ϴ� �������̴�.
	LPVOID m_pCameraUpdatedContext;

	//�÷��̾ ���� ������ ī�޶��̴�.
	CCamera *m_pCamera = NULL;

public:
	CPlayer(int nMeshes = 1);
	virtual ~CPlayer();
	XMFLOAT3 GetPosition() { return(m_xmf3Position); }
	XMFLOAT3 GetLookVector() { return(m_xmf3Look); }
	XMFLOAT3 GetUpVector() { return(m_xmf3Up); }
	XMFLOAT3 GetRightVector() { return(m_xmf3Right); }
	void SetFriction(float fFriction) { m_fFriction = fFriction; }
	void SetGravity(XMFLOAT3& xmf3Gravity) { m_xmf3Gravity = xmf3Gravity; }
	void SetMaxVelocityXZ(float fMaxVelocity) { m_fMaxVelocityXZ = fMaxVelocity; }
	void SetMaxVelocityY(float fMaxVelocity) { m_fMaxVelocityY = fMaxVelocity; }
	void SetVelocity(XMFLOAT3& xmf3Velocity) { m_xmf3Velocity = xmf3Velocity; }

	/*�÷��̾��� ��ġ�� xmf3Position ��ġ�� �����Ѵ�. xmf3Position ���Ϳ��� ���� �÷��̾��� ��ġ ���͸� ���� ��
	�� �÷��̾��� ��ġ���� xmf3Position ���������� ���Ͱ� �ȴ�. ���� �÷��̾��� ��ġ���� �� ���� ��ŭ �̵��Ѵ�.*/

	void SetPosition(XMFLOAT3& xmf3Position)
	{
		Move(XMFLOAT3(xmf3Position.x - m_xmf3Position.x, xmf3Position.y - m_xmf3Position.y, xmf3Position.z - m_xmf3Position.z), false);
	}

	XMFLOAT3& GetVelocity() { return(m_xmf3Velocity); }

	float GetYaw() { return(m_fYaw); }
	float GetPitch() { return(m_fPitch); }
	float GetRoll() { return(m_fRoll); }
	CCamera* GetCamera() { return(m_pCamera); }
	void SetCamera(CCamera* pCamera) { m_pCamera = pCamera; }

	//�÷��̾ �̵��ϴ� �Լ��̴�.
	void Move(XMFLOAT3& xmf3Shift, bool bVelocity = false);
	void Move(DWORD dwDirection, float fDistance, bool bUpdateVelocity);

	//�÷��̾ ȸ���ϴ� �Լ��̴�.
	void Rotate(float x, float y, float z);

	//�÷��̾��� ��ġ�� ȸ�� ������ ��� �ð��� ���� �����ϴ� �Լ��̴�.
	void Update(float fTimeElapsed);

	//�÷��̾��� ��ġ�� �ٲ� ������ ȣ��Ǵ� �Լ��� �� �Լ����� ����ϴ� ������ �����ϴ� �Լ��̴�.
	virtual void OnPlayerUpdateCallback(float fTimeElapsed) { }
	void SetPlayerUpdatedContext(LPVOID pContext) { m_pPlayerUpdatedContext = pContext; }

	//ī�޶��� ��ġ�� �ٲ� ������ ȣ��Ǵ� �Լ��� �� �Լ����� ����ϴ� ������ �����ϴ� �Լ��̴�.
	virtual void OnCameraUpdateCallback(float fTimeElapsed) { }
	void SetCameraUpdatedContext(LPVOID pContext) { m_pCameraUpdatedContext = pContext; }
	virtual void CreateShaderVariables(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void ReleaseShaderVariables();
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);

	//ī�޶� �����ϱ� ���Ͽ� ȣ���ϴ� �Լ��̴�.
	CCamera *OnChangeCamera(DWORD nNewCameraMode, DWORD nCurrentCameraMode);

	virtual CCamera* ChangeCamera(DWORD nNewCameraMode, float fTimeElapsed)
	{
		return(NULL);
	}

	//�÷��̾��� ��ġ�� ȸ�������κ��� ���� ��ȯ ����� �����ϴ� �Լ��̴�.
	virtual void OnPrepareRender();
	//�÷��̾��� ī�޶� 3��Ī ī�޶��� �� �÷��̾�(�޽�)�� �������Ѵ�.
	virtual void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera = NULL);
};

class CCubeMappingPlayer : public CPlayer
{
public:
	CCubeMappingPlayer(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, LONG nCubeMapSize, CShader* pShader, int nMeshes);
	~CCubeMappingPlayer();

	virtual void OnPreRender(ComPtr<ID3D12GraphicsCommandList> pd3dCommandList, ComPtr<ID3D12CommandQueue> pd3dCommandQueue, std::shared_ptr<CScene> pScene, ID3D12DescriptorHeap** pDescriptorHeaps, int nDescriptorHeaps, D3D12_GPU_DESCRIPTOR_HANDLE hDescriptorStart);
	std::array<std::shared_ptr<CCamera>, 6> m_apCameras;

	D3D12_CPU_DESCRIPTOR_HANDLE m_pd3dRtvCPUDescriptorHandles[6];

	ComPtr<ID3D12Resource> m_pd3dDepthStencilBuffer = NULL;
	D3D12_CPU_DESCRIPTOR_HANDLE	m_d3dDsvCPUDescriptorHandle;

	std::shared_ptr<CTexture> m_pTexture;

	ComPtr<ID3D12DescriptorHeap> m_pd3dRtvDescriptorHeap = NULL;
	ComPtr<ID3D12DescriptorHeap> m_pd3dDsvDescriptorHeap = NULL;
};

class CVehiclePlayer : public CCubeMappingPlayer
{
private:
	class CWheel : public CGameObject
	{
	public:
		CWheel(std::shared_ptr<CMeshFileRead> pWheelMesh);
		~CWheel();
		virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList);
		virtual void UpdateReflectedShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList, float mirrorZ);
		virtual void ReflectedRender(ID3D12GraphicsCommandList* pd3dCommandList, float mirrorZ);
		virtual void Update(float fTimeElapsed, btRaycastVehicle* pbtVehicle, int index);
	private:
		XMFLOAT3 m_xmf3Look;
	};

public:
	CVehiclePlayer(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, btAlignedObjectArray<btCollisionShape*>& btCollisionShapes, btDiscreteDynamicsWorld* pbtDynamicsWorld, CShader* pShader,int nMeshes = 5);
	virtual ~CVehiclePlayer();
	std::shared_ptr<CBullet> GetBullet() { return m_pBullet; };
	virtual void Update(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, float fTimeElapsed, btDiscreteDynamicsWorld* pbtDynamicsWorld, DWORD dwBehave);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);
	virtual void ReflectedRender(ID3D12GraphicsCommandList* pd3dCommandList, float mirrorZ);
	virtual void UpdateReflectedShaderVariables(ID3D12GraphicsCommandList* pd3dCommandList, float mirrorZ);
	virtual CCamera* ChangeCamera(DWORD nNewCameraMode, float fTimeElapsed);
	void FireBullet(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, btDiscreteDynamicsWorld* pbtDynamicsWorld);
	void EraseBullet() { m_pBullet = NULL; }
	void SetRigidBodyPosition(XMFLOAT3 xmf3Position);

	std::shared_ptr<CWheel>* GetWheels() { return m_pWheel; }

private:
	std::shared_ptr<CWheel> m_pWheel[4];

	btRaycastVehicle::btVehicleTuning m_tuning;
	btVehicleRaycaster* m_vehicleRayCaster;
	btRaycastVehicle* m_vehicle;

	std::shared_ptr<CBullet> m_pBullet;

	float m_gEngineForce = 0.f;

	float m_defaultBreakingForce = 10.f;
	float m_gBreakingForce = 0.f;

	float m_maxEngineForce = 4000.f;
	float m_EngineForceIncrement = 5.0f;

	float m_gVehicleSteering = 0.f;
	float m_steeringIncrement = 0.01f;
	float m_steeringClamp = 0.1f;
	float m_wheelRadius = 0.5f;
	float m_wheelWidth = 0.4f;
};
