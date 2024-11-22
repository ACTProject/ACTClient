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

	// �߷� ����
	if (_useGravity)
		Addforce(Vec3(0, -9.8f * _mass, 0)); // F = m * g

	// ���ӵ� ��� (a = F / m)
	Vec3 acceleration = _force / _mass;

	// �ӵ� ��� (v = v0 + at)
	_velocity += acceleration * _fixedDeltaTime;

	// ��ġ ��� (s = s0 + vt)
	//shared_ptr<Transform> transform = GetTransform();
	if (transform != nullptr)
	{
		Vec3 position = transform->GetPosition();
		HandleCollisionWithTerrain(position);
		position += _velocity * _fixedDeltaTime;
		transform->SetPosition(position);
	}

	// �ܺ� �� �ʱ�ȭ
	_force = Vec3(0.f);
}

void Rigidbody::Addforce(const Vec3& force)
{
	_force += force;
}

void Rigidbody::HandleCollisionWithTerrain(Vec3& objectPosition)
{
	auto terrain = SCENE->GetCurrentScene()->GetCurrentTerrain();
	// Terrain�� �ش� ��ġ���� ���� ��������
	float terrainHeight = terrain->GetTerrain()->GetHeightAtPosition(objectPosition.x, objectPosition.z);

	Vec3 tp = terrain->GetTransform()->GetPosition();
	Vec3 player = GetTransform()->GetPosition();

	// ��ü�� Terrain �Ʒ��� �������� �浹 ó��
	if (objectPosition.y <= terrainHeight) {
		objectPosition.y = terrainHeight;  // Terrain ���� ��ġ ����
		_velocity.y = 0.0f;          // y�� �ӵ� ����
	}
}
