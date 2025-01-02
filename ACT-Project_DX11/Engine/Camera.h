#pragma once
#include "Component.h"

enum class ProjectionType
{
	Perspective, // 원근 투영
	Orthographic, // 직교 투영
};

class Camera :  public Component
{
	using Super = Component;
public:
	Camera();
	virtual ~Camera();
	
public:
	static bool S_IsWireFrame;
	static Matrix S_MatView;
	static Matrix S_MatProjection;
	static Matrix S_UIMatView;
	static Matrix S_UIMatProjection;

	virtual void Update() override;
	void UpdateCameraWithMouseInput();
	void UpdateMatrix();
	void FreeCameraMovement(); // 디버그 모드시 카메라 이동
    void SetCameraZoomForTerrain(const shared_ptr<class Terrain>& terrain);
    void RestrictCameraAboveTerrain(const shared_ptr<Terrain>& terrain);

	void SetProjectionType(ProjectionType type) { _type = type; }
	ProjectionType GetProjectionType() { return _type; }


	void SetNear(float value) { _near = value; }
	void SetFar(float value) { _far = value; }
	void SetFOV(float value) { _fov = value; }
	void SetWidth(float value) { _width = value; }
	void SetHeight(float value) { _height = value; }

    Vec3 GetForward() const;
    Vec3 GetRight() const;
	void SetCameraOffset(Vec3 v);

	Matrix& GetViewMatrix() { return _matView; }
	Matrix& GetProjectionMatrix() { return _matProjection; }

	float GetWidth() { return _width; }
	float GetHeight() { return _height; }

	Vec3 GetCameraOffset() { return _cameraOffset; }
    Vec3 GetCameraPosition() { return _cameraPosition; }

    // 메인 카메라인지 여부 설정 및 확인
    void SetMainCamera(bool isMain) { _isMainCamera = isMain; }
    bool IsMainCamera() const { return _isMainCamera; }

public:
	void SortGameObject();
	void Render_Forward();

	void SetCullingMaskLayerOnOff(uint8 layer, bool on)
	{
		if (on)
			_cullingMask |= (1 << layer);
		else
			_cullingMask &= ~(1 << layer);
	}

	void SetCullingMaskAll() { SetCullingMask(UINT32_MAX); }
	void SetCullingMask(uint32 mask) { _cullingMask = mask; }
	bool IsCulled(uint8 layer) { return (_cullingMask & (1 << layer)) != 0; }

    vector<shared_ptr<GameObject>>& GetVecForward() { return _vecForward; }

    void SetMouseLock();
    void SetIsTitle(bool isTitle);
private:
    // 메인 카메라인지 여부
    bool _isMainCamera = false;

	uint32 _cullingMask = 0;
	vector<shared_ptr<GameObject>> _vecForward;
	shared_ptr<GameObject> _player = nullptr;

private:
	ProjectionType _type = ProjectionType::Perspective;
	Matrix _matView = Matrix::Identity;
	Matrix _matProjection = Matrix::Identity;

	float _near = 1.f;
	float _far = 1000.f;
	float _fov = XM_PI / 4.f;
	float _width = 0.f;
	float _height = 0.f;

	//카메라가 플레이어를 중심으로 어느 정도 떨어진 위치에 있을지 정하는 위치 벡터
	Vec3 _cameraOffset = Vec3(0.f);

	float _yaw = 0.0f;				// 좌우 회전 각도
	float _pitch = 0.0f;			// 상하 회전 각도
	float _cameraDistance = 5.0f;	// 플레이어와 카메라 간의 거리
    float _zoomCameraDistance = 5.0f;
	float _sensitivity = 0.005f;	// 마우스 감도

	float _normalSpeed = 15.0f;  // 일반 이동 속도
	float _fastSpeed = 100.0f;   // 빠른 이동 속도 (Shift 키)

	// 디버깅 모드 이전 상태 저장
	bool _debugInitialized = false;			// 디버깅 모드 초기화 여부

    Vec3 _camForward;
    Vec3 _camRight;
	// 카메라 위치
	Vec3 _cameraPosition = Vec3(0.f);
	// 초점 위치
	Vec3 _focusPosition = Vec3(0.f);

    bool _lockMouse = true; //마우스 고정 여부
    bool _isTitle = false;
};
