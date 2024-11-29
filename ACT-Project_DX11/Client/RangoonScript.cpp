#include "pch.h"
#include "RangoonScript.h"
#include "MyCoroutine.h"
#include <coroutine>

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

void RangoonScript::SetNextType(int type)
{
	atkType = type;
}

void RangoonScript::Aggro()
{
	_isAnimating = true;

	SetAnimationState(AnimationState::Aggro);

	MyCoroutine aggroCoroutine = RangoonCoroutine(this, _aggroDuration / _FPS);
	currentRangoonCoroutine = aggroCoroutine.GetHandler();
	currentRangoonCoroutine.resume();
}

void RangoonScript::Move(const Vec3 targetPos)
{
	direction = targetPos - _transform->GetPosition();
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
	if (rotationAxis.LengthSquared() < 1e-6f)
	{
		return;
	}

	rotationAxis.Normalize();

	// ���� ���
	float angle = std::acos(CurForward.Dot(direction));

	// ���� ������ ����
	if (abs(angle) < 0.001f) // 0.01 ����(�� 0.57��) ���� ȸ�� ����
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
	UpdateTimers(); // Ÿ�̸� ������Ʈ

	UpdatePlayerInfo(); // ��ġ ������Ʈ

	CheckRanges(); // ���� Ȯ�� �� ���� ����

	if (_isAnimating) // �ִϸ��̼� ���� ���̸� �ش� ���� ó��
	{
		HandleAnimationState();
		return;
	}

	// ���º� ���� ó��
	if (BackToStart)
	{
		HandleBackToStart();
	}
	else if (onTarget && !onAttack)
	{
		HandleChasePlayer();
	}
	else if (onAttack)
	{
		HandleAttack();
	}
	else
	{
		SetAnimationState(AnimationState::Idle);
	}
}
void RangoonScript::UpdateTimers()
{
	_FPS = static_cast<float>(TIME->GetFps());
	_deltaTime = TIME->GetDeltaTime();
	if (_isAnimating)
	{
		animPlayingTime += _deltaTime; // �ִϸ��̼� ���� �ð� ����
	}
}
void RangoonScript::UpdatePlayerInfo()
{
	_player = SCENE->GetCurrentScene()->GetPlayer();
	Vec3 playerPosition = _player->GetTransform()->GetPosition();

	direction = playerPosition - _transform->GetPosition();
	distance = direction.Length();
	rangeDis = (_transform->GetPosition() - StartPos).Length();
}
void RangoonScript::CheckRanges()
{
	// �ʱ� ��ġ���� �ʹ� �ָ� �������� ����
	if (rangeDis > 15.f)
	{
		BackToStart = true;
		onTarget = false;
		onAttack = false;
	}
	else if(distance < 15.f && !BackToStart)
	{
		onTarget = true;
		if (distance < 5.f)
		{
			onAttack = true;
		}
		else
		{
			onAttack = false;
		}
	}
}
void RangoonScript::HandleAnimationState()
{
	if (!isAggro) // Aggro �ִϸ��̼� ���� ��
	{
		if (animPlayingTime > _aggroDuration)
		{
			isAggro = true; // Aggro ���� �Ϸ�
			ResetToIdleState();
		}
	}
	else if (onAttack) // ���� �ִϸ��̼� ���� ��
	{
		float atkDuration = _attackDuration[atkType] / _FPS;
		if (animPlayingTime > atkDuration)
		{
			atkType = rand() % 3; // ���� ���� Ÿ�� ����
			ResetToIdleState();
		}
	}
}
void RangoonScript::HandleBackToStart()
{
	SetAnimationState(AnimationState::Run);
	Move(StartPos);   // ���� ��ġ�� �̵�
	Rota(StartPos);   // ���� ��ġ�� ���ϵ��� ȸ��

	if (rangeDis < 1.f) // ���� ��ġ ��ó�� �����ϸ� ����
	{
		BackToStart = false;
	}
}
void RangoonScript::HandleChasePlayer()
{
	SetAnimationState(AnimationState::Run);
	Move(direction);  // �÷��̾ ���� �̵�
	Rota(_player->GetTransform()->GetPosition()); // �÷��̾ �ٶ󺸵��� ȸ��
}
void RangoonScript::HandleAttack()
{
	if (!_isAnimating) // �ִϸ��̼��� ���� ���� �ƴϸ�
	{
		Attack(atkType); // ���� ����
		Rota(_player->GetTransform()->GetPosition()); // �÷��̾ �ٶ󺸵��� ȸ��
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