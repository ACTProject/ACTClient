#include "pch.h"
#include "PlayerScript.h"
#include "Model.h"
#include "Camera.h"
#include "ModelAnimator.h"
#include "MyCoroutine.h"
#include <coroutine>

// Coroutine
std::coroutine_handle<MyCoroutine::promise_type> currentCoroutine;

void EndAttackCoroutine() {
	if (currentCoroutine) {
		currentCoroutine.destroy();
		currentCoroutine = nullptr;
	}
}

// �÷��̾���� �ڷ�ƾ �Լ� ����
MyCoroutine PlayAttackCoroutine(PlayerScript* playerScript, float animationDuration)
{
	// �ִϸ��̼� ��� �ð� ���
	co_await AwaitableSleep(chrono::milliseconds(static_cast<int>(animationDuration * 1000)));
	EndAttackCoroutine();
}

void PlayerScript::Start()
{

}

void PlayerScript::Update()
{
	_FPS = static_cast<float>(TIME->GetFps());
	float dt = TIME->GetDeltaTime();
	for (int i = 0; i < 4; ++i) {
		_attackDurations[i] = _player->GetAnimationDuration(static_cast<AnimationState>((int)AnimationState::Attack1 + i));
	}
	_transform = GetTransform();

	Vec3 moveDir = Vec3(0.0f);
	bool isRunning = INPUT->GetButton(KEY_TYPE::SHIFT);  // Shift Ű�� �޸��� ��� ���� Ȯ��

	// �̵� �Է� ó��
	if (INPUT->GetButton(KEY_TYPE::W))
		moveDir += Vec3(0.0f, 0.0f, 1.0f);
	if (INPUT->GetButton(KEY_TYPE::S))
		moveDir += Vec3(0.0f, 0.0f, -1.0f);
	if (INPUT->GetButton(KEY_TYPE::A))
		moveDir += Vec3(-1.0f, 0.0f, 0.0f);
	if (INPUT->GetButton(KEY_TYPE::D))
		moveDir += Vec3(1.0f, 0.0f, 0.0f);
	// ���� �Է� ó��
	if (INPUT->GetButtonDown(KEY_TYPE::LBUTTON)) {
		_isPlayeringAttackAnimation = true;
		if (!_isAttacking) {
			StartAttack();
		}
		else if (_attackTimer < _attackCooldown) {
			ContinueAttack();
		}
	}

	// ���� Ÿ�̸� ����
	if (_isAttacking) {
		_attackTimer += dt;

		float a = ((_attackDurations[_attackStage - 1]) / _FPS);
		// ���� �ܰ� �ð� �ʰ� �� Idle�� ����
		if (_attackTimer >= a) {
			_attackStage = 0;  // ������ ������ �ƴϸ� �ʱ�ȭ
			_isAttacking = false;
			ResetToIdleState();
		}
	}

	// ���� �ִϸ��̼��� ��� ���̸� �ٸ� �ִϸ��̼� ���·� ��ȯ���� ����
	if (_isPlayeringAttackAnimation)
		return;

	// �̵� ������ ũ�⸦ �������� �ִϸ��̼� ���� ����
	AnimationState targetAnimationState;

	if (moveDir.LengthSquared() > 0.0f)  // �̵� ���Ͱ� 0�� �ƴ϶�� �̵� ������ ����
	{
		moveDir.Normalize();
		float speed = isRunning ? _speed*2 : _speed;
		_transform->SetPosition(_transform->GetPosition() + moveDir * speed * dt);

		targetAnimationState = isRunning ? AnimationState::Run : AnimationState::Walk;


		// �̵� ���⿡ ���� ȸ�� ����
		Vec3 targetForward = moveDir;					// ĳ���Ͱ� �̵��Ϸ��� ����
		Vec3 currentForward = _transform->GetLook();	// ĳ���Ͱ� ���� �ٶ󺸴� ����

		// �� ���� ������ ������ ����Ͽ� ȸ��
		float angle = std::acos(currentForward.Dot(targetForward));	// �� ���� ������ ����
		if (angle != 0.f)
		{
			Vec3 rotationAxis = currentForward.Cross(targetForward);	// �� ���Ͱ� �̷�� ����� ��������
			rotationAxis.Normalize();

			// ȸ�� ���� y ������ �¿� ������ ����
			if (rotationAxis.y < 0) {
				angle = -angle;  // �������� ȸ��
			}
			_transform->SetRotation(_transform->GetRotation() + Vec3(0, angle, 0));

		}
	}
	else
	{
		targetAnimationState = AnimationState::Idle;
	}

	// �ִϸ��̼� ���°� ����Ǿ��� ���� ���� ��ȯ
	if (_currentAnimationState != targetAnimationState)
	{
		SetAnimationState(targetAnimationState);
	}

	if (INPUT->GetButton(KEY_TYPE::KEY_1))
	{
		Camera::S_IsWireFrame = false;
	}

	if (INPUT->GetButton(KEY_TYPE::KEY_2))
	{
		Camera::S_IsWireFrame = true;
	}
}

// ���� �ִϸ��̼� ���� ����
void PlayerScript::SetAnimationState(AnimationState state)
{
	_modelAnimator->ChangeAnimation(state);
	_currentAnimationState = state;
}

void PlayerScript::StartAttack()
{
	_isAttacking = true;
	_attackStage = 1;
	_attackTimer = 0.0f;

	// 1Ÿ ���� �ִϸ��̼� ���
	PlayAttackAnimation(_attackStage);
	MyCoroutine attackCoroutine = PlayAttackCoroutine(this, _attackDurations[_attackStage - 1] / _FPS);
	currentCoroutine = attackCoroutine.GetHandler();
	currentCoroutine.resume();
}

void PlayerScript::ContinueAttack()
{
	if (_attackStage < 4) {
		_attackStage++;
		_attackTimer = 0.0f;
		EndAttackCoroutine();

		// ���� ���� �ִϸ��̼� ���
		PlayAttackAnimation(_attackStage);
		MyCoroutine attackCoroutine = PlayAttackCoroutine(this, _attackDurations[_attackStage - 1] / _FPS);
		currentCoroutine = attackCoroutine.GetHandler();
		currentCoroutine.resume();
	}
}

void PlayerScript::PlayAttackAnimation(int stage) 
{
	switch (stage) 
	{
	case 1:
		SetAnimationState(AnimationState::Attack1);
		break;
	case 2:
		SetAnimationState(AnimationState::Attack2);
		break;
	case 3:
		SetAnimationState(AnimationState::Attack3);
		break;
	case 4:
		SetAnimationState(AnimationState::Attack4);
		break;
	default:
		SetAnimationState(AnimationState::Idle);
		break;
	}
}

void PlayerScript::ResetToIdleState() {
	_isPlayeringAttackAnimation = false;
	EndAttackCoroutine();
	SetAnimationState(AnimationState::Idle);
}