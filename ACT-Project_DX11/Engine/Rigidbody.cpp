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

	Vec3 position = transform->GetPosition();
	if (transform != nullptr)
	{
		HandleCollisionWithTerrain(position);
		position += _velocity * _fixedDeltaTime;
		transform->SetPosition(position);
	}

	// �ܺ� �� �ʱ�ȭ
	_force = Vec3(0.f);

	// ������ ���� (�ӵ� ����)
	ApplyFriction();
}

void Rigidbody::Addforce(const Vec3& force)
{
	_force += force;
}

void Rigidbody::ApplyFriction()
{
	// ���� ���
	float friction = 0.97f; // 0.0 (���� ����) ~ 1.0 (���� ����)

	// �ӵ� ����
	_velocity *= friction;

	// �ּ� �ӵ� �Ӱ谪
	if (_velocity.LengthSquared() < 0.001f)
	{
		_velocity = Vec3(0.f); // ���� ���·� ����
	}
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
