#include "pch.h"
#include "PlayerScript.h"
#include "Model.h"
#include "Camera.h"
#include "ModelAnimator.h"
#include "MyCoroutine.h"
#include <coroutine>
#include "HitBox.h"
#include "BaseCollider.h"
#include "Rigidbody.h"

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

    _rigidbody = GetGameObject()->GetRigidbody();
	_FPS = static_cast<float>(TIME->GetFps());
	float dt = TIME->GetDeltaTime();
	for (int i = 0; i < 4; ++i) {
		_attackDurations[i] = _player->GetAnimationDuration(static_cast<AnimationState>((int)AnimationState::Attack1 + i));
	}
	_transform = GetTransform();

    Vec3 oldPosition = _transform->GetPosition();

	bool isRunning = INPUT->GetButton(KEY_TYPE::SHIFT);  // Shift 키로 달리기 모드 여부 확인

    _moveDir = Vec3(0.0f);

    // 카메라의 forward 및 right 벡터 가져오기
    Vec3 cameraForward = CUR_SCENE->GetMainCamera()->GetCamera()->GetForward(); // 카메라가 바라보는 방향
    Vec3 cameraRight = CUR_SCENE->GetMainCamera()->GetCamera()->GetRight(); // 카메라의 오른쪽 방향

    // 수직 축(y) 제거 (2D 평면에서 이동하도록)
    cameraForward.y = 0.0f;
    cameraRight.y = 0.0f;

    // 이동 입력 처리
    if (INPUT->GetButton(KEY_TYPE::W))
        _moveDir += cameraForward;
    if (INPUT->GetButton(KEY_TYPE::S))
        _moveDir -= cameraForward;
    if (INPUT->GetButton(KEY_TYPE::A))
        _moveDir += cameraRight;
    if (INPUT->GetButton(KEY_TYPE::D))
        _moveDir -= cameraRight;
    if (INPUT->GetButton(KEY_TYPE::SPACE))
        Jump();
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
    if (INPUT->GetButtonDown(KEY_TYPE::CTRL))
        StartDodge();
    
    if (_isDodging)
    {
        UpdateDodge();
    }

    // 점프 타이머 갱신
    if (_isJumping)
    {
        if (_rigidbody->GetIsGrounded())
        {
            _isJumping = false;
            _isPlayeringJumpAnimation = false;
        }
        else {
            SetAnimationState(AnimationState::Jump);
        }
    }

	// 공격 타이머 갱신
	if (_isAttacking)
	{
        _attackTimer += dt;

        if (_attackTimer >= 0.2f && _attackTimer <= 0.4f) // 애니메이션 중간에서 이동
        {
            Vec3 forward = _transform->GetLook(); // 캐릭터가 바라보는 방향
            forward.Normalize();

            float moveAmount = _attackMoveSpeed * dt;
            if (_attackMoveDistance > 0.0f)
            {
                float moveStep = std::min(moveAmount, _attackMoveDistance);
                Vec3 newPosition = _transform->GetPosition() + forward * moveStep;
                _transform->SetPosition(newPosition);

                _attackMoveDistance -= moveStep;
            }
        }

		// 히트박스 활성화
        auto hitboxCollider = _hitbox->GetCollider();
        hitboxCollider->SetActive(true);

        _hitbox->GetTransform()->SetPosition(_transform->GetPosition() + _hitbox->GetHitBox()->GetOffSet() + _transform->GetLook() * 2.0f);

        // 옥트리에서 충돌 가능한 객체 가져오기
        vector<shared_ptr<BaseCollider>> nearbyColliders = OCTREE->QueryColliders(hitboxCollider);

        for (const auto& collider : nearbyColliders)
        {
            ObjectType type = collider->GetGameObject()->GetObjectType();
            if (type != ObjectType::Monster)
                continue;
            
            if (hitboxCollider->Intersects(collider))
            {
                // TODO : 해당 몬스터 OnHit 상태 만들기
                collider->GetGameObject()->Destroy();
            }
        }

		// 공격 단계 시간 초과 시 Idle로 복귀
		if (_attackTimer >= (_attackDurations[_attackStage - 1] / _FPS)) {
			_attackStage = 0;
			_isAttacking = false;
            _attackMoveDistance = 1.0f;
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

    // Hitbox
    {
        _hitbox->GetTransform()->SetPosition(_transform->GetPosition() + _hitbox->GetHitBox()->GetOffSet() + _transform->GetLook() * 2.0f);
    }
	// 이동 방향의 크기를 기준으로 애니메이션 상태 결정
	AnimationState targetAnimationState;

    // Shell 찾기
    CheckInteraction();

    if (_isShellEquipped == true && INPUT->GetButton(KEY_TYPE::RBUTTON))
        _isBlocking = true;
    else
        _isBlocking = false;

    // Move
	if (_moveDir.LengthSquared() > 0.0f)  // 이동 벡터가 0이 아니라면 이동 중으로 간주
	{
        // 정규화 (속도 보정)
        if (_moveDir.LengthSquared() > 0.0f)
        {
            _moveDir.Normalize();
        }
        else
        {
            _moveDir = Vec3(0.0f); // 이동 방향 초기화
        }

		float speed = isRunning ? _speed * 2 : _speed;
        if (_isBlocking)
            speed = _crawlSpeed;

        Vec3 oldPosition = GetTransform()->GetPosition();
        Vec3 newPosition = oldPosition + _moveDir * speed * dt;

        bool canMove = true;

        Ray ray(oldPosition, _moveDir);

        // 옥트리에서 충돌 가능한 객체 가져오기
        vector<shared_ptr<BaseCollider>> nearbyColliders = OCTREE->QueryColliders(ray);
        
        for (const auto& collider : nearbyColliders)
        {
            if (collider->GetGameObject()->GetRigidbody() != nullptr)
                continue;

            float distance = 0.0f;
            if (collider->Intersects(ray, distance))
            {
                // Ray 길이와 충돌 거리를 비교하여 충돌 여부 확인
                if (distance <= speed * dt)
                {
                    // 충돌이 발생했으므로 이동 취소
                    canMove = false;
                }
            }
        }

        if (canMove)
		    _transform->SetPosition(newPosition);


        Vec3 changePosition = _transform->GetPosition();

		targetAnimationState = isRunning ? AnimationState::Run : AnimationState::Walk;
        if (_isBlocking)
            targetAnimationState = AnimationState::BlockingCrawl;

		// 이동 방향에 따라 회전 설정
		Vec3 targetForward = _moveDir;					// 캐릭터가 이동하려는 방향
		Vec3 currentForward = _transform->GetLook();	// 캐릭터가 현재 바라보는 방향

        // 두 벡터 사이의 각도를 계산하여 회전
        float dot = std::clamp(currentForward.Dot(targetForward), -1.0f, 1.0f); // 안전한 범위 클램프
        float angle = std::acos(dot);

		if (angle != 0.f)
		{
			Vec3 rotationAxis = currentForward.Cross(targetForward);	// 두 벡터가 이루는 평면의 법선벡터
			rotationAxis.Normalize();

			// 회전 축의 y 값으로 좌우 방향을 구분
			if (rotationAxis.y < 0) {
				angle = -angle;  // 왼쪽으로 회전
			}
			_transform->SetRotation(_transform->GetLocalRotation() + Vec3(0, angle, 0));

		}
	}
	else
	{
		targetAnimationState = AnimationState::Idle;
        if (_isBlocking)
            targetAnimationState = AnimationState::BlockingIdle;
	}

    // 회피 애니메이션이 재생 중이면 다른 애니메이션 상태로 전환되지 않음
    if (_isPlayeringDodgeAnimation)
        return;

    // 점프 애니메이션이 재생 중이면 다른 애니메이션 상태로 전환되지 않음
    if (_isPlayeringJumpAnimation)
        return;

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

void PlayerScript::CheckInteraction()
{
    auto playerCollider = GetGameObject()->GetCollider();
    // 옥트리에서 충돌 가능한 객체 가져오기
    vector<shared_ptr<BaseCollider>> nearbyColliders = OCTREE->QueryColliders(playerCollider);

    for (const auto& collider : nearbyColliders)
    {
        if (collider->GetGameObject()->GetObjectType() != ObjectType::Shell)
            return;

        if (collider->Intersects(playerCollider) && INPUT->GetButtonDown(KEY_TYPE::E))
        {
            InteractWithShell(collider->GetGameObject());
            break;
        }
        
    }
}
void PlayerScript::InteractWithShell(shared_ptr<GameObject> gameObject)
{
    ModelMesh& shellModel = *gameObject->GetModelRenderer()->GetModel()->GetMeshes()[0];
    _player->AddDummyBoneAndAttach(shellModel, L"Shell", L"ShellDummy");

    // Shell 오브젝트 비활성화
    gameObject->SetActive(false);

    // 플레이어의 상태를 Shell 장착 상태로 변경
    _isShellEquipped = true;
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
        _attackMoveDistance = 1.0f;

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

void PlayerScript::StartDodge()
{
    if (_isDodging)
        return;

    _isDodging = true;
    _dodgeTimer = 0.0f;
    _dodgeDuration = _player->GetAnimationDuration(static_cast<AnimationState>((int)AnimationState::Dodge)); // 회피 동작 시간
    _dodgeDuration /= _FPS;
    _dodgeDistance = 3.0f; // 회피 이동 거리
    
    // 회피 방향 설정
    _dodgeDirection = _moveDir;
    if (_dodgeDirection.LengthSquared() == 0.0f)
        _dodgeDirection = _transform->GetLook(); // 입력이 없으면 바라보는 방향으로 이동
    _dodgeDirection.Normalize();

    _isPlayeringDodgeAnimation = true;
    SetAnimationState(AnimationState::Dodge);

    // TODO (무적 상태)
    _isInvincible = true;
}

void PlayerScript::UpdateDodge()
{
    float dt = TIME->GetDeltaTime();

    _dodgeTimer += dt;

    // 회피 이동 처리
    float moveAmount = (_dodgeDistance / _dodgeDuration) * dt;
    Vec3 newPosition = _transform->GetPosition() + _dodgeDirection * moveAmount;
    _transform->SetPosition(newPosition);

    // 회피 종료 처리
    if (_dodgeTimer >= _dodgeDuration)
    {
        _isDodging = false;
        _isInvincible = false; // 무적 상태 해제
        _isPlayeringDodgeAnimation = false;
    }
}

void PlayerScript::Jump()
{
    auto rigidbody = _rigidbody;
    if (rigidbody->GetIsGrounded())
    {
        _isJumping = true;
        auto velocity = rigidbody->GetVelocity();
        velocity.y = _jumpSpeed;
        rigidbody->SetVelocity(velocity);
        rigidbody->SetIsGrounded(false);
        
        Vec3 position = _transform->GetPosition();
        position += velocity * SCENE->GetFixedDeltaTime();
        _transform->SetPosition(position);

        SetAnimationState(AnimationState::Jump);
        _isPlayeringJumpAnimation = true;
    }
}
void PlayerScript::ResetToIdleState() {
	_isPlayeringAttackAnimation = false;
	EndAttackCoroutine();
	SetAnimationState(AnimationState::Idle);
}
