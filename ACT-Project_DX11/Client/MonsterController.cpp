#include "pch.h"
#include "MonsterController.h"

void MonsterController::Start()
{
    Super::Start();

    // 몬스터 공통 초기화
    std::cout << "MonsterController Start()" << std::endl;
}

void MonsterController::Update()
{
    Super::Update();
    // 몬스터 공통 업데이트
    // std::cout << "MonsterController Update()" << std::endl;
}
