#pragma once
#include "MonoBehaviour.h"
#include "Camera.h"

class Model;
class ModelAnimator;

class PlayerScript : public MonoBehaviour
{
	virtual void Start() override;
	virtual void Update() override;

public:
	shared_ptr<Model> GetPlayer() { return _player; }
	void SetPlayer(shared_ptr<Model> player) { _player = player; }	
	shared_ptr<ModelAnimator> GetModelAnimator() { return _modelAnimator; }
	void SetModelAnimator(shared_ptr<ModelAnimator> modelAnimator) { _modelAnimator = modelAnimator; }
	void SetAnimationState(AnimationState state);
	void SetHitBox(shared_ptr<GameObject> hitbox) { _hitbox = hitbox; }
	void SetCamera(shared_ptr<GameObject> camera) { _camera = camera; }

	void StartAttack();
	void ContinueAttack();
	void PlayAttackAnimation(int stage);
    void Jump();
	void ResetToIdleState();
private:
	float _speed = 5.f;
	shared_ptr<Model> _player;
	shared_ptr<ModelAnimator> _modelAnimator;
	shared_ptr<Transform> _transform;
    shared_ptr<GameObject> _camera;
	shared_ptr<GameObject> _hitbox;
	shared_ptr<Rigidbody> _rigidbody;

private:
	float _FPS;

    // Jump
    float _jumpSpeed = 7.5f;
    bool _isJumping = false;
    float _jumpDuration = 0.0f; // 점프 애니메이션 지속시간 (초)
    float _jumpTimer = 0.0f;   // 점프 애니메이션 시간 추적

	// Attack
	int _attackStage = 0; // 현재 공격 단계 (0: Idle, 1~4: 연속 공격 단계)
	bool _isAttacking = false; // 공격 중인지 여부
	float _attackCooldown = 0.f; // 공격 애니메이션 최소 실행 시간
	float _attackTimer = 0.0f; // 현재 공격 단계의 경과 시간

	float _attackDurations[4]; // 각 공격 애니메이션 지속 시간 (초)
	float _currentDuration = 0.f;

    // AttackMove
    float _attackMoveDistance = 1.0f;  // 공격 시 이동할 거리
    float _attackMoveSpeed = 2.0f;     // 이동 속도
    float _attackMoveTimer = 0.0f;     // 이동 타이머 초기화

	bool _isPlayeringJumpAnimation = false; // 점프 애니메이션 재생 중인지 여부 확인

	bool _isPlayeringAttackAnimation = false; // 공격 애니메이션 재생 중인지 여부 확인
	AnimationState _currentAnimationState = AnimationState::Idle;
};

