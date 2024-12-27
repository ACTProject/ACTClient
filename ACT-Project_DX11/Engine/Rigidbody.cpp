#include "pch.h"
#include "Rigidbody.h"
#include "Terrain.h"

Rigidbody::Rigidbody() : Component(ComponentType::Rigidbody)
{

}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::Start()
{
	_fixedDeltaTime = SCENE->GetFixedDeltaTime();
}

void Rigidbody::FixedUpdate()
{
	shared_ptr<Transform> transform = GetTransform();
	if (_fixedDeltaTime == 0.f)
		return;

	// 중력 적용
    if (_useGravity)
    {
        Addforce(Vec3(0, -9.8f * _mass * 2.5, 0)); // F = m * g
    }
	// 가속도 계산 (a = F / m)
	Vec3 acceleration = _force / _mass;

	// 속도 계산 (v = v0 + at)
	_velocity += acceleration * _fixedDeltaTime;

	Vec3 position = transform->GetPosition();
	if (_useGravity)
		HandleCollisionWithTerrain(position);

    position += _velocity * _fixedDeltaTime;
    transform->SetPosition(position);

	// 외부 힘 초기화
	_force = Vec3(0.f);

	// 마찰력 적용 (속도 감속)
	ApplyFriction();
}

void Rigidbody::Addforce(const Vec3& force)
{
	_force += force;
}

void Rigidbody::ApplyFriction()
{
	// 마찰 계수
	float friction = 0.97f; // 0.0 (완전 정지) ~ 1.0 (마찰 없음)

	// 속도 감속
	_velocity *= friction;

	// 최소 속도 임계값
	if (_velocity.LengthSquared() < 0.001f)
	{
		_velocity = Vec3(0.f); // 정지 상태로 설정
	}
}


void Rigidbody::HandleCollisionWithTerrain(Vec3& objectPosition)
{
	auto terrain = SCENE->GetCurrentScene()->GetCurrentTerrain();
	// Terrain의 해당 위치에서 높이 가져오기
	float terrainHeight = terrain->GetTerrain()->GetHeightAtPosition(objectPosition.x, objectPosition.z);

	// 물체가 Terrain 아래로 내려가면 충돌 처리
	if (objectPosition.y <= terrainHeight) {
		objectPosition.y = terrainHeight;  // Terrain 위로 위치 조정
		_velocity.y = 0.0f;          // y축 속도 제거
        _isGrounded = true;
	}
}
