#pragma once
#include "../Client/ShootingMonsterController.h"
#include "../Client/MelleMonsterController.h"
#include "../Client/FinalBossMonsterContoller.h"
#include "Component.h"

class CreatureBase
{
    DECLARE_SINGLE(CreatureBase);
public:

    void SetCreatrue(shared_ptr<GameObject> GameObject) { _creatrue = GameObject; }

    void Move(Vec3 objPos, Vec3 targetPos, float speed);
    void Rota(Vec3 objPos, Vec3 targetPos);


    shared_ptr<Transform> _transform;
    shared_ptr<GameObject> _creatrue;

};

