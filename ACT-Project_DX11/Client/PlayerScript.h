#pragma once
#include "MonoBehaviour.h"

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

	void StartAttack();
	void ContinueAttack();
	void PlayAttackAnimation(int stage);
	void ResetToIdleState();
private:
	float _speed = 5.f;
	shared_ptr<Model> _player;
	shared_ptr<ModelAnimator> _modelAnimator;
	shared_ptr<Transform> _transform;

private:
	float _FPS;

	// Attack
	int _attackStage = 0; // ���� ���� �ܰ� (0: Idle, 1~4: ���� ���� �ܰ�)
	bool _isAttacking = false; // ���� ������ ����
	float _attackCooldown = 0.f; // ���� �ִϸ��̼� �ּ� ���� �ð�
	float _attackTimer = 0.0f; // ���� ���� �ܰ��� ��� �ð�

	float _attackDurations[4]; // �� ���� �ִϸ��̼� ���� �ð� (��)
	float _currentDuration = 0.f;

	bool _isPlayeringAttackAnimation = false; // ���� �ִϸ��̼� ��� ������ ���� Ȯ��
	AnimationState _currentAnimationState = AnimationState::Idle;
};

