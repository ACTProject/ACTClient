#include "pch.h"
#include "CreatureController.h"


void CreatureController::Start()
{
    // 생명체 공통적인 초기화 로직
    std::cout << "CreatureController Start()" << std::endl;
}

void CreatureController::Update()
{
    // 생명체 공통적인 업데이트 로직
    // std::cout << "CreatureController Update()" << std::endl;
}

// 데미지 처리
void CreatureController::OnDamage(float damage)
{
    _hp -= damage;

    string name = "";
    switch (GetMonoBehaviourType())
    {
    case MonoBehaviourType::Player:
        name = "player";
        break;
    case MonoBehaviourType::MelleMonster:
        name = "MelleMonster";
        break;
    case MonoBehaviourType::ShootingMonster:
        name = "ShootingMonster";
        break;
    case MonoBehaviourType::FinalBossMonster:
        name = "FinalBossMonster";
        break;
    default:
        name = "null";
    }
    std::cout << name << " took " << damage << " damage! Remaining HP: " << _hp << std::endl;

    if (_hp <= 0.0f)
    {
        OnDeath();
    }
}

// 죽음 처리
void CreatureController::OnDeath()
{
    GetGameObject()->Destroy();
}