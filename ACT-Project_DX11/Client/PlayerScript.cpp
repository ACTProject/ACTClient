#include "pch.h"
#include "PlayerScript.h"
#include "Model.h"
#include "Camera.h"
#include "ModelAnimator.h"

void PlayerScript::Start()
{
}

void PlayerScript::Update()
{
	float dt = TIME->GetDeltaTime();
	_transform = GetTransform();

	Vec3 moveDir = Vec3(0.0f);
	bool isRunning = INPUT->GetButton(KEY_TYPE::SHIFT);  // Shift Ű�� �޸��� ��� ���� Ȯ��
	bool isAttack = false;  // Shift Ű�� �޸��� ��� ���� Ȯ��


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

	// �̵� ������ ũ�⸦ �������� �ִϸ��̼� ���� ����
	AnimationState targetAnimationState;

	//if (isAttack)
	//{
	//	targetAnimationState = AnimationState::Attack;
	//}
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
		_modelAnimator->SetAnimationState(targetAnimationState);
		_currentAnimationState = targetAnimationState;  // ���� ���� ������Ʈ
	}
	

	if (isAttack)
	{
		_modelAnimator->SetAnimationState(AnimationState::Attack);
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
