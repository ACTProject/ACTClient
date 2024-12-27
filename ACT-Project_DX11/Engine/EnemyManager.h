#pragma once
#include "../Client/ShootingMonsterController.h"
#include "../Client/MelleMonsterController.h"
#include "../Client/FinalBossMonsterFirstPhaseController.h"
#include "../Client/FinalBossMonsterSecondPhaseController.h"
#include "Rigidbody.h"
#include "AABBBoxCollider.h"
#include "Slider.h"
#include "Ui.h"
#include "Material.h"

class EnemyManager
{
    DECLARE_SINGLE(EnemyManager);

    shared_ptr<Shader> renderShader = make_shared<Shader>(L"23. RenderDemo.fx");

public:
    void CreateMeleeMonster(Vec3 SpawnPos, int num);
    void CreateShootingMonster(Vec3 SpawnPos);
    void CreateFinalBoss(Vec3 SpawnPos);
    shared_ptr<GameObject> CreateFinalPhase(Vec3 SpawnPos);

    Vec3 Spawnpos;
};

