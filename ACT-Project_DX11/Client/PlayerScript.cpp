#include "pch.h"
#include "PlayerScript.h"
#include "Model.h"
#include "Camera.h"
#include "ModelAnimator.h"
#include "MyCoroutine.h"
#include <coroutine>
#include "HitBox.h"
#include "BaseCollider.h"
#include "Particle.h"
#include "Material.h"

// Coroutine
std::coroutine_handle<MyCoroutine::promise_type> currentCoroutine;

void EndAttackCoroutine() {
	if (currentCoroutine) {
		currentCoroutine.destroy();
		currentCoroutine = nullptr;
	}
}

// 플레이어공격 코루틴 함수 정의
MyCoroutine PlayAttackCoroutine(PlayerScript* playerScript, float animationDuration)
{
	co_await AwaitableSleep(chrono::milliseconds(static_cast<int>(animationDuration * 1000)));
	EndAttackCoroutine();
}

void PlayerScript::Start()
{

}

void PlayerScript::Update()
{
	if (DEBUG->IsDebugEnabled())
		return;


	_FPS = static_cast<float>(TIME->GetFps());
	float dt = TIME->GetDeltaTime();
	for (int i = 0; i < 4; ++i) {
		_attackDurations[i] = _player->GetAnimationDuration(static_cast<AnimationState>((int)AnimationState::Attack1 + i));
	}
	_transform = GetTransform();

	Vec3 moveDir = Vec3(0.0f);
	bool isRunning = INPUT->GetButton(KEY_TYPE::SHIFT);  // Shift 키로 달리기 모드 여부 확인

	// 이동 입력 처리
	if (INPUT->GetButton(KEY_TYPE::W))
		moveDir += Vec3(0.0f, 0.0f, 1.0f);
	if (INPUT->GetButton(KEY_TYPE::S))
		moveDir += Vec3(0.0f, 0.0f, -1.0f);
	if (INPUT->GetButton(KEY_TYPE::A))
		moveDir += Vec3(-1.0f, 0.0f, 0.0f);
	if (INPUT->GetButton(KEY_TYPE::D))
		moveDir += Vec3(1.0f, 0.0f, 0.0f);
	// 공격 입력 처리
	if (INPUT->GetButtonDown(KEY_TYPE::LBUTTON)) {
		if (_attackStage > 0)
			_currentDuration = _attackDurations[_attackStage - 1] / _FPS;	// 현재 애니메이션 지속 시간

		_isPlayeringAttackAnimation = true;
		if (!_isAttacking) {
			StartAttack();
		}
		else if (_attackTimer >= (_currentDuration / 2.5f) && _attackTimer <= _currentDuration)
		{
			ContinueAttack();
		}
	}

	// 공격 타이머 갱신
	if (_isAttacking)
	{
		// 히트박스 활성화
		_hitbox->GetCollider()->SetActive(true);

		_attackTimer += dt;

		// 공격 단계 시간 초과 시 Idle로 복귀
		if (_attackTimer >= (_attackDurations[_attackStage - 1] / _FPS)) {
			_attackStage = 0;
			_isAttacking = false;
			ResetToIdleState();
		}
	}
	else
	{
		// 히트박스 비활성화
		_hitbox->GetCollider()->SetActive(false);
	}
	// 공격 애니메이션이 재생 중이면 다른 애니메이션 상태로 전환되지 않음
	if (_isPlayeringAttackAnimation)
		return;

	// 이동 방향의 크기를 기준으로 애니메이션 상태 결정
	AnimationState targetAnimationState;

	if (moveDir.LengthSquared() > 0.0f)  // 이동 벡터가 0이 아니라면 이동 중으로 간주
	{
		moveDir.Normalize();
		float speed = isRunning ? _speed * 2 : _speed;
		_transform->SetPosition(_transform->GetPosition() + moveDir * speed * dt);

		targetAnimationState = isRunning ? AnimationState::Run : AnimationState::Walk;


		// 이동 방향에 따라 회전 설정
		Vec3 targetForward = moveDir;					// 캐릭터가 이동하려는 방향
		Vec3 currentForward = _transform->GetLook();	// 캐릭터가 현재 바라보는 방향

		// 두 벡터 사이의 각도를 계산하여 회전
		float angle = std::acos(currentForward.Dot(targetForward));	// 두 벡터 사이의 각도
		if (angle != 0.f)
		{
			Vec3 rotationAxis = currentForward.Cross(targetForward);	// 두 벡터가 이루는 평면의 법선벡터
			rotationAxis.Normalize();

			// 회전 축의 y 값으로 좌우 방향을 구분
			if (rotationAxis.y < 0) {
				angle = -angle;  // 왼쪽으로 회전
			}
			_transform->SetRotation(_transform->GetRotation() + Vec3(0, angle, 0));

		}

		// HitBox
		{
			_hitbox->GetTransform()->SetPosition(_transform->GetPosition() + _hitbox->GetHitBox()->GetOffSet() + _transform->GetLook() * 1.8f);
		}

        _dustTimer += TIME->GetDeltaTime();
        if (_dustTimer >= _dustInterval) {
            CreateDustEffect();
            _dustTimer = 0.0f; // 타이머 초기화
        }
	}
	else
	{
		targetAnimationState = AnimationState::Idle;
        _dustTimer = 0.0f;
	}

	// 애니메이션 상태가 변경되었을 때만 상태 전환
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

	float duration = _attackDurations[_attackStage - 1] / _FPS;

	// 1타 공격 애니메이션 재생
	PlayAttackAnimation(_attackStage);
	MyCoroutine attackCoroutine = PlayAttackCoroutine(this, duration);
	currentCoroutine = attackCoroutine.GetHandler();
	currentCoroutine.resume();
}

void PlayerScript::ContinueAttack()
{
	if (_attackStage < 4) {
		_attackStage++;
		_attackTimer = 0.0f;
		EndAttackCoroutine();

		float duration = _attackDurations[_attackStage - 1] / _FPS;

		// 다음 공격 애니메이션 재생
		PlayAttackAnimation(_attackStage);
		MyCoroutine attackCoroutine = PlayAttackCoroutine(this, duration);
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

void PlayerScript::SetDust(shared_ptr<Material> dustMaterial)
{
    RESOURCES->Add(L"Dust", dustMaterial);
}

void PlayerScript::CreateDustEffect()
{
    /*auto obj = make_shared<GameObject>();
    obj->AddComponent(make_shared<Particle>());

    Vec3 dustPosition = _transform->GetPosition();
    obj->GetParticle()->Create(dustPosition, Vec2(2.0f, 2.0f), RESOURCES->Get<Material>(L"Dust"));

    CUR_SCENE->Add(obj);*/
}
