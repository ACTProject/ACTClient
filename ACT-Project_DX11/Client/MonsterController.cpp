#include "pch.h"
#include "MonsterController.h"

void MonsterController::Start()
{
    Super::Start();

    _player = SCENE->GetCurrentScene()->GetPlayer();

    // 몬스터 공통 초기화
    std::cout << "MonsterController Start()" << std::endl;
}

void MonsterController::Update()
{
    Super::Update();

    currentTime = GAMETIME; // 현재 게임 시간
    _FPS = FPS;

    playerPos = _player->GetTransform()->GetPosition();
    // 몬스터 공통 업데이트
    // std::cout << "MonsterController Update()" << std::endl;
}
