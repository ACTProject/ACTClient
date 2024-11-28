#include "pch.h"
#include "Camera.h"
#include "Scene.h"

Matrix	Camera::S_MatView			= Matrix::Identity;
Matrix	Camera::S_MatProjection		= Matrix::Identity;
Matrix	Camera::S_UIMatView			= Matrix::Identity;
Matrix	Camera::S_UIMatProjection	= Matrix::Identity;
bool	Camera::S_IsWireFrame	= false;

void Camera::SetCameraOffset(Vec3 v)
{	
	_cameraOffset.x = v.x; 
	_cameraOffset.y = v.y; 
	_cameraOffset.z = v.z; 

	_cameraDistance = _cameraOffset.Length();
}
void Camera::SortGameObject()
{
	shared_ptr<Scene> scene = CUR_SCENE;
	unordered_set<shared_ptr<GameObject>>& gameObjects = scene->GetObjects();

	_vecForward.clear();

	// ��� ���ӿ�����Ʈ�� ������� ���� �׷������ ������� üũ
	for (auto& gameObject : gameObjects)
	{
		if (IsCulled(gameObject->GetLayerIndex()))
			continue;

		if (gameObject->GetMeshRenderer() == nullptr
			&& gameObject->GetModelRenderer() == nullptr
			&& gameObject->GetModelAnimator() == nullptr)
			continue;

		// ���� �׷������ ����� gameObject
		_vecForward.push_back(gameObject);		
	}
}

void Camera::Render_Forward()
{
	// ���� ���� �׸� ī�޶�ϱ� ���� ������� ���� 
	S_MatView = _matView;
	S_MatProjection = _matProjection;

	GET_SINGLE(InstancingManager)->Render(_vecForward);
}

Camera::Camera() : Super(ComponentType::Camera)
{
	_width = static_cast<float>(GAME->GetGameDesc().width);
	_height = static_cast<float>(GAME->GetGameDesc().height);
}

Camera::~Camera()
{

}

void Camera::Update()
{
	if (DEBUG->IsDebugEnabled())
	{
		if (!_debugInitialized) // ����� ��� ���� ����
		{
			_debugInitialized = true;

			// ����� ��� �ʱ� ��ġ�� ���� ����
			_cameraPosition = Vec3(0.0f, 20.0f, -5.0f); // ������ �Ʒ��� �ٶ󺸴� ��ġ
			_focusPosition = Vec3(0.0f, 0.0f, 0.0f);     // ������ ����
			_yaw = XM_PIDIV4;                           // 45�� ȸ��
			_pitch = -XM_PIDIV4;                        // �Ʒ��� 45�� �ٶ�

			UpdateMatrix();

			return;
		}
		FreeCameraMovement();
	}
	else
	{
		if (_debugInitialized) // ����� ��� ����
		{
			_debugInitialized = false;
		}

		UpdateCameraWithMouseInput();
	}

	// ī�޶� ��Ʈ���� ������Ʈ
	UpdateMatrix();
}

void Camera::FreeCameraMovement()	// WASD Ű�� �̵�, ���콺�� ȸ��
{
	// WASD �̵�
	Vec3 movement(0.0f);
	// ���� ���� ���
	Vec3 forward(
		cosf(_pitch) * sinf(_yaw),
		sinf(_pitch),
		cosf(_pitch) * cosf(_yaw)
	);
	forward.Normalize();

	Vec3 right = forward.Cross(Vec3(0.0f, 1.0f, 0.0f));
	right.Normalize();

	Vec3 up = right.Cross(forward);
	up.Normalize();

	if (INPUT->GetButton(KEY_TYPE::W)) movement += forward;    // ������
	if (INPUT->GetButton(KEY_TYPE::S)) movement -= forward;    // �ڷ�
	if (INPUT->GetButton(KEY_TYPE::A)) movement += right;      // ����
	if (INPUT->GetButton(KEY_TYPE::D)) movement -= right;      // ������
	if (INPUT->GetButton(KEY_TYPE::Q)) movement -= up;         // �Ʒ���
	if (INPUT->GetButton(KEY_TYPE::E)) movement += up;         // ����

	// �̵� �ӵ� (Shift Ű�� �ӵ� ����)
	float speed = (INPUT->GetButton(KEY_TYPE::SHIFT)) ? _fastSpeed : _normalSpeed;
	_cameraPosition += movement * speed * TIME->GetDeltaTime();

	if (INPUT->GetButton(KEY_TYPE::LBUTTON)) // �巡�� ���� Ȯ��
	{
		// ���콺 �Է�
		float dx = INPUT->GetMouseDeltaX();
		float dy = INPUT->GetMouseDeltaY();

		// yaw�� pitch ������ ���콺 �̵��� ���� ����
		_yaw += dx * _sensitivity;
		_pitch -= dy * _sensitivity;

		// pitch ���� ������ �����Ͽ� ī�޶� �������� �ʵ��� ���� (-90�� ~ 90�� ����)
		_pitch = std::clamp(_pitch, -XM_PIDIV2 + 0.1f, XM_PIDIV2 - 0.1f);
	}	
	// ī�޶� ���� ����
	forward = Vec3(
		cosf(_pitch) * sinf(_yaw),
		sinf(_pitch),
		cosf(_pitch) * cosf(_yaw)
	);
	forward.Normalize(); 
	_focusPosition = _cameraPosition + forward;
}

void Camera::UpdateCameraWithMouseInput()
{
	_player = CUR_SCENE->GetPlayer();

	float dx = INPUT->GetMouseDeltaX(); // x�� ���콺 �̵���
	float dy = INPUT->GetMouseDeltaY(); // y�� ���콺 �̵���

	// yaw�� pitch ������ ���콺 �̵��� ���� ����
	_yaw += dx * _sensitivity;
	_pitch += dy * _sensitivity;

	// pitch ���� ������ �����Ͽ� ī�޶� �������� �ʵ��� ���� (-90�� ~ 90�� ����)
	_pitch = std::clamp(_pitch, -XM_PIDIV2 + 0.1f, XM_PIDIV2 - 0.1f);

	// ī�޶� ��ġ ���
	float x = _cameraDistance * cosf(_pitch) * sinf(_yaw);
	float y = _cameraDistance * sinf(_pitch);
	float z = _cameraDistance * cosf(_pitch) * cosf(_yaw);

	// ī�޶� ��ġ�� �÷��̾� ��ġ �������� ����
	if (_player == nullptr)
	{
		Vec3 cameraposition = GetTransform()->GetPosition();
		_cameraPosition = cameraposition + Vec3(x, y, z);
		_focusPosition = cameraposition;
	}
	else 
	{
		Vec3 playerPosition = _player->GetTransform()->GetPosition();
		_cameraPosition = playerPosition + Vec3(x, y, z);
		_focusPosition = playerPosition;
	}
}

void Camera::UpdateMatrix()
{
	Vec3 eyePosition;
	Vec3 focusPosition;
	Vec3 upDirection;

	if (_type == ProjectionType::Perspective) // Main ī�޶�
	{
		eyePosition = _cameraPosition;
		focusPosition = _focusPosition;
		upDirection = Vec3(0.0f, 1.0f, 0.0f);
	}
	else // UI ī�޶�
	{
		eyePosition = GetTransform()->GetPosition();
		focusPosition = eyePosition + GetTransform()->GetLook();
		upDirection = GetTransform()->GetUp();
	}

	_matView = ::XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);

	if (_type == ProjectionType::Perspective) 
	{
		_matProjection = ::XMMatrixPerspectiveFovLH(_fov, _width / _height, _near, _far);
		S_MatView = _matView;
		S_MatProjection = _matProjection;
	}
	else
	{
		_matProjection = ::XMMatrixOrthographicLH(_width, _height, _near, _far);
		S_UIMatView = _matView;
		S_UIMatProjection = _matProjection;
	}
	
}

