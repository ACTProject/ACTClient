#include "pch.h"
#include "HitBox.h"
#include "AABBBoxCollider.h"

HitBox::HitBox() : Super(ComponentType::HitBox)
{

}

HitBox::~HitBox()
{

}

void HitBox::Craete(shared_ptr<GameObject> target, Vec3 size)
{
	auto go = _gameObject.lock();
	if (!go) return;

	// 타겟의 Look벡터를 기준으로 히트박스 위치 설정
	auto targetTransform = target->GetTransform();
	go->GetOrAddTransform()->SetPosition(targetTransform->GetPosition() +_offset + targetTransform->GetLook() * 2.5f);

	// Collider
	auto collider = make_shared<AABBBoxCollider>();
    collider->SetBoundingBox(BoundingBox(Vec3(0.f), size));
	collider->SetOffset(Vec3(0.f, 1.f, 0.f));
	go->AddComponent(collider);
}

void HitBox::AirHitCraete(shared_ptr<GameObject> target, Vec3 size)
{
    auto go = _gameObject.lock();
    if (!go) return;

    // 타겟의 중심 위치를 기준으로 히트박스 위치 설정
    auto targetTransform = target->GetTransform();
    go->GetOrAddTransform()->SetPosition(targetTransform->GetPosition() + _offset);

    // Collider
    auto collider = make_shared<AABBBoxCollider>();
    collider->SetBoundingBox(BoundingBox(Vec3(0.f),size));
    collider->SetOffset(Vec3(0.f, 1.f, 0.f));
    go->AddComponent(collider);
}