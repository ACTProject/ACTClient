#include "pch.h"
#include "PlayerController.h"
#include "Model.h"
#include "Camera.h"
#include "ModelAnimator.h"
#include "MyCoroutine.h"
#include <coroutine>
#include "HitBox.h"
#include "BaseCollider.h"
#include "Rigidbody.h"
#include "MelleMonsterController.h"
#include "ShootingMonsterController.h"
#include "FinalBossMonsterFirstPhaseController.h"
#include "FinalBossMonsterSecondPhaseController.h"
#include "Material.h"
#include "Particle.h"

// Coroutine
std::coroutine_handle<MyCoroutine::promise_type> currentCoroutine;

void EndAttackCoroutine() {
	if (currentCoroutine) {
		currentCoroutine.destroy();
		currentCoroutine = nullptr;
	}
}

// 플레이어공격 코루틴 함수 정의
MyCoroutine PlayAttackCoroutine(PlayerController* playerScript, float animationDuration)
{
	co_await AwaitableSleep(chrono::milliseconds(static_cast<int>(animationDuration * 1000)));
	EndAttackCoroutine();
}

void PlayerController::Start()
{
    Super::Start();

    // 플레이어 스탯 초기화
    _maxHp = 400.f;
    _hp = 150.0f;
    _atk = 20.0f;

    for (int i = 0; i < 4; ++i) {
        _attackDurations[i] = _player->GetAnimationDuration(static_cast<AnimationState>((int)AnimationState::Attack1 + i));
    }

    std::cout << "PlayerController Start()" << std::endl;
}

void PlayerController::Update()
{
    Super::Update();

    if (DEBUG->IsDebugEnabled())
        return;

    _FPS = static_cast<float>(TIME->GetFps());
    _transform = GetTransform();
    _rigidbody = GetGameObject()->GetRigidbody();

    // 입력 처리
    HandleInput();

    // 이동 처리
    HandleMovement();

    // 애니메이션 처리
    HandleAnimations();

    // 공격 처리
    HandleAttack();

    // 회피 처리
    HandleDodge();

    // 점프 처리
    HandleJump();

    // 상호작용 처리
    HandleInteraction();

    HandlePortal();
}

void PlayerController::HandleInput()
{
    _moveDir = Vec3(0.0f);

    Vec3 cameraRight = CUR_SCENE->GetMainCamera()->GetCamera()->GetRight();
    Vec3 cameraForward = CUR_SCENE->GetMainCamera()->GetCamera()->GetForward();

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

    // 점프 입력 처리
    if (INPUT->GetButton(KEY_TYPE::SPACE))
        Jump();

    // 회피 입력 처리
    if (INPUT->GetButtonDown(KEY_TYPE::CTRL))
        StartDodge();

    // 공격 입력 처리
    if (INPUT->GetButtonDown(KEY_TYPE::LBUTTON))
    {
        if (_attackStage > 0)
            _currentDuration = _attackDurations[_attackStage - 1] / _FPS;

        _isPlayeringAttackAnimation = true;
        if (!_isAttacking)
            StartAttack();
        else if (_attackTimer >= (_currentDuration / 2.5f) && _attackTimer <= _currentDuration)
            ContinueAttack();
    }

    if (_isShellEquipped == true && INPUT->GetButton(KEY_TYPE::RBUTTON))
        _isBlocking = true;
    else
        _isBlocking = false;
}

void PlayerController::HandleMovement()
{
    if (_isAttacking)
        return;

    if (_moveDir.LengthSquared() > 0.0f)
    {
        _moveDir.Normalize();

        float dt = TIME->GetDeltaTime();
        float speed = INPUT->GetButton(KEY_TYPE::SHIFT) ? _speed * 2 : _speed;
        if (_isBlocking)
            speed = _crawlSpeed;

        Vec3 oldPosition = _transform->GetPosition();
        Vec3 newPosition = oldPosition + _moveDir * speed * dt;

        Ray ray(oldPosition, _moveDir);

        vector<shared_ptr<BaseCollider>> nearbyColliders = OCTREE->QueryColliders(ray);
        for (const auto& collider : nearbyColliders)
        {
            if (collider->GetGameObject()->GetRigidbody() != nullptr)
                continue;
            if (collider->GetGameObject()->GetHitBox() != nullptr)
                continue;

            float distance = 0.0f;
            if (collider->Intersects(ray, distance) && distance <= speed * dt)
                return; // 충돌 시 이동 취소
        }
        _transform->SetPosition(newPosition);

        // 방향에 따라 회전
        Vec3 targetForward = _moveDir;
        Vec3 currentForward = _transform->GetLook();
        float dot = std::clamp(currentForward.Dot(targetForward), -1.0f, 1.0f);
        float angle = std::acos(dot);

        if (angle != 0.0f)
        {
            Vec3 rotationAxis = currentForward.Cross(targetForward);
            rotationAxis.Normalize();

            if (rotationAxis.y < 0)
                angle = -angle;

            _transform->SetRotation(_transform->GetLocalRotation() + Vec3(0, angle, 0));
        }
        _dustTimer += TIME->GetDeltaTime();
        if (_dustTimer >= _dustInterval) {
            CreateDustEffect();
            _dustTimer = 0.0f; // 타이머 초기화
    }
}

void PlayerController::HandleAnimations()
{
    AnimationState targetState = (_moveDir.LengthSquared() > 0.0f) ?
        (INPUT->GetButton(KEY_TYPE::SHIFT) ? AnimationState::Run : AnimationState::Walk) : AnimationState::Idle;

    if (_isBlocking)
        targetState = (_moveDir.LengthSquared() > 0.0f) ? AnimationState::BlockingCrawl : AnimationState::BlockingIdle;

    if (!_isPlayeringAttackAnimation && !_isPlayeringDodgeAnimation && !_isPlayeringJumpAnimation)
    {
        if (_currentAnimationState != targetState)
            SetAnimationState(targetState);
    }
}

void PlayerController::HandleAttack()
{
    if (!_isAttacking)
    {
        _isHit = false;

        if (_hitbox)
            _hitbox->GetCollider()->SetActive(false);
        return;
    }

    float dt = TIME->GetDeltaTime();
    _attackTimer += dt;

    UpdateHitBox();

    if (_attackTimer >= 0.2f && _attackTimer <= 0.4f)
    {
        Vec3 forward = _transform->GetLook();
        forward.Normalize();
        float moveStep = std::min(_attackMoveSpeed * dt, _attackMoveDistance);
        _transform->SetPosition(_transform->GetPosition() + forward * moveStep);
        _attackMoveDistance -= moveStep;
    }

    if (_attackTimer >= (_attackDurations[_attackStage - 1] / _FPS))
    {
        _isAttacking = false;
        _attackStage = 0;
        _attackMoveDistance = 1.0f;
        ResetToIdleState();
    }
}

void PlayerController::HandleDodge()
{
    if (_isDodging)
        UpdateDodge();
}

void PlayerController::HandleJump()
{
    if (_isJumping && _rigidbody->GetIsGrounded())
    {
        _isJumping = false;
        _isPlayeringJumpAnimation = false;
    }
}

void PlayerController::SetAnimationState(AnimationState state)
{
	_modelAnimator->ChangeAnimation(state);
	_currentAnimationState = state;
}

void PlayerController::HandleInteraction()
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
void PlayerController::HandlePortal()
{
    auto playerCollider = GetGameObject()->GetCollider();
    // 옥트리에서 충돌 가능한 객체 가져오기
    vector<shared_ptr<BaseCollider>> nearbyColliders = OCTREE->QueryColliders(playerCollider);

    for (const auto& collider : nearbyColliders)
    {
        if (collider->GetGameObject()->GetObjectType() != ObjectType::Portal)
            return;

        if (collider->Intersects(playerCollider))
        {
            GAME->ChangeScene(2);
            break;
        }

    }
}
void PlayerController::InteractWithShell(shared_ptr<GameObject> gameObject)
{
    ModelMesh& shellModel = *gameObject->GetModelRenderer()->GetModel()->GetMeshes()[0];
    _player->AddDummyBoneAndAttach(shellModel, L"Shell", L"ShellDummy");

    // Shell 오브젝트 비활성화
    gameObject->SetActive(false);

    // 플레이어의 상태를 Shell 장착 상태로 변경
    _isShellEquipped = true;
}

void PlayerController::StartAttack()
{
	_isAttacking = true;
    _isHit = false;
	_attackStage = 1;
	_attackTimer = 0.0f;

	float duration = _attackDurations[_attackStage - 1] / _FPS;

	// 1타 공격 애니메이션 재생
	PlayAttackAnimation(_attackStage);
	MyCoroutine attackCoroutine = PlayAttackCoroutine(this, duration);
	currentCoroutine = attackCoroutine.GetHandler();
	currentCoroutine.resume();
}

void PlayerController::ContinueAttack()
{
	if (_attackStage < 4) {
		_attackStage++;
		_attackTimer = 0.0f;
		EndAttackCoroutine();

		float duration = _attackDurations[_attackStage - 1] / _FPS;
        _attackMoveDistance = 1.0f;
        _isHit = false;

		// 다음 공격 애니메이션 재생
		PlayAttackAnimation(_attackStage);
		MyCoroutine attackCoroutine = PlayAttackCoroutine(this, duration);
		currentCoroutine = attackCoroutine.GetHandler();
		currentCoroutine.resume();
	}
}

void PlayerController::PlayAttackAnimation(int stage)
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
void PlayerController::UpdateHitBox()
{
    if (!_hitbox || _isHit)
        return;

    auto hitboxCollider = _hitbox->GetCollider();
    hitboxCollider->SetActive(true);

    // 히트박스 위치 갱신
    _hitbox->GetTransform()->SetPosition(
        _transform->GetPosition() + _hitbox->GetHitBox()->GetOffSet() + _transform->GetLook() * 2.0f
    );

    vector<shared_ptr<BaseCollider>> nearbyColliders = OCTREE->QueryColliders(hitboxCollider);

    for (const auto& collider : nearbyColliders)
    {
        ObjectType type = collider->GetGameObject()->GetObjectType();
        if (type != ObjectType::Monster)
            continue;

        if (hitboxCollider->Intersects(collider))
        {
            auto controller = collider->GetGameObject()->GetController();
            if (!controller)
                continue;

            switch (controller->GetMonoBehaviourType())
            {
            case MonoBehaviourType::MelleMonster:
            {
                auto melleMonster = dynamic_pointer_cast<MelleMonsterController>(controller);
                if (melleMonster)
                    melleMonster->OnDamage(GetGameObject(), _atk);
                    melleMonster->PlayingHitMotion = true;
                break;
            }
            case MonoBehaviourType::ShootingMonster:
            {
                auto shootingMonster = dynamic_pointer_cast<ShootingMonsterController>(controller);
                if (shootingMonster)
                    shootingMonster->OnDamage(GetGameObject(), _atk);
                    shootingMonster->PlayingHitMotion = true;
                break;
            }
            case MonoBehaviourType::FinalBossMonster_1:
            {
                auto FinalBossMonster = dynamic_pointer_cast<FinalBossMonsterFirstPhaseController>(controller);
                if (FinalBossMonster)
                {
                    FinalBossMonster->OnDamage(GetGameObject(), _atk);
                    FinalBossMonster->PlayingHitMotion = true;
                }
                break;
            }
            case MonoBehaviourType::FinalBossMonster_2:
            {
                auto FinalBossMonster = dynamic_pointer_cast<FinalBossMonsterSecondPhaseController>(controller);
                if (FinalBossMonster)
                {
                    FinalBossMonster->OnDamage(GetGameObject(), _atk);
                    FinalBossMonster->PlayingHitMotion = true;
                }
                break;
            }
            default:
                break;
            }
            _isHit = true;
        }
    }
}

void PlayerController::StartDodge()
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

void PlayerController::UpdateDodge()
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

void PlayerController::Jump()
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
void PlayerController::ResetToIdleState() {
	_isPlayeringAttackAnimation = false;
	EndAttackCoroutine();
	SetAnimationState(AnimationState::Idle);
}

void PlayerController::SetDust(shared_ptr<Material> dust)
{
    _dustMaterial = dust;
    RESOURCES->Add(L"Dust", dust);
}
void PlayerController::CreateDustEffect()
{
    auto dustObject = make_shared<GameObject>();
    dustObject->GetOrAddTransform()->SetLocalPosition(Vec3(0, 0, 0));
    dustObject->AddComponent(make_shared<Particle>());

    Vec3 dustPosition = _transform->GetPosition();

    dustObject->GetParticle()->SetMaterial(_dustMaterial);
    dustObject->GetParticle()->Add(dustPosition, Vec2(4.0f, 4.0f));
    CUR_SCENE->Add(dustObject);
}

void PlayerController::OnDeath()
{
    std::cout << "Player has died! Game Over!" << std::endl;
}