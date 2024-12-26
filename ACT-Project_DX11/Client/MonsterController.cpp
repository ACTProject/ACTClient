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
    _FPS = static_cast<float>(TIME->GetFps());
    // 몬스터 공통 업데이트
    // std::cout << "MonsterController Update()" << std::endl;
}


void MonsterController::Die()
{
    if (PlayCheckAnimating(AnimationState::Die))
    {
        return;
    }
    Super::OnDeath();
}

bool MonsterController::PlayCheckAnimating(AnimationState state)
{
    SetAnimationState(state);

    animPlayingTime += DT;
    duration = _enemy->GetAnimationDuration(state) / _FPS;

    if (animPlayingTime >= duration)
    {
        animPlayingTime = 0.0f;
        ResetToIdleState();
        return false;
    }

    return true; // 플레이 중
}

void MonsterController::ResetToIdleState()
{
    SetAnimationState(AnimationState::Idle);
}

void MonsterController::SetAnimationState(AnimationState state)
{
    _modelAnimator->ChangeAnimation(state);
    _currentAnimationState = state;
}