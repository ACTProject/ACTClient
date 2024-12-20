#pragma once
#include "../Client/ShootingMonster.h"
#include "../Client/MelleMonster.h"
#include "../Client/FinalBossMonster.h"
#include "Component.h"

class CreatureBase
{
    DECLARE_SINGLE(CreatureBase);
public:

    void SetCreatrueTransform(shared_ptr<Transform> transform) { _transform = transform; }
    void SetCreatrue(shared_ptr<GameObject> GameObject) { _creatrue = GameObject; }

    void Move(Vec3 objPos, Vec3 targetPos, float speed);
    void Rota(Vec3 objPos, Vec3 targetPos);


    shared_ptr<Transform> _transform;
    shared_ptr<GameObject> _creatrue;

};

