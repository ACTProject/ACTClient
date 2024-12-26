#pragma once
#include "CreatureController.h"
/*
    MonsterController는 모든 몬스터의 공통 동작을 관리.
*/
class MonsterController : public CreatureController
{
    using Super = CreatureController;

public:
    void Start() override;
    void Update() override;

    void Die();
    bool PlayCheckAnimating(AnimationState state);
    void ResetToIdleState();
    void SetAnimationState(AnimationState state);

    bool _isDead = false;
    float animPlayingTime = 0.0f;

    float _FPS;
    float duration;
    float distance;
    Vec3 direction;


    shared_ptr<Model> _enemy;
    shared_ptr<GameObject> _hitbox;
    shared_ptr<ModelAnimator> _modelAnimator;
    shared_ptr<Transform> _transform;
    shared_ptr<GameObject> _player;
    AnimationState _currentAnimationState = AnimationState::Idle;
};

