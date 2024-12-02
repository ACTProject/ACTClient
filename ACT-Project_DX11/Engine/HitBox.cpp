#include "pch.h"
#include "HitBox.h"
#include "AABBBoxCollider.h"

HitBox::HitBox() : Super(ComponentType::HitBox)
{

}

HitBox::~HitBox()
{

}

void HitBox::Craete(shared_ptr<GameObject> player)
{
	auto go = _gameObject.lock();
	if (!go) return;

	// �÷��̾� ��ġ�� �������� ��Ʈ�ڽ� ��ġ ����
	Vec3 playerPosition = player->GetTransform()->GetPosition();
	go->GetOrAddTransform()->SetPosition(playerPosition + _offset);

	// Collider
	auto collider = make_shared<AABBBoxCollider>();
	collider->SetRadius(5.0f);
	collider->SetOffset(Vec3(0.f, 1.f, 0.f));
	player->AddComponent(collider);
}
