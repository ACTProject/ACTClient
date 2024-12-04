#include "pch.h"
#include "ShootingMonster.h"

#define AggroRange 30.0f
#define AttackRange 5.0f

void ShootingMonster::Move(Vec3 objPos, Vec3 targetPos, float speed)
{
	Vec3 direction = targetPos - objPos;
	if (direction.LengthSquared() < EPSILON) // EPSILON ���
	{
		return;
	}

	direction.Normalize();  // ���� ���͸� ���� ���ͷ� ����ȭ

	_transform->SetPosition(_transform->GetPosition() + direction * speed * dt);  // ���� �Ÿ���ŭ �̵�
}

void ShootingMonster::Rota(Vec3 objPos, Vec3 targetPos)
{
	CurForward = _transform->GetLook();
	Vec3 direction = targetPos - objPos;
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

void ShootingMonster::Tracking(Vec3 pos, const std::vector<Node3D>& path)
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

void ShootingMonster::Attack(int type)
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
	MyCoroutine attackCoroutine = EnemyCoroutine(this, atkDuration);
	currentEnemyCoroutine = attackCoroutine.GetHandler();
	currentEnemyCoroutine.resume();

}

void ShootingMonster::Aggro()
{
	_isAnimating = true;

	float duration = _aggroDuration / _FPS;

	SetAnimationState(AnimationState::Aggro);
	MyCoroutine aggroCoroutine = EnemyCoroutine(this, duration);
	currentEnemyCoroutine = aggroCoroutine.GetHandler();
	currentEnemyCoroutine.resume();
}

void ShootingMonster::Patrol()
{
	static float lastPatrolTime = 0.0f; // ������ ��ǥ ���� �ð�
	float currentTime = TIME->GetGameTime(); // ���� ���� �ð�

	// ���� �ð��� ������ �ʾҴٸ� ��ǥ ���� �ߴ�
	if (currentTime - lastPatrolTime > 1.f) // 3~6�� ����
	{// ���ο� ���� ��ǥ ���� ����
		hasPatrolTarget = true;
	}


	if (hasPatrolTarget)
	{
		SetAnimationState(AnimationState::Run);
		// ���� ��ǥ �������� ��� �̵�
		Move(EnemyPos, patrolTarget, 10.f);
		Rota(EnemyPos, patrolTarget);

		// ��ǥ ������ �����ߴ��� Ȯ��
		if ((patrolTarget - _transform->GetPosition()).LengthSquared() < 1.f)
		{
			lastPatrolTime = currentTime; // Ÿ�̸� ����
			hasPatrolTarget = false;
		}
	}
	else
	{
		float radius = 5.f; // ��ȸ �ݰ�
		float randomX = StartPos.x + (rand() % 2000 / 1000.0f - 1.0f) * radius;
		float randomZ = StartPos.z + (rand() % 2000 / 1000.0f - 1.0f) * radius;
		patrolTarget = Vec3(randomX, StartPos.y, randomZ);
		SetAnimationState(AnimationState::Idle);
	}

	// ��ǥ �������� �̵�
	//Move(patrolTarget);
	//Rota(patrolTarget);
}

void ShootingMonster::Start()
{
	_transform = GetTransform();
	StartPos = _transform->GetPosition();
	patrolTarget = StartPos;
	for (int i = 0; i < 3; ++i)
	{
		_attackDuration[i] = _enemy->GetAnimationDuration(static_cast<AnimationState>((int)AnimationState::Attack1 + i));
	}
	_aggroDuration = _enemy->GetAnimationDuration(static_cast<AnimationState>((int)AnimationState::Aggro));
}

void ShootingMonster::Update()
{
	if (INPUT->GetButton(KEY_TYPE::KEY_4))
	{
		int a = 0;
	}

	_FPS = static_cast<float>(TIME->GetFps());
	dt = TIME->GetDeltaTime();
	// �÷��̾� ��ġ ���4
	_player = SCENE->GetCurrentScene()->GetPlayer();
	PlayerPos = _player->GetTransform()->GetPosition();
	EnemyPos = _transform->GetPosition();

	if (_isAnimating)
	{
		animPlayingTime += dt;
		Rota(EnemyPos, PlayerPos);

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

	Vec3 EnemyToPlayerdir = PlayerPos - EnemyPos;
	float EnemyToPlayerdistance = EnemyToPlayerdir.Length();
	rangeDis = (EnemyPos - StartPos).Length();

	// ���� �˻�
	if (rangeDis > 50.f) // �ʱ� ��ġ���� �ʹ� �ָ� �������� ����
	{
		BackToStart = true;
		onTarget = false;
		onAttack = false;
		isFirstAggro = true;
	}
	else if (EnemyToPlayerdistance <= AggroRange)
	{
		onTarget = true;
	} // Ž�� ���� �ȿ� ���� ��
	else
	{
		onTarget = false;
	}

	if (EnemyToPlayerdistance < AttackRange) { onAttack = true; } // ���� ���� �ȿ� ���� ��
	else { onAttack = false; }
	//

	// ���º� �ִϸ��̼� ����
	if (BackToStart)
	{
		SetAnimationState(AnimationState::Run);
		Move(EnemyPos, StartPos, _speed);
		Rota(EnemyPos, StartPos);
		if (abs(rangeDis) < 1.f)
		{
			BackToStart = false;
		}
	}
	else if (isFirstAggro && onTarget)
	{
		Aggro();
	}
	else if (onAttack)
	{
		Attack(atkType);
	}
	else if (onTarget)
	{
		SetAnimationState(AnimationState::Run);
		Move(EnemyPos, PlayerPos, _speed);
		Rota(EnemyPos, PlayerPos);
	}
	else
	{
		Patrol();
	}

}


void ShootingMonster::SetAnimationState(AnimationState state)
{
	_modelAnimator->ChangeAnimation(state);
	_currentAnimationState = state;
}

void ShootingMonster::ResetToIdleState() {
	_isAnimating = false;
	animPlayingTime = 0.0f;
	EnemyEndCoroutine();
	SetAnimationState(AnimationState::Idle);
}