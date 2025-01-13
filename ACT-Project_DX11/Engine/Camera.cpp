#include "pch.h"
#include "Camera.h"
#include "Scene.h"
#include "Terrain.h"

Matrix	Camera::S_MatView = Matrix::Identity;
Matrix	Camera::S_MatProjection = Matrix::Identity;
Matrix	Camera::S_UIMatView = Matrix::Identity;
Matrix	Camera::S_UIMatProjection = Matrix::Identity;
bool	Camera::S_IsWireFrame = false;

Vec3 Camera::GetForward() const
{
    Vec3 forward = Vec3(_matView._13, _matView._23, _matView._33);
    if (forward.LengthSquared() > 0.0f)
        forward.Normalize();
    return forward;
}

Vec3 Camera::GetRight() const
{
    Vec3 forward = GetForward();
    Vec3 up = Vec3(0.0f, 1.0f, 0.0f);
    Vec3 right = forward.Cross(up);
    right.Normalize();
    return right;
}

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

    // 모든 게임오브젝트를 대상으로 내가 그려줘야할 대상인지 체크
    for (auto& gameObject : gameObjects)
    {
        if (IsCulled(gameObject->GetLayerIndex()))
            continue;

        if (gameObject->GetMeshRenderer() == nullptr
            && gameObject->GetModelRenderer() == nullptr
            && gameObject->GetModelAnimator() == nullptr)
        {
            if (gameObject->GetCollider() == nullptr)
                continue;
        }

        // 내가 그려줘야할 대상인 gameObject
        _vecForward.push_back(gameObject);
    }
}

void Camera::Render_Forward()
{
    if (IsMainCamera())
    {
        S_MatView = _matView;
        S_MatProjection = _matProjection;
        FRUSTUM->FinalUpdate();
        vector<shared_ptr<GameObject>> visibleObjects = CUR_SCENE->FrustumCulling(_vecForward);
        GET_SINGLE(InstancingManager)->Render(visibleObjects);
    }
    else
    {
        S_UIMatView = _matView;
        S_UIMatProjection = _matProjection;
        GET_SINGLE(InstancingManager)->Render(_vecForward);
    }
}


Camera::Camera() : Super(ComponentType::Camera)
{
    _width = static_cast<float>(GAME->GetGameDesc().width);
    _height = static_cast<float>(GAME->GetGameDesc().height);
}

Camera::~Camera()
{

}

void Camera::Start()
{

}

void Camera::Update()
{
    if (_isCutsceneActive)
    {
        UpdateCutscene();
        return;
    }
    else
    {
        if (_isMainCamera && !CUTSCENE->IsQueueEmpty())
        {
            CUTSCENE->PlayNext();
            UpdateCutscene();
            return;
        }
    }

    auto uiCamera = CUR_SCENE->GetUICamera();
    uiCamera->SetActive(true);

	if (DEBUG->IsDebugEnabled())
	{
		if (!_debugInitialized) // 디버깅 모드 최초 진입
		{
			_debugInitialized = true;

			// 디버깅 모드 초기 위치와 방향 설정
			_cameraPosition = Vec3(0.0f, 20.0f, -5.0f); // 위에서 아래를 바라보는 위치
			_focusPosition = Vec3(0.0f, 0.0f, 0.0f);     // 초점은 원점
			_yaw = XM_PIDIV4;                           // 45도 회전
			_pitch = -XM_PIDIV4;                        // 아래를 45도 바라봄

			UpdateMatrix();
			return;
		}
		FreeCameraMovement();
	}
	else
	{
		if (_debugInitialized) // 디버깅 모드 종료
		{
			_debugInitialized = false;
		}
        SetMouseLock();
		UpdateCameraWithMouseInput();

        auto terrain = CUR_SCENE->GetCurrentTerrain()->GetTerrain();  // 현재 Terrain 가져오기
        if (terrain)
        {
            // Terrain을 통과하지 못하도록 카메라 위치 제한
            RestrictCameraAboveTerrain(terrain);
        }
	}

	// 카메라 매트릭스 업데이트
	UpdateMatrix();
}

void Camera::FreeCameraMovement()	// WASD 키로 이동, 마우스로 회전
{
    // WASD 이동
    Vec3 movement(0.0f);
    // 방향 벡터 계산
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

    if (INPUT->GetButton(KEY_TYPE::W)) movement += forward;    // 앞으로
    if (INPUT->GetButton(KEY_TYPE::S)) movement -= forward;    // 뒤로
    if (INPUT->GetButton(KEY_TYPE::A)) movement += right;      // 왼쪽
    if (INPUT->GetButton(KEY_TYPE::D)) movement -= right;      // 오른쪽
    if (INPUT->GetButton(KEY_TYPE::Q)) movement -= up;         // 아래로
    if (INPUT->GetButton(KEY_TYPE::E)) movement += up;         // 위로

    // 이동 속도 (Shift 키로 속도 변경)
    float speed = (INPUT->GetButton(KEY_TYPE::SHIFT)) ? _fastSpeed : _normalSpeed;
    _cameraPosition += movement * speed * TIME->GetDeltaTime();

    if (INPUT->GetButton(KEY_TYPE::LBUTTON)) // 드래그 상태 확인
    {
        // 마우스 입력
        float dx = INPUT->GetMouseDeltaX();
        float dy = INPUT->GetMouseDeltaY();

        // yaw와 pitch 각도를 마우스 이동에 따라 조절
        _yaw += dx * _sensitivity;
        _pitch -= dy * _sensitivity;

        // pitch 값의 범위를 제한하여 카메라가 뒤집히지 않도록 조정 (-90도 ~ 90도 사이)
        _pitch = std::clamp(_pitch, -XM_PIDIV4, XM_PIDIV4);
    }
    // 카메라 초점 갱신
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
    
    if (_lockMouse)
    {
        POINT centerPos;
        RECT rect;
        GetClientRect(GAME->GetHWND(), &rect);
        centerPos.x = (rect.left + rect.right) / 2;
        centerPos.y = (rect.top + rect.bottom) / 2;

        ClientToScreen(GAME->GetHWND(), &centerPos);

        POINT currentMousePos;
        GetCursorPos(&currentMousePos);
        float deltaX = (float)(currentMousePos.x - centerPos.x);
        float deltaY = (float)(currentMousePos.y - centerPos.y);

        SetCursorPos(centerPos.x, centerPos.y);

        // yaw와 pitch 각도를 마우스 이동에 따라 조절
        _yaw += deltaX * _sensitivity;
        _pitch += deltaY * _sensitivity;

        // pitch 값의 범위를 제한하여 카메라가 뒤집히지 않도록 조정 (-90도 ~ 90도 사이)
        _pitch = std::clamp(_pitch, -XM_PIDIV2 + 0.1f, XM_PIDIV2 - 0.1f);
        _pitch = max(_pitch, -1.0f); // pitch를 수평 이상으로 제한
    } 

    // 카메라 위치 계산
    float x = _cameraDistance * cosf(_pitch) * sinf(_yaw);
    float y = _cameraDistance * sinf(_pitch);
    float z = _cameraDistance * cosf(_pitch) * cosf(_yaw);

	// 카메라 위치를 플레이어 위치 기준으로 설정
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

    if (_type == ProjectionType::Perspective) // Main 카메라
    {
        eyePosition = _cameraPosition;
        focusPosition = _focusPosition;
        upDirection = Vec3(0.0f, 1.0f, 0.0f);

        //Vec3 forward = GetForward();
        //cout << "cameraPosition : " << _cameraPosition.x << " " << _cameraPosition.y << " " << _cameraPosition.z << endl;
        //cout << "forward : " << forward.x << " " << forward.y << " " << forward.z << endl;
    }
    else // UI 카메라
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

void Camera::SetCameraZoomForTerrain(const shared_ptr<Terrain>& terrain)
{
    // Terrain 높이를 가져오기
    float terrainHeight = terrain->GetHeightAtPosition(_cameraPosition.x, _cameraPosition.z);

    // Terrain과 카메라의 거리 계산
    float distanceToTerrain = _cameraPosition.y - terrainHeight;

    // Terrain과의 거리가 임계값 이하일 경우 카메라 줌인
    if (distanceToTerrain < 2.0f) // 임계값 설정 (2.0f는 조정 가능)
    {
        // 줌인 비율 계산 (줌인을 완화)
        float zoomFactor = std::clamp(0.9f + (distanceToTerrain / 2.0f * 0.1f), 0.9f, 1.0f);

        // 카메라 중심축 벡터
        Vec3 direction = _cameraPosition - _focusPosition;
        direction.Normalize();

        // 카메라 위치 조정
        _cameraPosition = _focusPosition + direction * _cameraDistance * zoomFactor;
    }
}

void Camera::RestrictCameraAboveTerrain(const shared_ptr<Terrain>& terrain)
{
    // Terrain 높이를 가져오기
    float terrainHeight = terrain->GetHeightAtPosition(_cameraPosition.x, _cameraPosition.z);

    // 카메라가 Terrain 아래로 내려가지 못하도록 y 좌표를 조정
        if (_cameraPosition.y < terrainHeight + 2.0f) // 여유 높이 설정 (0.5f는 조정 가능)
    {
        _cameraPosition.y = terrainHeight + 2.0f;

        // 카메라가 Terrain에 너무 가까운 경우, 뒤로 밀어내기
        Vec3 directionToFocus = _focusPosition - _cameraPosition;
        directionToFocus.Normalize();

        // 카메라를 뒤로 이동시켜 Terrain과의 충돌을 완벽히 방지
        _cameraPosition -= directionToFocus * 0.1f; // 0.1f는 조정 가능
    }
}

void Camera::SetMouseLock()
{
    static bool _cursorVisible = true;

    if (_isTitle)
    {
        _lockMouse = false;
        if (!_cursorVisible)
        {
            ShowCursor(true);
            _cursorVisible = true;
        }
        ClipCursor(nullptr);
        return;
    }
    
    static POINT lastMousePos = {};   // 이전 마우스 위치 저장
    bool previousLockMouse = _lockMouse;
    _lockMouse = !SAVE->GetIsActive();

    if (_lockMouse)
    {
        if (_cursorVisible)
        {
            ShowCursor(false);
            _cursorVisible = false;
        }

        // 마우스 고정 상태로 전환 시 이동량 초기화
        if (!previousLockMouse)
        {
            RECT clientRect;
            GetClientRect(GAME->GetHWND(), &clientRect); // 클라이언트 영역 가져오기

            // 클라이언트 좌표를 화면 좌표로 변환
            POINT centerPos;
            centerPos.x = (clientRect.right + clientRect.left) / 2;
            centerPos.y = (clientRect.bottom + clientRect.top) / 2;
            ClientToScreen(GAME->GetHWND(), &centerPos); // 화면 좌표로 변환

            // 마우스를 화면 중앙에 위치
            SetCursorPos(centerPos.x, centerPos.y);

            lastMousePos = centerPos;
        }
    }
    else
    {
        if (!_cursorVisible)
        {
            ShowCursor(true);
            _cursorVisible = true;
        }
        ClipCursor(nullptr);
    }
}

void Camera::SetIsTitle(bool isTitle)
{
    _isTitle = isTitle;
}

// CutScene
void Camera::StartCutscene(const Vec3& start, const Vec3& end, const Vec3& focus, float duration)
{
    _cutsceneStartPosition = start;
    _cutsceneEndPosition = end;
    _cutsceneFocusPosition = focus;
    _cutsceneDuration = duration;
    _cutsceneElapsedTime = 0.0f;
    _isCutsceneActive = true;

    std::cout << "Cutscene started!" << std::endl;
}

void Camera::UpdateCutscene()
{
    auto uiCamera = CUR_SCENE->GetUICamera();
    uiCamera->SetActive(false);

    _cutsceneElapsedTime += TIME->GetDeltaTime();
    float t = _cutsceneElapsedTime / _cutsceneDuration;

    if (t >= 1.0f)
    {
        _cameraPosition = _cutsceneEndPosition;
        _focusPosition = _cutsceneFocusPosition + _cameraPosition;
        EndCutscene();
        return;
    }

    // 선형 보간 (Lerp)
    _cameraPosition = _cutsceneStartPosition * (1.0f - t) + _cutsceneEndPosition * t;
    _focusPosition = _cutsceneFocusPosition + _cameraPosition;

    UpdateMatrix();
}

void Camera::EndCutscene()
{
    _isCutsceneActive = false;
    std::cout << "Cutscene ended!" << std::endl;
}