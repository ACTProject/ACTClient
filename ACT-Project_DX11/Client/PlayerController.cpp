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
#include "MeshRenderer.h"
#include "DynamicObj.h"
#include "MathUtils.h"

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
    _hp = 400.0f;
    _atk = 20.0f;
    _shellMaxHp = 200.0f;
    _shellHp = 200.0f;

    for (int i = 0; i < 4; ++i) {
        _attackDurations[i] = _player->GetAnimationDuration(static_cast<AnimationState>((int)AnimationState::Attack1 + i));
    }

    std::cout << "PlayerController Start()" << std::endl;

    if (GAME->GetShellState())
    {
        //auto obj = GAME->GetShellObject();

        InteractWithShell(_shellObject);
    }

    for (auto& armor : _armorGroup) {
        armor->SetActive(_isShellEquipped);
    }

    // 로드 이벤트 등록.
    SAVE->AddLoadEvent(std::bind(&PlayerController::LoadPlayer,this,std::placeholders::_1));
}

void PlayerController::Update()
{
    if (CUR_SCENE->GetMainCamera()->GetCamera()->IsCutSceneActive() == true)
    {
        return;
    }

    Super::Update();

    if (DEBUG->IsDebugEnabled())
        return;

    SOUND->SetVolume(L"bgm", 0.3);
    _FPS = static_cast<float>(TIME->GetFps());
    _transform = GetTransform();
    _rigidbody = GetGameObject()->GetRigidbody();

    if (_isDead)
    {
        return;
    }

    if (startChoke)
    {
        _rigidbody->SetUseGravity(false);
        onChoked();
        return;
    }
    else
    {
        _rigidbody->SetUseGravity(true);
    }

    if (_playerActive == true)
    {
        // 입력 처리
        HandleInput();
    }

    // 이동 처리
    HandleMovement();

    // 애니메이션 처리
    HandleAnimations();

    // 공격 처리
    HandleAttack();

    // 차지 공격 처리
    HandleChargeAttack();

    // 대쉬 공격 처리
    HandleDashAttack();

    // 회피 처리
    HandleDodge();

    // 점프 처리
    HandleJump();

    // 히트 상태 처리
    HandleHit();
  
    // Shell 히트 상태 처리
    HandleShellHit();

    // 공중 공격 처리
    HandleAirAttack();

    // 상호작용 처리
    HandleInteraction();

    // 충돌 처리
    HandleCollision();

    // 중복 데미지 처리 방지
    if (!_isAttacking && !_isAirAttacking && !_isChargeAttacking && !_isDashAttacking)
        _isHit = false;
}

void PlayerController::HandleInput()
{
    _moveDir = Vec3(0.0f);

    Vec3 cameraRight = CUR_SCENE->GetMainCamera()->GetCamera()->GetRight();
    Vec3 cameraForward = CUR_SCENE->GetMainCamera()->GetCamera()->GetForward();

    // 수직 축(y) 제거 (2D 평면에서 이동하도록)
    cameraForward.y = 0.0f;
    cameraRight.y = 0.0f;

    // 차지 공격 처리
    if (INPUT->GetButton(KEY_TYPE::Q))
    {
        if (!_isCharging && !_isChargeAttacking)
        {
            _isCharging = true; // 차지 시작
            _chargeTimer = 0.0f; // 차지 시간 초기화
        }
        if (_isCharging)
        {
            // 차지 시간 누적
            _chargeTimer += TIME->GetDeltaTime();

            // 차지 시간이 임계값을 초과하면 차지 공격 발동
            if (_chargeTimer >= _chargeThreshold)
            {
                _isCharging = false;
                _chargeTimer = 0.0f;
                StartChargeAttack();
            }
        }
    }

    if (_isChargeAttacking)
        return;

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

    // 플레이어가 땅에 붙어있지 않으면 return 처리
    if (!GetGameObject()->GetRigidbody()->GetIsGrounded())  
        return;

    // 회피 입력 처리
    if (INPUT->GetButtonDown(KEY_TYPE::CTRL))
        StartDodge();

    // 공격 입력 처리
    if (INPUT->GetButtonDown(KEY_TYPE::LBUTTON))
    {
        if (_isRunning)
        {
            StartDashAttack();
            return;
        }
        // 기본 공격 Start
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
    if (_isAttacking || _isDashAttacking)
        return;

    if (_moveDir.LengthSquared() > 0.0f)
    {
        _moveDir.Normalize();

        float dt = TIME->GetDeltaTime();
        float speed = _speed;

        if (INPUT->GetButton(KEY_TYPE::SHIFT))
        {
            speed *= 2;
            _isRunning = true;
        }
        else
            _isRunning = false;

        if (_isJumping)
            speed = _speed;

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
            if (collider->GetGameObject()->GetObjectType() == ObjectType::Spoils)
                continue;

            float distance = 0.0f;
            if (collider->Intersects(ray, distance) && distance <= speed * dt)
                return; // 충돌 시 이동 취소
        }
        _transform->SetPosition(newPosition);

        if (_isBackStep)
            return;

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

        float _footstepInterval = (speed == _speed) ? _walkingInterval : _runningInterval;
        _footstepTimer += DT;
        if (_footstepTimer >= _footstepInterval)
        {
            SOUND->PlayEffect(L"player_footstep");
            _footstepTimer = 0.0f;
        }
    }
}

void PlayerController::HandleAnimations()
{
    AnimationState targetState = (_moveDir.LengthSquared() > 0.0f) ?
        (INPUT->GetButton(KEY_TYPE::SHIFT) ? AnimationState::Run : AnimationState::Walk) : AnimationState::Idle;

    if (_isBlocking)
        targetState = (_moveDir.LengthSquared() > 0.0f) ? AnimationState::BlockingCrawl : AnimationState::BlockingIdle;

    if (!_isPlayeringAttackAnimation    && 
        !_isPlayeringDodgeAnimation     && 
        !_isPlayeringJumpAnimation      && 
        !_isPlayeringHitAnimation       &&
        !_isPlayeringAirAttackAnimation &&
        !_isPlayeringDashAttackAnimation &&
        !_isPlayeringChargeAttackAnimation)
    {
        if (_currentAnimationState != targetState)
            SetAnimationState(targetState);
    }
}

void PlayerController::HandleAttack()
{
    if (!_isAttacking)
    {
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
void PlayerController::HandleAirAttack()
{
    if (_isAirAttacking)
        UpdateAirAttack();

    else
    {
        if (_airHitbox)
            _airHitbox->GetCollider()->SetActive(false);
        return;
    }

}

void PlayerController::HandleChargeAttack()
{
    if (_isChargeAttacking)
        UpdateChargeAttack();
    else
    {
        if (_chargeHitbox)
            _chargeHitbox->GetCollider()->SetActive(false);
        return;
    }
}

void PlayerController::HandleDashAttack()
{
    if (_isDashAttacking)
        UpdateDashAttack();
    else
    {
        if (_dashHitbox)
            _dashHitbox->GetCollider()->SetActive(false);
        return;
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

    if (!_rigidbody->GetIsGrounded())
    {
        if (INPUT->GetButtonDown(KEY_TYPE::LBUTTON))
        {
            StartAirAttack();
        }
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
        if (collider->Intersects(playerCollider) && INPUT->GetButtonDown(KEY_TYPE::E))
        {
            // Shell 상호작용
            if (collider->GetGameObject()->GetObjectType() == ObjectType::Shell)
            {
                InteractWithShell(collider->GetGameObject());
                break;
            }
            // 전리품 상호작용
            if (collider->GetGameObject()->GetObjectType() == ObjectType::Spoils)
            {
                _spoil++;

                if (_spoil <= 10)
                {
                    auto ui = UIMANAGER->GetUi("MissionUI");
                    wstring wstr = to_wstring(_spoil);
                    ui->GetGameObject()->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(wstr));

                    if (_spoil == 10)
                    {
                        auto camera = CUR_SCENE->GetMainCamera()->GetCamera();
                        Vec3 start(344.074f, 27.1922f, 309.091f);
                        Vec3 end(344.074f, 27.1922f, 309.091f);
                        Vec3 focus(0.643073f, -0.26635f, 0.717994f);
                        float duration = 5.0f; // 컷씬 진행 시간

                        camera->StartCutscene(start, end, focus, duration);
                        
                        SOUND->PlayEffect(L"openPortal");
                        CUR_SCENE->SetMissionClear(true);
                    }
                }

                SOUND->PlayEffect(L"player_pickupItem");
                OCTREE->RemoveCollider(collider);
                CUR_SCENE->Remove(collider->GetGameObject());
                TaskQueue::GetInstance().AddTask([collider]() {
                    std::cout << "Destroying object in TaskQueue..." << std::endl;
                    collider->GetGameObject()->Destroy();
                });
                std::cout << "spoil : " << _spoil << std::endl;
                break;
            }

            if (collider->GetGameObject()->GetDynamicObj() == nullptr)
                return;

            // 힐 상호작용
            if (collider->GetGameObject()->GetDynamicObj()->GetDynamicType() == DynamicType::Heal)
            {
                HealPlayer();
                
                // 힐 오브젝트를 비활성화
                collider->GetGameObject()->SetActive(false);
                OCTREE->RemoveCollider(collider);

                // 작업 큐에 활성화 작업 추가
                constexpr float respawnTime = 5.0f; // 리스폰 대기 시간 (초)
                TaskQueue::GetInstance().AddTask([collider]() {
                    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(respawnTime * 1000)));                    
                    collider->GetGameObject()->SetActive(true);
                    OCTREE->InsertCollider(collider);
                });
                break;
            }
            // 세이브 상호작용
            if (collider->GetGameObject()->GetDynamicObj()->GetDynamicType() == DynamicType::Save)
            {
                SAVE->OpenSaveUI();
                _playerActive = !_playerActive;
                break;
            }
        }
    }
}
void PlayerController::HandleCollision()
{
    auto playerCollider = GetGameObject()->GetCollider();
    // 옥트리에서 충돌 가능한 객체 가져오기
    vector<shared_ptr<BaseCollider>> nearbyColliders = OCTREE->QueryColliders(playerCollider);

    for (const auto& collider : nearbyColliders)
    {
        if (collider->GetGameObject()->GetDynamicObj() == nullptr)
            return;

        if (collider->Intersects(playerCollider))
        {
            // 트랩 충돌
            if (collider->GetGameObject()->GetDynamicObj()->GetDynamicType() == DynamicType::PitFall)
            {
                if (auto ui = UIMANAGER->GetUi("PlayerHP"))
                {
                    _hp -= 10;
                    _hp = std::clamp(_hp, 0.0f, _maxHp);

                    auto hpSlider = dynamic_pointer_cast<Slider>(ui);
                    float hpRatio = _hp / _maxHp;
                    hpSlider->SetRatio(hpRatio);

                    if (_hp <= 0.f)
                        OnDeath();
                    else
                        StartHit();

                    // 밀리는 로직
                    Vec3 TrapPosition = collider->GetGameObject()->GetTransform()->GetPosition(); // 가시의 위치
                    Vec3 playerPosition = _transform->GetPosition();                // 플레이어의 위치

                    Vec3 knockbackDirection = playerPosition - TrapPosition; // 가시 -> 플레이어 방향
                    knockbackDirection.Normalize();                              // 방향 벡터 정규화

                    float knockbackForce = 150.0f; // 밀리는 힘
                    _rigidbody->Addforce(knockbackDirection * knockbackForce); // 힘 적용
                }
            }

            if (collider->GetGameObject()->GetObjectType() != ObjectType::Portal)
                return;

            bool isClear = CUR_SCENE->GetMissionClear();
            if (isClear)
            {
                SOUND->PlayEffect(L"player_enterPortal");
                TaskQueue::GetInstance().Clear();
                GAME->ChangeScene(2);
                break;
            }
        }
    }
}

void PlayerController::HandleHit()
{
    if (_hit)
        UpdateHit();
}

void PlayerController::HandleShellHit()
{
    if (_shellHit)
        UpdateShellHit();
}

void PlayerController::InteractWithShell(shared_ptr<GameObject> gameObject)
{
    ModelMesh& shellModel = *gameObject->GetModelRenderer()->GetModel()->GetMeshes()[0];
    _player->AddDummyBoneAndAttach(shellModel, L"Shell", L"ShellDummy");
    _shellModel = make_shared<ModelMesh>(shellModel);

    SOUND->PlayEffect(L"player_shellConfirm");
    // Shell 오브젝트 비활성화
    gameObject->SetActive(false);

    // 플레이어의 상태를 Shell 장착 상태로 변경
    _isShellEquipped = true;
    for (auto& armor : _armorGroup) {
        armor->SetActive(true);
    }
}

void PlayerController::BreakShell()
{
    if (!_isShellEquipped)
    {
        std::cerr << "Error: Shell is not equipped. Cannot break it." << std::endl;
        return;
    }

    // 더미 본 및 연결된 쉘 메쉬 제거
    _player->RemoveDummyBoneAndDetach(_shellModel, L"ShellDummy");

    // 플레이어 상태 업데이트
    _isShellEquipped = false;

    for (auto& armor : _armorGroup) {
        armor->SetActive(false);
    }

    // 깨지는 소리 효과 재생
    SOUND->PlayEffect(L"player_shellBreak");

    // 애니메이션 상태 업데이트 (쉘 깨지는 애니메이션이 있을 경우)
    // TODO

    std::cout << "Shell has been broken and detached from the player." << std::endl;
}

void PlayerController::StartAttack()
{
	_isAttacking = true;
    _isHit = false;
	_attackStage = 1;
	_attackTimer = 0.0f;

	float duration = _attackDurations[_attackStage - 1] / _FPS;

    {
        SOUND->PlayEffect(L"player_atk1");
        SOUND->PlayEffect(L"player_atk1_md");
        //SOUND->SetVolume(L"player_atk1_md", 0.25f);
        SOUND->PlayEffect(L"player_atk1_sw");
    }
    SetAttackReaource();
    ActiveEffect(_effect);
    CreateBubbleEffect(15, Vec3(3.f, 1.f, 1.f), 1.f, 3.f);
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
        _effect->GetParticle()->SetMaterial(RESOURCES->Get<Material>(L"AttackEffect2"));

        {   // sound
            switch (_attackStage)
            {
                case 2:
                {
                    SOUND->PlayEffect(L"player_atk2");
                    SOUND->PlayEffect(L"player_atk2_md");
                    //SOUND->SetVolume(L"player_atk2_md", 0.25f);
                    SOUND->PlayEffect(L"player_atk2_sw");
                    break;
                }
                case 3:
                {
                    SOUND->PlayEffect(L"player_atk3");
                    SOUND->PlayEffect(L"player_atk3_md");
                    //SOUND->SetVolume(L"player_atk3_md", 0.25f);
                    SOUND->PlayEffect(L"player_atk3_sw");
                    break;
                }
                case 4:
                {
                    SOUND->PlayEffect(L"player_atk4");
                    SOUND->PlayEffect(L"player_atk4_md");
                    //SOUND->SetVolume(L"player_atk4_md", 0.25f);
                    SOUND->PlayEffect(L"player_atk4_sw");
                    break;
                }
                default:
                    break;
            }
        }
        SetAttackReaource();
        ActiveEffect(_effect);
        CreateBubbleEffect(15, Vec3(3.f, 1.f, 1.f), 1.f, 3.f);
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

    CheckAtk(hitboxCollider, _atk);
}

void PlayerController::UpdateAirHitBox()
{
    if (!_airHitbox || _isHit)
        return;

    auto hitboxCollider = _airHitbox->GetCollider();
    hitboxCollider->SetActive(true);

    // 히트박스 위치 갱신
    _airHitbox->GetTransform()->SetPosition(
        _transform->GetPosition() + _airHitbox->GetHitBox()->GetOffSet()
    );

    CheckAtk(hitboxCollider, _atk * 1.5f);
}

void PlayerController::UpdateChargeHitBox()
{
    if (!_chargeHitbox || _isHit)
        return;

    auto hitboxCollider = _chargeHitbox->GetCollider();
    hitboxCollider->SetActive(true);

    // 히트박스 위치 갱신
    _chargeHitbox->GetTransform()->SetPosition(
        _transform->GetPosition() + _chargeHitbox->GetHitBox()->GetOffSet() + _transform->GetLook() * 2.5f
    );

    CheckAtk(hitboxCollider, _atk * 2.5f);
}

void PlayerController::UpdateDashHitBox()
{
    if (!_dashHitbox || _isHit)
        return;

    auto hitboxCollider = _dashHitbox->GetCollider();
    hitboxCollider->SetActive(true);

    // 히트박스 위치 갱신
    _dashHitbox->GetTransform()->SetPosition(
        _transform->GetPosition() + _dashHitbox->GetHitBox()->GetOffSet() + _transform->GetLook() * 2.5f
    );

    CheckAtk(hitboxCollider, _atk * 1.5f);
}

void PlayerController::CheckAtk(shared_ptr<BaseCollider> hitboxCollider, float damage)
{
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
                {
                    melleMonster->OnDamage(GetGameObject(), damage);
                    ActiveEffect(_hitEffect);
                }

                melleMonster->PlayingHitMotion = true;
                break;
            }
            case MonoBehaviourType::ShootingMonster:
            {
                auto shootingMonster = dynamic_pointer_cast<ShootingMonsterController>(controller);
                if (shootingMonster)
                {
                    shootingMonster->OnDamage(GetGameObject(), damage);
                    ActiveEffect(_hitEffect);
                }

                shootingMonster->PlayingHitMotion = true;

                break;
            }
            case MonoBehaviourType::FinalBossMonster_1:
            {
                auto FinalBossMonster = dynamic_pointer_cast<FinalBossMonsterFirstPhaseController>(controller);
                if (FinalBossMonster)
                {
                    FinalBossMonster->OnDamage(GetGameObject(), damage);
                    FinalBossMonster->PlayingHitMotion = true;
                    ActiveEffect(_hitEffect);
                    //
                }
                break;
            }
            case MonoBehaviourType::FinalBossMonster_2:
            {
                auto FinalBossMonster = dynamic_pointer_cast<FinalBossMonsterSecondPhaseController>(controller);
                if (FinalBossMonster)
                {
                    FinalBossMonster->OnDamage(GetGameObject(), damage);
                    FinalBossMonster->PlayingHitMotion = true;
                    ActiveEffect(_hitEffect);
                    //
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
void PlayerController::SetAttackReaource()
{
    Vec3 playerLook = _transform->GetLook();
    Vec3 cameraForward = CUR_SCENE->GetMainCamera()->GetCamera()->GetForward();
    playerLook.Normalize();
    cameraForward.Normalize();
    float dot = playerLook.Dot(cameraForward);
    switch (_attackStage)
    {
    case 1:
    {
        if (dot > 0.0f)
        {
            _effect->GetParticle()->SetMaterial(RESOURCES->Get<Material>(L"AttackEffect"));
            _effect->GetParticle()->SetLeft(true);
        }
        else
        {
            _effect->GetParticle()->SetMaterial(RESOURCES->Get<Material>(L"AttackEffect2"));
            _effect->GetParticle()->SetLeft(false);
        }
        break;
    }
        
    case 2:
    {
        if (dot > 0.0f)
        {
            _effect->GetParticle()->SetMaterial(RESOURCES->Get<Material>(L"AttackEffect3"));
            _effect->GetParticle()->SetLeft(true);
        }
        else
        {
            _effect->GetParticle()->SetMaterial(RESOURCES->Get<Material>(L"AttackEffect4"));
            _effect->GetParticle()->SetLeft(false);
        }
        break;
    }
    case 3:
    {
        if (dot > 0.0f)
        {
            _effect->GetParticle()->SetMaterial(RESOURCES->Get<Material>(L"AttackEffect5"));
            _effect->GetParticle()->SetLeft(true);
        }
        else
        {
            _effect->GetParticle()->SetMaterial(RESOURCES->Get<Material>(L"AttackEffect6"));
            _effect->GetParticle()->SetLeft(false);
        }
        break;
    }
    case 4:
    {
        if (dot > 0.0f)
        {
            _effect->GetParticle()->SetMaterial(RESOURCES->Get<Material>(L"AttackEffect8"));
            _effect->GetParticle()->SetLeft(true);
        }
        else
        {
            _effect->GetParticle()->SetMaterial(RESOURCES->Get<Material>(L"AttackEffect7"));
            _effect->GetParticle()->SetLeft(false);
        }
        break;
    }
    default:
        break;
    }

    if (_isAirAttacking)
    {
        if (dot > 0.0f)
        {
            _effect->GetParticle()->SetMaterial(RESOURCES->Get<Material>(L"AttackEffect9"));
            _effect->GetParticle()->SetLeft(true);
        }
        else
        {
            _effect->GetParticle()->SetMaterial(RESOURCES->Get<Material>(L"AttackEffect10"));
            _effect->GetParticle()->SetLeft(false);
        }
        _effect->GetParticle()->SetDelayTime(0.f);
    }

    _effect->GetParticle()->SetDelayTime(0.4f);
    
}
void PlayerController::ActiveEffect(shared_ptr<GameObject> effect)
{
    Vec3 effectTransform = _transform->GetPosition();
    effectTransform += _transform->GetLook() * 2.f;
    effectTransform.y += 2.f;
    effect->GetOrAddTransform()->SetPosition(effectTransform);
    effect->GetParticle()->SetElapsedTime(0.0f);
}
void PlayerController::StartAirAttack()
{
    if (_isAirAttacking)
        return;

    {
        SOUND->PlayEffect(L"player_aerialAtk");
        SOUND->PlayEffect(L"player_aerialAtk_md");
    }
    
    _isAirAttacking = true;
    SetAttackReaource();
    ActiveEffect(_effect);
    CreateBubbleEffect(40, Vec3(4, 0.5, 3), 1, 1);
    _airAttackTimer = 0.0f;
    _airAttackDuration = _player->GetAnimationDuration(static_cast<AnimationState>((int)AnimationState::AirAttack));
    _airAttackDuration /= _FPS;

    _isPlayeringAirAttackAnimation = true;
    SetAnimationState(AnimationState::AirAttack);
}

void PlayerController::UpdateAirAttack()
{
    float dt = TIME->GetDeltaTime();

    _airAttackTimer += dt;

    UpdateAirHitBox();

    // 공중 공격 종료 처리
    if (_airAttackTimer >= _airAttackDuration)
    {
        _isAirAttacking = false;
        _isPlayeringAirAttackAnimation = false;
        if (GetGameObject()->GetRigidbody()->GetIsGrounded())
            SetAnimationState(AnimationState::Idle);
        else
            SetAnimationState(AnimationState::Jump);
    }
}

void PlayerController::StartChargeAttack()
{
    _isChargeAttacking = true;
    _chargeAttackTimer = 0.0f;
    _chargeAttackDuration = _player->GetAnimationDuration(static_cast<AnimationState>((int)AnimationState::AtkChargeThrust));
    _chargeAttackDuration /= _FPS;

    _isPlayeringChargeAttackAnimation = true;
    SetAnimationState(AnimationState::AtkChargeThrust);

    SOUND->PlayEffect(L"player_chargeUp");
}

void PlayerController::UpdateChargeAttack()
{
    float dt = TIME->GetDeltaTime();

    _chargeAttackTimer += dt;

    UpdateChargeHitBox();

    if (_chargeAttackTimer >= _chargeAttackDuration * 3 / 5)
    {
        if (!_isPlaySound)
        {
            SOUND->PlayEffect(L"player_chargeAtk");
            _isPlaySound = true;
        }
    }

    // 차지 공격 종료 처리
    if (_chargeAttackTimer >= _chargeAttackDuration)
    {
        _isChargeAttacking = false;
        _isPlayeringChargeAttackAnimation = false;
        _isCharging = false;
        _isPlaySound = false;
        _chargeTimer = 0.f;
        _chargeAttackTimer = 0.f;
        SetAnimationState(AnimationState::Idle);
    }
}

void PlayerController::StartDashAttack()
{
    if (_isDashAttacking)
        return;

    _isDashAttacking = true;
    _dashAttackTimer = 0.0f;
    _dashAttackDuration = _player->GetAnimationDuration(static_cast<AnimationState>((int)AnimationState::DashAtk));
    _dashAttackDuration /= _FPS;

    _isPlayeringDashAttackAnimation = true;
    SetAnimationState(AnimationState::DashAtk);

    // 대쉬 방향과 속도 설정
    Vec3 forward = _transform->GetLook(); // 플레이어가 바라보는 방향
    forward.Normalize();
    _dashDirection = forward;

    _dashSpeed = 10.0f; // 대쉬 속도
    _dashDistance = 5.0f; // 대쉬 거리
    _remainingDashDistance = _dashDistance; // 남은 대쉬 거리 초기화
}

void PlayerController::UpdateDashAttack()
{
    float dt = TIME->GetDeltaTime();

    _dashAttackTimer += dt;

    UpdateDashHitBox();

    if (!_isPlaySound)
    {
        SOUND->PlayEffect(L"player_dashAtk_1");
        SOUND->PlayEffect(L"player_dashAtk_2");
        SOUND->PlayEffect(L"player_dashAtk_3");
        _isPlaySound = true;
    }

    // 대쉬 이동 처리
    if (_remainingDashDistance > 0.0f)
    {
        float moveStep = min(_dashSpeed * dt, _remainingDashDistance); // 이동 거리 계산
        Vec3 newPosition = _transform->GetPosition() + _dashDirection * moveStep; // 새 위치 계산
        _transform->SetPosition(newPosition); // 위치 업데이트
        _remainingDashDistance -= moveStep; // 남은 거리 감소
    }

    // 대쉬 공격 종료 처리
    if (_dashAttackTimer >= _dashAttackDuration)
    {
        _isPlaySound = false;
        _isDashAttacking = false;
        _isPlayeringDashAttackAnimation = false;
        _remainingDashDistance = 0.f;
        SetAnimationState(AnimationState::Idle);
    }
}

void PlayerController::StartHit()
{
    if (_hit || _isAttacking || _isAirAttacking || _isChargeAttacking || _isDashAttacking)
        return;

    // 랜덤 사운드 재생
    int randNum = rand() % 4 + 1;
    wstring s = L"player_hit" + std::to_wstring(rand() % 4 + 1);
    SOUND->PlayEffect(s);

    _hit = true;
    _hitTimer = 0.0f;
    _hitDuration = _player->GetAnimationDuration(static_cast<AnimationState>((int)AnimationState::Hit1)); // 히트 동작 시간
    _hitDuration /= _FPS;

    _isPlayeringHitAnimation = true;
    SetAnimationState(AnimationState::Hit1);
}
void PlayerController::UpdateHit()
{
    float dt = TIME->GetDeltaTime();

    _hitTimer += dt;

    // 히트 상태 종료 처리
    if (_hitTimer >= _hitDuration)
    {
        _hit = false;
        _isPlayeringHitAnimation = false;
        SetAnimationState(AnimationState::Idle);
    }
}

void PlayerController::StartShellHit(shared_ptr<GameObject> attacker)
{
    if (_shellHit || _isAttacking || _isAirAttacking || _isChargeAttacking)
        return;

    _shellHit = true;
    _shellHitTimer = 0.0f;
    _shellHitDuration = _player->GetAnimationDuration(static_cast<AnimationState>((int)AnimationState::BlockHit)); // 히트 동작 시간
    _shellHitDuration /= (_FPS / 2); // 30프레임 애니메이션

    _isPlayeringShellHitAnimation = true;
    SetAnimationState(AnimationState::BlockHit);
    SOUND->PlayEffect(L"player_impachShell");

    // 공격자로부터 밀리는 로직
    if (attacker)
    {
        Vec3 attackerPosition = attacker->GetTransform()->GetPosition(); // 공격자의 위치
        Vec3 playerPosition = _transform->GetPosition();                // 플레이어의 위치

        Vec3 knockbackDirection = playerPosition - attackerPosition; // 공격자 -> 플레이어 방향
        knockbackDirection.Normalize();                              // 방향 벡터 정규화

        float knockbackForce = 100.0f; // 밀리는 힘
        _rigidbody->Addforce(knockbackDirection * knockbackForce); // 힘 적용
    }
}
void PlayerController::UpdateShellHit()
{
    float dt = TIME->GetDeltaTime();

    _shellHitTimer += dt;

    // 히트 상태 종료 처리
    if (_shellHitTimer >= _shellHitDuration)
    {
        _shellHit = false;
        _isPlayeringShellHitAnimation = false;
        SetAnimationState(AnimationState::BlockingIdle);
    }
}

void PlayerController::StartDodge()
{
    if (_isDodging)
        return;

    SOUND->PlayEffect(L"player_dash");
    _isDodging = true;
    _dodgeTimer = 0.0f;
    _dodgeDistance = 3.0f; // 회피 이동 거리
    
    // 회피 방향 설정
    _dodgeDirection = _moveDir;
    if (_dodgeDirection.LengthSquared() == 0.0f)
    {
        _isBackStep = true;

        _dodgeDistance = 5.0f; // 회피 이동 거리
        _dodgeDirection = _transform->GetLook(); // 입력이 없으면 바라보는 방향 뒤로 Stepback
        _dodgeDirection = Vec3(-_dodgeDirection.x, -_dodgeDirection.y, -_dodgeDirection.z);

        _dodgeDuration = _player->GetAnimationDuration(static_cast<AnimationState>((int)AnimationState::DodgeStepback)); // 회피 동작 시간
        _dodgeDuration /= _FPS;
        SetAnimationState(AnimationState::DodgeStepback);
    }
    else 
    {
        if (_isShellEquipped) // 등껍질 착용 상태
        {
            _dodgeDuration = _player->GetAnimationDuration(static_cast<AnimationState>((int)AnimationState::DodgeMedium)); // 회피 동작 시간
            _dodgeDuration /= (_FPS / 2);   // 30FPS 애니메이션임
            SetAnimationState(AnimationState::DodgeMedium);
        }
        else
        {
            _dodgeDuration = _player->GetAnimationDuration(static_cast<AnimationState>((int)AnimationState::Dodge)); // 회피 동작 시간
            _dodgeDuration /= _FPS;
            SetAnimationState(AnimationState::Dodge);
        }
    }
    _dodgeDirection.Normalize();

    _isPlayeringDodgeAnimation = true;

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
        _isBackStep = false;
        _isInvincible = false; // 무적 상태 해제
        _isPlayeringDodgeAnimation = false;
        SetAnimationState(AnimationState::Idle);
    }
}

void PlayerController::Jump()
{
    auto rigidbody = _rigidbody;
    if (rigidbody->GetIsGrounded())
    {
        SOUND->PlayEffect(L"player_jump");
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

        CreateBubbleEffect(10, Vec3(2.f,1.f,1.f),1.f, -1.f);
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

void PlayerController::SetBubble(shared_ptr<Material> bubble)
{
    _bubbleMaterial = bubble;
    RESOURCES->Add(L"Dust", bubble);
}
void PlayerController::CreateBubbleEffect(int numBubbles, Vec3 bubbleSpread, float positionY, float positionLook)
{
    for (int i = 0; i < numBubbles; i++)
    {
        auto obj = make_shared<GameObject>();
        obj->GetOrAddTransform()->SetLocalPosition(Vec3(0, 0, 0));
        obj->AddComponent(make_shared<Particle>());

        Vec3 bubblePosition = _transform->GetPosition();
        bubblePosition += _transform->GetLook() * positionLook;
        bubblePosition.y += positionY;

        float angle = MathUtils::Random(0.0f, 2.0f * 3.141592f);
        float radius = MathUtils::Random(0.0f, bubbleSpread.x);

        bubblePosition.x += cos(angle) * radius;
        bubblePosition.z += sin(angle) * radius;

        bubblePosition.y += MathUtils::Random(-bubbleSpread.y / 2, bubbleSpread.y / 2);


        float randomSize = MathUtils::Random(0.2f, 0.4f);

        obj->GetParticle()->SetMaterial(_bubbleMaterial);
        obj->GetParticle()->SetLifetime(1.0f);
        obj->GetParticle()->SetfadeStart(0.3f);
        obj->GetParticle()->Add(bubblePosition, Vec2(randomSize));
        CUR_SCENE->Add(obj);
    }
}

void PlayerController::HealPlayer()
{
    if (auto ui = UIMANAGER->GetUi("PlayerHP"))
    {
        _hp += _healHp;
        _hp = std::clamp(_hp, 0.0f, _maxHp);

        auto hpSlider = dynamic_pointer_cast<Slider>(ui);
        float hpRatio = _hp / _maxHp;
        hpSlider->SetRatio(hpRatio);
        SOUND->PlayEffect(L"player_heal");
    }
}

void PlayerController::LoadPlayer(SaveData data)
{
    if (_isDead)
    {
        _isDead = false;
        _hp = _maxHp;
        if (auto ui = UIMANAGER->GetUi("PlayerHP"))
        {
            auto hpSlider = dynamic_pointer_cast<Slider>(ui);
            float hpRatio = _hp / _maxHp;
            hpSlider->SetRatio(hpRatio);
        }
    }
   
    _transform->SetLocalPosition(data.playerPos);
}

void PlayerController::OnDeath()
{
    if (_isDead)
        return;

    // 게임 오버 메시지 출력
    std::cout << "Player has died! Game Over!" << std::endl;

    SOUND->PlayEffect(L"player_die");

    // 죽었을 때 UI 표시
    SAVE->OpenSaveUI();

    // 플레이어 Death 애니메이션
    _isDead = true;
    _deadDuration = _player->GetAnimationDuration(static_cast<AnimationState>((int)AnimationState::Die)); // 히트 동작 시간
    _deadDuration /= _FPS;
    SetAnimationState(AnimationState::Die);

    // 죽음 처리 이후 리스폰 또는 종료 처리
    // TODO
    //TaskQueue::GetInstance().AddTask([this]() {
    //    std::this_thread::sleep_for(std::chrono::seconds(3)); // 3초 대기

    //    // 선택적으로 리스폰 또는 메인 메뉴로 이동
    //    if (_allowRespawn)
    //    {
    //        RespawnPlayer(); // 리스폰 함수 호출
    //    }
    //    else
    //    {
    //        Game::GetInstance().ChangeScene(SceneTag::TITLE); // 타이틀 화면으로 이동
    //    }
    //    });
}

void PlayerController::onChoked()
{
    const float targetY = 5.0f;

    // 총 이동 시간 (초)
    const float duration = 3.0f;

    // 현재 경과 시간
    static float elapsedTime = 0.0f;

    static bool onDamageTriggered1 = false;
    static bool onDamageTriggered2 = false;
    static bool onDamageTriggered3 = false;
    static bool onDamageTriggered4 = false;

    SetAnimationState(AnimationState::Struggle);

    // 현재 y 위치 계산
    if (elapsedTime < duration) {
        elapsedTime += DT; // 경과 시간 증가
        
        // 현재 위치 = 선형 보간 (Lerp)
        float t = elapsedTime / duration; // 0.0f ~ 1.0f
        float currentY = t * targetY;

        if (t >= 3.0f / 10.0f && !onDamageTriggered1) {
            OnDamage(GetGameObject(), 10.0f);
            SOUND->PlayEffect(L"player_hit2");
            onDamageTriggered1 = true;
        }
        if (t >= 6.0f / 10.0f && !onDamageTriggered2) {
            OnDamage(GetGameObject(), 10.0f);
            SOUND->PlayEffect(L"player_hit2");
            onDamageTriggered2 = true;
        }
        if (t >= 9.0f / 10.0f && !onDamageTriggered3) {
            OnDamage(GetGameObject(), 10.0f);
            SOUND->PlayEffect(L"player_hit2");
            onDamageTriggered3 = true;
        }
        if (!onDamageTriggered4)
        {
            _transform->SetPosition(_transform->GetPosition() + fixedPos * 5.0f);
            onDamageTriggered4 = true;
        }

        // 위치 설정
        Vec3 currentPosition = _transform->GetPosition();
        currentPosition.y = currentY;
        _transform->SetPosition(currentPosition);
    }
    else
    {
        SetAnimationState(AnimationState::Idle);
        _rigidbody->SetUseGravity(true);
        startChoke = false;
        elapsedTime = 0.0f;
        onDamageTriggered1 = false;
        onDamageTriggered2 = false;
        onDamageTriggered3 = false;
        onDamageTriggered4 = false;
    }
}