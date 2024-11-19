#include "pch.h"
#include "PlayerScript.h"
#include "Model.h"
#include "Camera.h"
#include "ModelAnimator.h"
#include "MyCoroutine.h"
#include <coroutine>

void PlayerScript::Start()
{
}

std::coroutine_handle<MyCoroutine::promise_type> coroutineHandle;

void EndAttackCoroutine() {
	if (coroutineHandle) {
		coroutineHandle.destroy();
		coroutineHandle = nullptr;
	}
}

// �÷��̾���� �ڷ�ƾ �Լ� ����
MyCoroutine PlayAttackAnimation(PlayerScript* playerScript, float animationDuration)
{
	// �ִϸ��̼� ��� �ð� ���
	co_await AwaitableSleep(chrono::milliseconds(static_cast<int>(animationDuration * 1000)));
	// �ִϸ��̼��� ���� �� ���� ����
	playerScript->ResetAnimationState();
}

void PlayerScript::Update()
{
	float dt = TIME->GetDeltaTime();
	_transform = GetTransform();

	Vec3 moveDir = Vec3(0.0f);
	bool isRunning = INPUT->GetButton(KEY_TYPE::SHIFT);  // Shift Ű�� �޸��� ��� ���� Ȯ��
	bool isAttack = false;  // ��Ŭ������ ���� ���� Ȯ��


	// �̵� �Է� ó��
	if (INPUT->GetButton(KEY_TYPE::W))
		moveDir += Vec3(0.0f, 0.0f, 1.0f);
	if (INPUT->GetButton(KEY_TYPE::S))
		moveDir += Vec3(0.0f, 0.0f, -1.0f);
	if (INPUT->GetButton(KEY_TYPE::A))
		moveDir += Vec3(-1.0f, 0.0f, 0.0f);
	if (INPUT->GetButton(KEY_TYPE::D))
		moveDir += Vec3(1.0f, 0.0f, 0.0f);
	if (INPUT->GetButton(KEY_TYPE::LBUTTON))
		isAttack = true;

	if (isAttack && !_isPlayeringAttackAnimation)
	{
		OutputDebugString(L"x\n");
		// ���� �ִϸ��̼� ��� �� ���� ����
		_isPlayeringAttackAnimation = true;

		// ���� �ִϸ��̼� ��� ���·� ����
		SetAnimationState(AnimationState::Attack);

		// ���� �ִϸ��̼� �ڷ�ƾ ����
		float attackDuration = _player->GetAnimationDuration(AnimationState::Attack);
		MyCoroutine attackCoroutine = PlayAttackAnimation(this, attackDuration);
		coroutineHandle = attackCoroutine.GetHandler();
		coroutineHandle.resume();

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

void PlayerScript::ResetAnimationState()
{
	_isPlayeringAttackAnimation = false;
	EndAttackCoroutine();
	SetAnimationState(AnimationState::Idle); // �⺻ ���·� ����
}
