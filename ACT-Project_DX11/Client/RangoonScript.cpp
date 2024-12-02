#include "pch.h"
#include "RangoonScript.h"
#include "MyCoroutine.h"
#include <coroutine>

constexpr float EPSILON = 0.01f;
// Coroutine
std::coroutine_handle<MyCoroutine::promise_type> currentRangoonCoroutine;

void RangoonEndCoroutine() {
	if (currentRangoonCoroutine) {
		currentRangoonCoroutine.destroy();
		currentRangoonCoroutine = nullptr;
	}
}

// �÷��̾���� �ڷ�ƾ �Լ� ����
MyCoroutine RangoonCoroutine(RangoonScript* rangoonScript, float animationDuration)
{
	// �ִϸ��̼� ��� �ð� ���
	co_await AwaitableSleep(chrono::milliseconds(static_cast<int>(animationDuration * 1000)));		// ���� �� `atkType`�� �������� ����
	RangoonEndCoroutine();
}

void RangoonScript::Move(const Vec3 targetPos)
{
	direction = targetPos - _transform->GetPosition();
	
	if (direction.LengthSquared() < EPSILON) // EPSILON ���
	{
		return;
	}
	
	direction.Normalize();  // ���� ���͸� ���� ���ͷ� ����ȭ

	_transform->SetPosition(_transform->GetPosition() + direction * _speed * _deltaTime);  // ���� �Ÿ���ŭ �̵�
}

void RangoonScript::Rota(const Vec3 targetPos)
{
	CurForward = _transform->GetLook();
	direction = targetPos - _transform->GetPosition();
	direction.Normalize();

	// ������ �̿��� ȸ�� �� ���
	Vec3 rotationAxis = CurForward.Cross(direction);

	// ���� ����� �ſ� ������ ���� ���̰� ���� �����Ƿ� ȸ�� �ʿ� ����
	if (rotationAxis.LengthSquared() < EPSILON)
	{
		return;
	}

	rotationAxis.Normalize();

	// ���� ���
	float angle = std::acos(CurForward.Dot(direction));

	// ���� ������ ����
	if (abs(angle) < EPSILON) // 0.01 ����(�� 0.57��) ���� ȸ�� ����
	{
		return;
	}
	// ���⿡ ���� ���� ���� (y�� �߽� ȸ��)
	if (rotationAxis.y < 0) {
		angle = -angle;  // ���� ȸ��
	}

	// ���� ȸ���� ������Ʈ
	Vec3 currentRotation = _transform->GetLocalRotation();
	Vec3 newRotation = Vec3::Lerp(currentRotation, currentRotation + Vec3(0, angle, 0), 0.1f); // 0.1f�� ���� �ӵ�
	_transform->SetRotation(newRotation);

}

void RangoonScript::Tracking(Vec3 pos, const std::vector<Node3D>& path)
{
	if (path.empty()) {
		return;
	}

	// ��� ���� �� ��带 ���� �̵�
	for (size_t i = 0; i < path.size(); ++i) {
		// ���� ��ġ�� ��ǥ ��忡 �����ߴٸ� ���� ���� �̵�
		if (i + 1 < path.size()) {
			//Move(path[i + 1].pos);
		}
	}
}

void RangoonScript::Attack(int type)
{
	_isAnimating = true;

	float atkDuration = _attackDuration[atkType] / _FPS;

	switch (type)
	{
	case 0:
		SetAnimationState(AnimationState::Attack1);
		break;
	case 1:
		SetAnimationState(AnimationState::Attack2);
		break;
	case 2:
		SetAnimationState(AnimationState::Attack3);
		break;
	}

	// �ڷ�ƾ ����
	MyCoroutine attackCoroutine = RangoonCoroutine(this, atkDuration);
	currentRangoonCoroutine = attackCoroutine.GetHandler();
	currentRangoonCoroutine.resume();

}

void RangoonScript::Aggro()
{
	_isAnimating = true;

	float duration = _aggroDuration / _FPS;

	SetAnimationState(AnimationState::Aggro);
	MyCoroutine aggroCoroutine = RangoonCoroutine(this, duration);
	currentRangoonCoroutine = aggroCoroutine.GetHandler();
	currentRangoonCoroutine.resume();
}

void RangoonScript::Start()
{
	_transform = GetTransform();
	StartPos = _transform->GetPosition();
	for (int i = 0; i < 3; ++i)
	{
		_attackDuration[i] = _rangoon->GetAnimationDuration(static_cast<AnimationState>((int)AnimationState::Attack1 + i));
	}
	_aggroDuration = _rangoon->GetAnimationDuration(static_cast<AnimationState>((int)AnimationState::Aggro));
}

void RangoonScript::Update()
{
	if (INPUT->GetButton(KEY_TYPE::KEY_4))
	{
		int a = 0;
	}

	_FPS = static_cast<float>(TIME->GetFps());
	float dt = TIME->GetDeltaTime();

	if (_isAnimating)
	{
		animPlayingTime += dt;

		if (_currentAnimationState == AnimationState::Attack1 ||
			_currentAnimationState == AnimationState::Attack2 ||
			_currentAnimationState == AnimationState::Attack3)
		{
			float atkDuration = _attackDuration[atkType] / _FPS;

			if (animPlayingTime >= atkDuration)
			{
				atkType = rand() % 3; // ���� ���� Ÿ�� ����
				ResetToIdleState();
			}
			return;
		}

		// Aggro �ִϸ��̼��� �Ϸ�Ǿ����� Ȯ��
		if (_currentAnimationState == AnimationState::Aggro)
		{
			if (animPlayingTime >= _aggroDuration / _FPS)
			{
				isFirstAggro = false;
				ResetToIdleState();
			}
			return;
		}
	}

	// �÷��̾� ��ġ ���4
	_player = SCENE->GetCurrentScene()->GetPlayer();
	Vec3 playerPosition = _player->GetTransform()->GetPosition();

	direction = playerPosition - _transform->GetPosition();
	distance = direction.Length();
	rangeDis = (_transform->GetPosition() - StartPos).Length();

	// ���� �˻�
	if (rangeDis > 30.f) // �ʱ� ��ġ���� �ʹ� �ָ� �������� ����
	{
		BackToStart = true;
		onTarget = false;
		onAttack = false;
		isFirstAggro = true;
	}
	else if (distance < 15.f) { onTarget = true; } // Ž�� ���� �ȿ� ���� ��
	else
	{
		onTarget = false;
	}

	if (distance < 5.f) { onAttack = true; } // ���� ���� �ȿ� ���� ��
	else { onAttack = false; }


	// ���º� �ִϸ��̼� ����
	if (isFirstAggro && onTarget)
	{
		Aggro();
	}
	else if (onAttack)
	{
		Attack(atkType);
	}
	else if (BackToStart)
	{
		SetAnimationState(AnimationState::Run);
		Move(StartPos);
		Rota(StartPos);
		if (abs(rangeDis - 1.f) < EPSILON)
		{
			BackToStart = false;
		}
	}
	else if (onTarget)
	{
		SetAnimationState(AnimationState::Run);
		Move(direction);
		Rota(playerPosition);
	}
	
	else
	{
		SetAnimationState(AnimationState::Idle);
	}

}


void RangoonScript::SetAnimationState(AnimationState state)
{
	_modelAnimator->ChangeAnimation(state);
	_currentAnimationState = state;
}

void RangoonScript::ResetToIdleState() {
	_isAnimating = false;
	animPlayingTime = 0.0f;
	RangoonEndCoroutine();
	SetAnimationState(AnimationState::Idle);
}