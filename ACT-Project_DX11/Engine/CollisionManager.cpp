#include "pch.h"
#include "CollisionManager.h"
#include "BaseCollider.h"
#include "Rigidbody.h"

void CollisionManager::Init()
{

}

// 충돌 감지 및 처리
void CollisionManager::Update()
{
    // 모든 Collider를 순회하며 충돌 검사
    for (const auto& colliderA : _colliders)
    {
        // Collider가 비활성화 상태이면 충돌 검사 중단
        if (!colliderA->IsActive() || colliderA == nullptr)
            continue;

        // 옥트리에서 colliderA와 잠재적으로 충돌할 수 있는 콜라이더 검색
        auto nearbyColliders = OCTREE->QueryColliders(colliderA);

        for (const auto& colliderB : nearbyColliders)
        {
            // 동일한 콜라이더나 비활성화된 콜라이더는 무시
            if (colliderA == colliderB || !colliderB->IsActive())
                continue;

            if (colliderA->GetGameObject()->GetRigidbody() == nullptr ||
                colliderB->GetGameObject()->GetRigidbody() == nullptr)
                continue;

            // 충돌 검사 작업을 작업 큐에 추가
            TaskQueue::GetInstance().AddTask([this, colliderA, colliderB]() {
                if (colliderA->Intersects(colliderB))
                {
                    HandleCollision(colliderA, colliderB);
                }
            });
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
	// Rigidbody 가져오기
	auto rigidbodyA = colliderA->GetGameObject()->GetRigidbody();
	auto rigidbodyB = colliderB->GetGameObject()->GetRigidbody();

	// Rigidbody가 없으면 처리하지 않음
	if (!rigidbodyA || !rigidbodyB)
		return;

	// Penetration Depth 계산
	Vec3 penetrationDepth;
	if (!colliderA->CalculatePenetraionDepth(colliderB, penetrationDepth))
	{
		return; // 충돌이 발생하지 않음
	}

	// 충돌 방향 계산
	penetrationDepth.Normalize();
	Vec3 collisionNormal = penetrationDepth;

	// 간격 유지 설정
	float minimumSeparation = 0.1f; // 오브젝트 간 최소 간격
	float penetrationLength = penetrationDepth.Length();

	if (penetrationLength <= minimumSeparation)
	{
		// 이미 충분한 거리가 유지되면 더 이상 밀어내지 않음
		return;
	}

	// 질량 확인
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

	// 충돌 상태 설정
	colliderA->SetColliding(true);
	colliderB->SetColliding(true);

    if (colliderA->GetGameObject()->GetObjectType() == ObjectType::JumpObj ||
        colliderB->GetGameObject()->GetObjectType() == ObjectType::JumpObj)
    {
        if (colliderA->GetGameObject()->GetObjectType() == ObjectType::Player ||
            colliderB->GetGameObject()->GetObjectType() == ObjectType::Player)
        {
            SOUND->PlayEffect(L"player_springSound");
            SOUND->PlayEffect(L"player_scream");
        }
        else if (colliderA->GetGameObject()->GetObjectType() == ObjectType::Monster ||
                 colliderB->GetGameObject()->GetObjectType() == ObjectType::Monster)
        {
            SOUND->PlayEffect(L"player_springSound");
        }
    }
}

void CollisionManager::ApplyForce(shared_ptr<Rigidbody> rigidbody, const Vec3& target, const Vec3& source, float massDifference)
{
	Vec3 collisionNormal = (target - source);
	collisionNormal.Normalize();
	Vec3 force = collisionNormal * massDifference * 10.0f; // 밀림 강도
	rigidbody->Addforce(force);
}

void CollisionManager::Remove(shared_ptr<GameObject> obj)
{
    auto it = std::find(_colliders.begin(), _colliders.end(), obj->GetCollider());
    if (it != _colliders.end())
    {
        _colliders.erase(it);
    }

    auto its = std::find(_rigidbodies.begin(), _rigidbodies.end(), obj->GetRigidbody());
    if (its != _rigidbodies.end())
    {
        _rigidbodies.erase(its);
    }
}

