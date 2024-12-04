﻿#include "pch.h"
#include "RangoonScript.h"
#include "MyCoroutine.h"
#include <coroutine>

#define AggroRange 30.0f
#define AttackRange 5.0f

constexpr float EPSILON = 0.01f;
// Coroutine
std::coroutine_handle<MyCoroutine::promise_type> currentRangoonCoroutine;

void RangoonEndCoroutine() {
	if (currentRangoonCoroutine) {
		currentRangoonCoroutine.destroy();
		currentRangoonCoroutine = nullptr;
	}
}

// 플레이어공격 코루틴 함수 정의
MyCoroutine RangoonCoroutine(RangoonScript* rangoonScript, float animationDuration)
{
	// 애니메이션 재생 시간 대기
	co_await AwaitableSleep(chrono::milliseconds(static_cast<int>(animationDuration * 1000)));		// 공격 전 `atkType`을 랜덤으로 설정
	RangoonEndCoroutine();
}

void RangoonScript::Move(Vec3 objPos, Vec3 targetPos, float speed)
{
	Vec3 direction = targetPos - objPos;
	if (direction.LengthSquared() < EPSILON) // EPSILON 사용
	{
		return;
	}

	direction.Normalize();  // 방향 벡터를 단위 벡터로 정규화

	_transform->SetPosition(_transform->GetPosition() + direction * speed * dt);  // 일정 거리만큼 이동
}

void RangoonScript::Rota(Vec3 objPos, Vec3 targetPos)
{
	CurForward = _transform->GetLook();
	Vec3 direction = targetPos - objPos;
	direction.Normalize();

	// 외적을 이용한 회전 축 계산
	Vec3 rotationAxis = CurForward.Cross(direction);

	// 외적 결과가 매우 작으면 방향 차이가 거의 없으므로 회전 필요 없음
	if (rotationAxis.LengthSquared() < EPSILON)
	{
		return;
	}

	rotationAxis.Normalize();

	// 각도 계산
	float angle = std::acos(CurForward.Dot(direction));

	// 작은 각도는 무시
	if (abs(angle) < EPSILON) // 0.01 라디안(약 0.57도) 이하 회전 무시
	{
		return;
	}
	// 방향에 따라 각도 조정 (y축 중심 회전)
	if (rotationAxis.y < 0) {
		angle = -angle;  // 왼쪽 회전
	}

	// 현재 회전값 업데이트
	Vec3 currentRotation = _transform->GetLocalRotation();
	Vec3 newRotation = Vec3::Lerp(currentRotation, currentRotation + Vec3(0, angle, 0), 0.1f); // 0.1f는 보간 속도
	_transform->SetRotation(newRotation);

}

void RangoonScript::Tracking(Vec3 pos, const std::vector<Node3D>& path)
{
	if (path.empty()) {
		return;
	}

	// 경로 상의 각 노드를 따라 이동
	for (size_t i = 0; i < path.size(); ++i) {
		// 현재 위치가 목표 노드에 도달했다면 다음 노드로 이동
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

	// 코루틴 실행
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

void RangoonScript::Patrol()
{
	static float lastPatrolTime = 0.0f; // 마지막 목표 생성 시간
	float currentTime = TIME->GetGameTime(); // 현재 게임 시간

	// 일정 시간이 지나지 않았다면 목표 생성 중단
	if (currentTime - lastPatrolTime > 1.f) // 3~6초 간격
	{// 새로운 랜덤 목표 지점 생성
		hasPatrolTarget = true;		
	}
	

	if (hasPatrolTarget)
	{
		SetAnimationState(AnimationState::Run);
		// 기존 목표 지점으로 계속 이동
		Move(RangoonPos, patrolTarget, 10.f);
		Rota(RangoonPos, patrolTarget);

		// 목표 지점에 도달했는지 확인
		if ((patrolTarget - _transform->GetPosition()).LengthSquared() < 1.f)
		{
			lastPatrolTime = currentTime; // 타이머 갱신
			hasPatrolTarget = false;
		}
	}
	else
	{
		float radius = 5.f; // 배회 반경
		float randomX = StartPos.x + (rand() % 2000 / 1000.0f - 1.0f) * radius;
		float randomZ = StartPos.z + (rand() % 2000 / 1000.0f - 1.0f) * radius;
		patrolTarget = Vec3(randomX, StartPos.y, randomZ);
		SetAnimationState(AnimationState::Idle);
	}

	// 목표 지점으로 이동
	//Move(patrolTarget);
	//Rota(patrolTarget);
}

void RangoonScript::Start()
{
	_transform = GetTransform();
	StartPos = _transform->GetPosition();
	patrolTarget = StartPos;
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
	dt = TIME->GetDeltaTime();
	// 플레이어 위치 계산4
	_player = SCENE->GetCurrentScene()->GetPlayer();
	PlayerPos = _player->GetTransform()->GetPosition();
	RangoonPos = _transform->GetPosition();

	if (_isAnimating)
	{
		animPlayingTime += dt;
		Rota(RangoonPos, PlayerPos);

		if (_currentAnimationState == AnimationState::Attack1 ||
			_currentAnimationState == AnimationState::Attack2 ||
			_currentAnimationState == AnimationState::Attack3)
		{
			float atkDuration = _attackDuration[atkType] / _FPS;

			if (animPlayingTime >= atkDuration)
			{
				atkType = rand() % 3; // 다음 공격 타입 결정
				ResetToIdleState();
			}
			return;
		}

		// Aggro 애니메이션이 완료되었는지 확인
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

	Vec3 RangoonToPlayerdir = PlayerPos - RangoonPos;
	float RangoonToPlayerdistance = RangoonToPlayerdir.Length();
	rangeDis = (RangoonPos - StartPos).Length();

	// 범위 검사
	if (rangeDis > 50.f) // 초기 위치에서 너무 멀리 떨어지면 복귀
	{
		BackToStart = true;
		onTarget = false;
		onAttack = false;
		isFirstAggro = true;
	}
	else if (RangoonToPlayerdistance <= AggroRange) 
	{ 
		onTarget = true;
	} // 탐지 범위 안에 있을 때
	else
	{
		onTarget = false;
	}

	if (RangoonToPlayerdistance < AttackRange) { onAttack = true; } // 공격 범위 안에 있을 때
	else { onAttack = false; }
	//

	// 상태별 애니메이션 실행
	if (BackToStart)
	{
		SetAnimationState(AnimationState::Run);
		Move(RangoonPos, StartPos, _speed);
		Rota(RangoonPos, StartPos);
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
		Move(RangoonPos, PlayerPos, _speed);
		Rota(RangoonPos, PlayerPos);
	}
	else
	{
		Patrol();
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
