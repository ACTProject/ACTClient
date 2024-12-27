#include "pch.h"
#include "CreatureController.h"
#include "PlayerController.h"
#include "Slider.h"


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
void CreatureController::OnDamage(shared_ptr<GameObject> attacker, float damage)
{
    _hp -= damage;

    string name = "";
    auto uiList = UIMANAGER->GetUIList();

    auto controller = GetGameObject()->GetController();

    switch (GetMonoBehaviourType())
    {
    case MonoBehaviourType::Player:
    {
        name = "player";
        auto player = dynamic_pointer_cast<PlayerController>(controller);
        float hpRatio = _hp / _maxHp;

        float shellMaxHp = player->GetShellMaxHP();
        float shellHp = player->GetShellHP();
        for (auto& ui : uiList)
        {
            if (ui->GetUIID() == "PlayerHP")
            {
                auto hpSlider = dynamic_pointer_cast<Slider>(ui);
                hpSlider->SetRatio(hpRatio);
            }
            if (ui->GetUIID() == "PlayerArmor")
            {
                auto shellSlider = dynamic_pointer_cast<Slider>(ui);
                shellSlider->SetRatio(hpRatio);
            }
        }
        break;
    }

    case MonoBehaviourType::MelleMonster:
    {
        name = "MelleMonster";
        float hpRatio = _hp / _maxHp;
        for (auto& ui : uiList)
        {
            if (ui->GetUIID() == "Enemy")
            {
                auto slider = dynamic_pointer_cast<Slider>(ui);
                slider->SetRatio(hpRatio);
            }
        }
        break;
    }
    case MonoBehaviourType::ShootingMonster:
        name = "ShootingMonster";
        break;
    case MonoBehaviourType::FinalBossMonster_1:
        name = "FinalBossMonster";
        break;
    case MonoBehaviourType::FinalBossMonster_2:
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
    OCTREE->RemoveCollider(GetGameObject()->GetCollider());
    COLLISION->Remove(GetGameObject());
    CUR_SCENE->Remove(GetGameObject());

    TaskQueue::GetInstance().AddTask([this]() {
        std::cout << "Destroying object in TaskQueue..." << std::endl;
        GetGameObject()->Destroy();
        });
}