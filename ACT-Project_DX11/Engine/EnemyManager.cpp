#include "pch.h"
#include "EnemyManager.h"

void EnemyManager::CreateMeleeMonster(Vec3 SpawnPos)
{
    auto rangoon = make_shared<GameObject>();
    {
        rangoon->SetObjectType(ObjectType::Monster);
        rangoon->GetOrAddTransform()->SetPosition(SpawnPos);
        rangoon->GetOrAddTransform()->SetLocalRotation(Vec3(0, 0, 0)); // XMConvertToRadians()
        rangoon->GetOrAddTransform()->SetScale(Vec3(0.0001f));

        shared_ptr<Model> enemyModel = make_shared<Model>();
        // Model
        {
            enemyModel->ReadModel(L"Enemy/Rangoon");
            enemyModel->ReadMaterial(L"Enemy/Rangoon");

            enemyModel->ReadAnimation(L"Enemy/Rangoon_idle", AnimationState::Idle);
            enemyModel->ReadAnimation(L"Enemy/Rangoon_run", AnimationState::Run);
            enemyModel->ReadAnimation(L"Enemy/Rangoon_aggro", AnimationState::Aggro);
            enemyModel->ReadAnimation(L"Enemy/Rangoon_atkBigSnippy", AnimationState::Attack1);
            enemyModel->ReadAnimation(L"Enemy/Rangoon_atkSmallSnippy", AnimationState::Attack2);
            enemyModel->ReadAnimation(L"Enemy/Rangoon_atkSmash", AnimationState::Attack3);
            enemyModel->ReadAnimation(L"Enemy/Rangoon_die", AnimationState::Die);
            enemyModel->ReadAnimation(L"Enemy/Rangoon_hit", AnimationState::Hit1);
            enemyModel->ReadAnimation(L"Enemy/Rangoon_roar", AnimationState::Roar);
        }
        shared_ptr<ModelAnimator> ma2 = make_shared<ModelAnimator>(renderShader);
        rangoon->AddComponent(ma2);
        {
            rangoon->GetModelAnimator()->SetModel(enemyModel);
            rangoon->GetModelAnimator()->SetPass(2);
        }
        shared_ptr<MelleMonsterController> rangoonScript = make_shared<MelleMonsterController>();

        rangoonScript->SetEnemy(enemyModel);
        rangoonScript->SetModelAnimator(ma2);

        rangoon->SetController(rangoonScript);
        rangoon->AddComponent(rangoonScript);

        // Collider
        auto collider = make_shared<AABBBoxCollider>();
        collider->SetBoundingBox(BoundingBox(Vec3(0.f), Vec3(1.5f)));
        collider->SetOffset(Vec3(0.f, 1.f, 0.f));
        OCTREE->InsertCollider(collider);
        rangoon->AddComponent(collider);

        // Rigidbody
        shared_ptr<Rigidbody> rigidBody = make_shared<Rigidbody>();
        rigidBody->SetUseGravity(true);
        rigidBody->SetMass(2.0f + (rand() % 10 * 0.1));
        rangoon->AddComponent(rigidBody);

        COLLISION->AddRigidbody(rigidBody);
        COLLISION->AddCollider(collider);

        CUR_SCENE->Add(rangoon);
    }
}

void EnemyManager::CreateShootingMonsterController(Vec3 SpawnPos)
{
    auto PistolShrimp = make_shared<GameObject>(); // Pistol_Shrimp
    {
        PistolShrimp->SetObjectType(ObjectType::Monster);
        PistolShrimp->GetOrAddTransform()->SetPosition(SpawnPos);
        PistolShrimp->GetOrAddTransform()->SetLocalRotation(Vec3(0, XMConvertToRadians(180), 0)); // XMConvertToRadians()
        PistolShrimp->GetOrAddTransform()->SetScale(Vec3(0.01f));
        //Vec3(XMConvertToRadians(90), 0, XMConvertToRadians(90))
        shared_ptr<Model> enemyModel = make_shared<Model>();
        // Model
        {
            enemyModel->ReadModel(L"Enemy/pistol");
            enemyModel->ReadMaterial(L"Enemy/pistol");

            enemyModel->ReadAnimation(L"Enemy/pistol_Idle", AnimationState::Idle);
            enemyModel->ReadAnimation(L"Enemy/pistol_shoot", AnimationState::Attack1);
            enemyModel->ReadAnimation(L"Enemy/pistol_aggro", AnimationState::Aggro);

        }
        shared_ptr<ModelAnimator> ma2 = make_shared<ModelAnimator>(renderShader);
        PistolShrimp->AddComponent(ma2);
        {
            PistolShrimp->GetModelAnimator()->SetModel(enemyModel);
            PistolShrimp->GetModelAnimator()->SetPass(2);
        }
        shared_ptr<ShootingMonsterController> ShrimpScript = make_shared<ShootingMonsterController>();

        ShrimpScript->SetEnemy(enemyModel);
        ShrimpScript->SetModelAnimator(ma2);

        PistolShrimp->SetController(ShrimpScript);
        PistolShrimp->AddComponent(ShrimpScript);

        // Collider
        auto collider = make_shared<AABBBoxCollider>();
        collider->SetBoundingBox(BoundingBox(Vec3(0.f), Vec3(1.5f)));
        collider->SetOffset(Vec3(0.f, 1.f, 0.f));
        OCTREE->InsertCollider(collider);
        PistolShrimp->AddComponent(collider);

        // Rigidbody
        shared_ptr<Rigidbody> rigidBody = make_shared<Rigidbody>();
        rigidBody->SetUseGravity(true);
        rigidBody->SetMass(2.1f);
        PistolShrimp->AddComponent(rigidBody);

        COLLISION->AddRigidbody(rigidBody);
        COLLISION->AddCollider(collider);

        CUR_SCENE->Add(PistolShrimp);
    }
}

void EnemyManager::CreateFinalBoss(Vec3 SpawnPos)
{
    auto FinalBoss = make_shared<GameObject>(); // MR_Krab
    {
        FinalBoss->SetObjectType(ObjectType::Monster);
        FinalBoss->GetOrAddTransform()->SetPosition(SpawnPos);
        FinalBoss->GetOrAddTransform()->SetLocalRotation(Vec3(0, 0, 0)); // XMConvertToRadians()
        FinalBoss->GetOrAddTransform()->SetScale(Vec3(0.0005f));

        shared_ptr<Model> enemyModel = make_shared<Model>();
        // Model
        {
            enemyModel->ReadModel(L"Enemy/FinalBoss/mrKrab");
            enemyModel->ReadMaterial(L"Enemy/FinalBoss/mrKrab");

            enemyModel->ReadAnimation(L"Enemy/FinalBoss/00_mrKRAB_idle", AnimationState::Idle);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/01_boss_walk", AnimationState::Walk);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/02_boss_roar", AnimationState::Roar);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/03_warming_up", AnimationState::Appear);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/04_fight_idle", AnimationState::Combat);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/05_run_forward", AnimationState::Run);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/06_sprint", AnimationState::Run2);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/07_back_sprint", AnimationState::Run3);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/08_jumping", AnimationState::Jump);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/09_hit_1", AnimationState::Hit1);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/10_hit_2", AnimationState::Hit2);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/11_boxing_1", AnimationState::Attack1);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/12_boxing_2", AnimationState::Attack2);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/13_boxing_3", AnimationState::Attack3);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/14_boxing_4", AnimationState::Attack4);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/15_getUp_behind", AnimationState::GetUP1); // 이상함
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/16_getUp_front", AnimationState::GetUP2);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/17_knockedDown_back", AnimationState::Down1);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/18_knockedDown_forward", AnimationState::Down2);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/19_choke_lift", AnimationState::Skill1);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/20_fireball", AnimationState::Skill2);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/21_magic_spell", AnimationState::Skill3);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/22_sweep", AnimationState::Skill4);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/23_thrust_slash", AnimationState::Skill5);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/24_uppercut", AnimationState::Skill6);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/25_gettingThrown", AnimationState::Skill7);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/26_grabSlam", AnimationState::Skill8);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/27_hurricane", AnimationState::Skill9);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/28_dying", AnimationState::Die);

        }
        shared_ptr<ModelAnimator> ma2 = make_shared<ModelAnimator>(renderShader);
        FinalBoss->AddComponent(ma2);
        {
            FinalBoss->GetModelAnimator()->SetModel(enemyModel);
            FinalBoss->GetModelAnimator()->SetPass(2);
        }
        shared_ptr<FinalBossMonsterContoller> BossScript = make_shared<FinalBossMonsterContoller>();

        BossScript->SetEnemy(enemyModel);
        BossScript->SetModelAnimator(ma2);
        FinalBoss->AddComponent(BossScript);

        // Collider
        auto collider = make_shared<SphereCollider>();
        collider->SetRadius(30.0f);
        //collider->SetBoundingBox(BoundingBox(Vec3(0.f), Vec3(2.0f)));
        collider->SetOffset(Vec3(0.f, 2.5f, 0.f));
        OCTREE->InsertCollider(collider);
        FinalBoss->AddComponent(collider);

        // Rigidbody
        shared_ptr<Rigidbody> rigidBody = make_shared<Rigidbody>();
        rigidBody->SetUseGravity(true);
        rigidBody->SetMass(2.1f);
        FinalBoss->AddComponent(rigidBody);

        COLLISION->AddRigidbody(rigidBody);
        COLLISION->AddCollider(collider);

        CUR_SCENE->Add(FinalBoss);
    }
}

void EnemyManager::CreateFinalPhase(Vec3 SpawnPos)
{
    auto FinalBoss = make_shared<GameObject>(); // MR_Krab
    {
        FinalBoss->SetObjectType(ObjectType::Monster);
        FinalBoss->GetOrAddTransform()->SetPosition(SpawnPos);
        FinalBoss->GetOrAddTransform()->SetLocalRotation(Vec3(0, 0, 0)); // XMConvertToRadians()
        FinalBoss->GetOrAddTransform()->SetScale(Vec3(0.0005f));

        shared_ptr<Model> enemyModel = make_shared<Model>();
        // Model
        {
            enemyModel->ReadModel(L"Enemy/FinalBoss/mrKrab");
            enemyModel->ReadMaterial(L"Enemy/FinalBoss/angryKrab");

            enemyModel->ReadAnimation(L"Enemy/FinalBoss/00_mrKRAB_idle", AnimationState::Idle);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/01_boss_walk", AnimationState::Walk);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/02_boss_roar", AnimationState::Roar);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/03_warming_up", AnimationState::Appear);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/04_fight_idle", AnimationState::Combat);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/05_run_forward", AnimationState::Run);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/06_sprint", AnimationState::Run2);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/07_back_sprint", AnimationState::Run3);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/08_jumping", AnimationState::Jump);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/09_hit_1", AnimationState::Hit1);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/10_hit_2", AnimationState::Hit2);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/11_boxing_1", AnimationState::Attack1);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/12_boxing_2", AnimationState::Attack2);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/13_boxing_3", AnimationState::Attack3);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/14_boxing_4", AnimationState::Attack4);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/15_getUp_behind", AnimationState::GetUP1); // 이상함
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/16_getUp_front", AnimationState::GetUP2);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/17_knockedDown_back", AnimationState::Down1);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/18_knockedDown_forward", AnimationState::Down2);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/19_choke_lift", AnimationState::Skill1);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/20_fireball", AnimationState::Skill2);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/21_magic_spell", AnimationState::Skill3);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/22_sweep", AnimationState::Skill4);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/23_thrust_slash", AnimationState::Skill5);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/24_uppercut", AnimationState::Skill6);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/25_gettingThrown", AnimationState::Skill7);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/26_grabSlam", AnimationState::Skill8);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/27_hurricane", AnimationState::Skill9);
            enemyModel->ReadAnimation(L"Enemy/FinalBoss/28_dying", AnimationState::Die);

        }
        shared_ptr<ModelAnimator> ma2 = make_shared<ModelAnimator>(renderShader);
        FinalBoss->AddComponent(ma2);
        {
            FinalBoss->GetModelAnimator()->SetModel(enemyModel);
            FinalBoss->GetModelAnimator()->SetPass(2);
        }
        shared_ptr<FinalBossMonsterContoller> BossScript = make_shared<FinalBossMonsterContoller>();

        BossScript->SetEnemy(enemyModel);
        BossScript->SetModelAnimator(ma2);
        FinalBoss->AddComponent(BossScript);

        // Collider
        auto collider = make_shared<SphereCollider>();
        collider->SetRadius(30.0f);
        //collider->SetBoundingBox(BoundingBox(Vec3(0.f), Vec3(2.0f)));
        collider->SetOffset(Vec3(0.f, 2.5f, 0.f));
        OCTREE->InsertCollider(collider);
        FinalBoss->AddComponent(collider);

        // Rigidbody
        shared_ptr<Rigidbody> rigidBody = make_shared<Rigidbody>();
        rigidBody->SetUseGravity(true);
        rigidBody->SetMass(2.1f);
        FinalBoss->AddComponent(rigidBody);

        COLLISION->AddRigidbody(rigidBody);
        COLLISION->AddCollider(collider);

        CUR_SCENE->Add(FinalBoss);
    }
}
