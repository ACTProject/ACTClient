#include "pch.h"
#include "Bullet.h"
#include "../Client/PlayerController.h"

Bullet::Bullet() : Super(ComponentType::Bullet)
{

}

Bullet::~Bullet()
{

}

void Bullet::Update()
{
    Shooting();
}

void Bullet::Remove()
{
    CUR_SCENE->Remove(GetGameObject());
    OCTREE->RemoveCollider(GetGameObject()->GetCollider());
}

void Bullet::UpdateHitBox()
{
    auto hitboxCollider = _hitbox->GetCollider();
    hitboxCollider->SetActive(true);

    _hitbox->GetTransform()->SetLocalPosition(_transform->GetPosition());

    vector<shared_ptr<BaseCollider>> nearbyColliders = OCTREE->QueryColliders(hitboxCollider);

    for (const auto& collider : nearbyColliders)
    {
        ObjectType type = collider->GetGameObject()->GetObjectType();
        if (type != ObjectType::Player)
            continue;

        if (hitboxCollider->Intersects(collider))
        {
            auto controller = collider->GetGameObject()->GetController();
            if (!controller)
                continue;

            auto player = dynamic_pointer_cast<PlayerController>(controller);
            if (player)
                player->OnDamage(10.0f);
            _isHit = true;
            Remove();
        }
    }
}

void Bullet::Shooting()
{
    _transform = GetTransform();
    Vec3 pos = _transform->GetPosition();
    direction.Normalize();

    duration += DT;

    if (duration > 3.0f) // 3초 뒤 삭제
    {
        Remove();
    }

    {
        pos += direction * speed * DT;
        _transform->SetPosition(pos);
        UpdateHitBox();
    }
}