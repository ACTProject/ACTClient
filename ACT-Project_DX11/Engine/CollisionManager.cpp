#include "pch.h"
#include "CollisionManager.h"
#include "BaseCollider.h"
#include "Rigidbody.h"

void CollisionManager::Init()
{

}

// �浹 ���� �� ó��
void CollisionManager::Update()
{
	for (size_t i = 0; i < _colliders.size(); ++i)
	{
		for (size_t j = i + 1; j < _colliders.size(); ++j)
		{
			auto colliderA = _colliders[i];
			auto colliderB = _colliders[j];

			// �浹 ����
			if (colliderA->Intersects(colliderB))
			{
				HandleCollision(colliderA, colliderB);
			}
		}
	}
}

void CollisionManager::AddCollider(shared_ptr<BaseCollider> collider)
{
	if (collider)
		_colliders.push_back(collider);
}

void CollisionManager::AddRigidbody(shared_ptr<Rigidbody> rigidbody)
{
	if (rigidbody)
		_rigidbodies.push_back(rigidbody);
}

void CollisionManager::HandleCollision(shared_ptr<BaseCollider> colliderA, shared_ptr<BaseCollider> colliderB)
{
	// Rigidbody ��������
	auto rigidbodyA = colliderA->GetGameObject()->GetRigidbody();
	auto rigidbodyB = colliderB->GetGameObject()->GetRigidbody();

	// Rigidbody�� ������ ó������ ����
	if (!rigidbodyA || !rigidbodyB)
		return;

	// Penetration Depth ���
	Vec3 penetrationDepth;
	if (!colliderA->CalculatePenetraionDepth(colliderB, penetrationDepth))
	{
		return; // �浹�� �߻����� ����
	}

	// �浹 ���� ���
	penetrationDepth.Normalize();
	Vec3 collisionNormal = penetrationDepth;

	// ���� ���� ����
	float minimumSeparation = 0.1f; // ������Ʈ �� �ּ� ����
	float penetrationLength = penetrationDepth.Length();

	if (penetrationLength <= minimumSeparation)
	{
		// �̹� ����� �Ÿ��� �����Ǹ� �� �̻� �о�� ����
		return;
	}

	// ���� Ȯ��
	float massA = rigidbodyA->GetMass();
	float massB = rigidbodyB->GetMass();

	if (massA > massB)
	{
		ApplyForce(rigidbodyB, colliderB->GetColliderCenter(), colliderA->GetColliderCenter(), rigidbodyA->GetMass() - rigidbodyB->GetMass());
	}
	else
	{
		ApplyForce(rigidbodyA, colliderA->GetColliderCenter(), colliderB->GetColliderCenter(), rigidbodyB->GetMass() - rigidbodyA->GetMass());
	}

	// �浹 ���� ����
	colliderA->SetColliding(true);
	colliderB->SetColliding(true);
}

void CollisionManager::ApplyForce(shared_ptr<Rigidbody> rigidbody, const Vec3& target, const Vec3& source, float massDifference)
{
	Vec3 collisionNormal = (target - source);
	collisionNormal.Normalize();
	Vec3 force = collisionNormal * massDifference * 10.0f; // �и� ����
	rigidbody->Addforce(force);
}

