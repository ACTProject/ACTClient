#include "pch.h"
#include "FinalBossMonsterFirstPhaseController.h"
#include "PlayerController.h"

#define AttackRange 8.0f

void FinalBossMonsterFirstPhaseController::SetAnimationState(AnimationState state)
{
    _modelAnimator->ChangeAnimation(state);
    _currentAnimationState = state;
}

void FinalBossMonsterFirstPhaseController::ResetToIdleState()
{
    SetAnimationState(AnimationState::Combat);
}

bool FinalBossMonsterFirstPhaseController::PlayCheckAnimating(AnimationState state)
{
    SetAnimationState(state);

    animPlayingTime += DT;
    duration = _enemy->GetAnimationDuration(state) / _FPS;

    if (animPlayingTime >= duration)
    {
        animPlayingTime = 0.0f;
        ResetToIdleState();
        return false;
    }

    return true; // 플레이 중
}

void FinalBossMonsterFirstPhaseController::Start()
{
    Super::Start();
    _maxHp = 100.f;
    _hp = 100.0f;
    _atk = 50.0f;

    _transform = GetTransform();
    _player = SCENE->GetCurrentScene()->GetPlayer();
    SetAnimationState(AnimationState::Idle);
    randPunchType = rand() % 4;
    randType = rand() % 10;
}

void FinalBossMonsterFirstPhaseController::Update()
{
    Super::Update();
    currentTime = TIME->GetGameTime(); // 현재 게임 시간

    if (_isDead)
    {
        if (PlayCheckAnimating(AnimationState::Down2))
        {
            return;
        }
        _hpBar->SetActive(false);
        _secondPhase->SetActive(true);
        _secondPhase->GetTransform()->SetPosition(bossPos);
        Super::OnDeath();
        std::cout << "FinalBoss Phase_1 has been ended!" << std::endl;

        Vec3 pos = bossPos;
        TaskQueue::GetInstance().AddTask([this, pos]() {
            ENEMY->CreateFinalPhase(pos);
            });

        return;
    }
    _FPS = static_cast<float>(TIME->GetFps());
    
    bossPos = _transform->GetPosition();
    playerPos = _player->GetTransform()->GetPosition();

    direction = bossPos - playerPos;
    distance = direction.Length();

    Rota(bossPos, playerPos);

    Phase_1();
}

void FinalBossMonsterFirstPhaseController::Phase_1()
{
    if (!isFirstTime)
    {
        if (currentTime > 3.f) // 실행되는데 걸리는 시간으로 인한 애니메이션이 짤리는 현상때문에 설정
        {
            if (!playingSound)
            {
                SOUND->PlayEffect(L"boss_narrate_intro");
                playingSound = true;
            }
            Appear();
            return;
        }
        else
        {
            return;
        }
    }

    if (currentTime - lastTime > 0.0f)
    {
        postpone = true;
    }
    else
    {
        postpone = false;
    }

    if (postpone)
    {
        if (patternCnt < 4)
        {
            if (distance < AttackRange)
            {
                punchState = true;
            }
            if (punchState)
            {
                if (PlayCheckAnimating(static_cast<AnimationState>((int)AnimationState::Attack1 + randPunchType)))
                {
                    Punch(); // 히트 및 데미지 처리
                    return;
                }
                else
                {
                    randPunchType = rand() % 4;
                    patternCnt++;
                    punchState = false;
                    playingSound = false;
                    ResetHit();
                }
            }
            else
            {
                Rota(bossPos, playerPos);
                if (distance < 30.0f)
                {
                    Run(15.0f);
                }
                else
                {
                    Move(bossPos, playerPos, speed);
                }
            }
        }

        if (patternCnt == 4)
        {
            if (PlayCheckAnimating(AnimationState::Skill2))
            {
                Fireball();
                return;
            }
            else
            {
                playingSound = false;
                lastTime = currentTime;
                shootTime = 0.0f;
                patternCnt = 1;
            }
        }
    }
    else
    {
        int randomHit = rand() % 3;
        if (PlayingHitMotion && randomHit == 0)
        {
            if (!playingSound)
            {
                SOUND->PlayEffect(L"boss_hit" + std::to_wstring(rand() % 3 + 1));
                playingSound = true;
            }
            if (PlayCheckAnimating(AnimationState::Hit1))
            {
                return;
            }
        }
        playingSound = false;
        PlayingHitMotion = false;
        Rota(bossPos, playerPos);
    }

}

void FinalBossMonsterFirstPhaseController::Appear()
{
    if (PlayCheckAnimating(AnimationState::Appear))
    {
        return;
    }
    _hpBar->SetActive(true);
    isFirstTime = true;
    lastTime = currentTime;
    playingSound = false;
}

void FinalBossMonsterFirstPhaseController::Move(Vec3 objPos, Vec3 targetPos, float speed)
{
    _footstepTimer += DT;
    if (_footstepTimer >= _walkingInterval)
    {
        wstring w = L"boss_footstep" + std::to_wstring(_footstepType);
        SOUND->PlayEffect(w);
        if (_footstepType < 4)
        {
            _footstepType++;
        }
        else
        {
            _footstepType = 1;
        }
        _footstepTimer = 0.0f;
    }
    SetAnimationState(AnimationState::Walk);

    Vec3 direction = targetPos - objPos;
    if (direction.LengthSquared() < 5.f) // EPSILON 사용
    {
        ResetToIdleState();
        return;
    }

    direction.Normalize();  // 방향 벡터를 단위 벡터로 정규화

    _transform->SetPosition(_transform->GetPosition() + direction * speed * DT);  // 일정 거리만큼 이동
}

void FinalBossMonsterFirstPhaseController::Rota(Vec3 objPos, Vec3 targetPos)
{
    Vec3 CurForward = _transform->GetLook();
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

void FinalBossMonsterFirstPhaseController::Run(float speed)
{
    _footstepTimer += DT;
    if (_footstepTimer >= _runningInterval)
    {
        wstring w = L"boss_footstep" + std::to_wstring(_footstepType);
        SOUND->PlayEffect(w);
        if (_footstepType < 4)
        {
            _footstepType++;
        }
        else
        {
            _footstepType = 1;
        }
        _footstepTimer = 0.0f;
    }

    SetAnimationState(AnimationState::Run);

    Vec3 direction = playerPos - bossPos;
    if (direction.LengthSquared() < 5.f) // EPSILON 사용
    {
        ResetToIdleState();
        return;
    }

    direction.Normalize();  // 방향 벡터를 단위 벡터로 정규화

    _transform->SetPosition(_transform->GetPosition() + direction * speed * DT);  // 일정 거리만큼 이동
}

void FinalBossMonsterFirstPhaseController::Punch()
{
    if (animPlayingTime >= duration / 3.0f)
    {
        UpdateHitBox();
        {
            if (!playingSound)
            {
                wstring s = L"boss_punch" + std::to_wstring(randPunchType + 1);
                SOUND->PlayEffect(s);
                SOUND->PlayEffect(L"boss_punch_vo" + to_wstring(rand() % 3 + 1));
                playingSound = true;
            }
        }
    }
    if (_hit && !hasDealing)
    {
        auto player = dynamic_pointer_cast<PlayerController>(_player->GetController());
        player->OnDamage(GetGameObject(), _atk);
        hasDealing = true;
    }
}

void FinalBossMonsterFirstPhaseController::Fireball()
{
    Rota(bossPos, playerPos);
    shootTime += DT;
    float startTime = 208 / 60 - 1.2;
    float endTime = 208 / 60 - 0.9;
    if ((shootTime <= endTime) && (shootTime > startTime))
    {
        float randX = rand() % 10 / 5.f; // 0 ~ 1.8
        float randY = rand() % 10 / 5.f;
        float randZ = rand() % 10 / 5.f;
        makeBubble({ bossPos.x + randX, bossPos.y + randY , bossPos.z + randZ }, playerPos - bossPos);
        SOUND->PlayEffect(L"boss_bubbleSpawn");
        if (!playingSound)
        {
            SOUND->PlayEffect(L"boss_bubbleBullet_vo");
            playingSound = true;
        }
    }
}

void FinalBossMonsterFirstPhaseController::makeBubble(Vec3 pos, Vec3 dir)
{
    auto bullet = make_shared<GameObject>(); // bullet

    bullet->GetOrAddTransform()->SetPosition({ pos.x, pos.y + 3.f, pos.z });
    bullet->GetOrAddTransform()->SetLocalRotation(dir); // XMConvertToRadians()
    bullet->GetOrAddTransform()->SetScale(Vec3(0.005f));

    shared_ptr<Model> objModel = make_shared<Model>();
    // Model
    objModel->ReadModel(L"Enemy/bubble");
    objModel->ReadMaterial(L"Enemy/bubble");

    bullet->AddComponent(make_shared<ModelRenderer>(renderShader));
    {
        bullet->GetModelRenderer()->SetModel(objModel);
        bullet->GetModelRenderer()->SetPass(4);
    }

    shared_ptr<Bullet> bulletComponent = make_shared<Bullet>();
    bulletComponent->Add(objModel);
    bulletComponent->SetDirection({ dir.x,dir.y - 3.0f,dir.z });
    bulletComponent->SetSpeed(30.0f);

    // HitBox
    shared_ptr<GameObject> hitboxGO = make_shared<GameObject>();
    shared_ptr<HitBox> hitbox = make_shared<HitBox>();
    hitboxGO->AddComponent(hitbox);
    hitbox->SetOffSet(Vec3(0.f, 0.6f, 0.f));
    hitbox->Craete(bullet, Vec3(0.1f));
    CUR_SCENE->Add(hitboxGO);
    bulletComponent->SetHitBox(hitboxGO);
    bullet->AddComponent(bulletComponent);

    CUR_SCENE->Add(bullet);
}

void FinalBossMonsterFirstPhaseController::OnDeath()
{
    _isDead = true;
    animPlayingTime = 0.0f;
}

void FinalBossMonsterFirstPhaseController::UpdateHitBox()
{
    if (_hit)
        return;

    auto hitboxCollider = _hitbox->GetCollider();
    hitboxCollider->SetActive(true);

    _hitbox->GetTransform()->SetPosition(_transform->GetPosition()
        + _hitbox->GetHitBox()->GetOffSet() + _transform->GetLook() * 8.0f);

    vector<shared_ptr<BaseCollider>> nearbyColliders = OCTREE->QueryColliders(hitboxCollider);

    for (const auto& collider : nearbyColliders)
    {
        ObjectType type = collider->GetGameObject()->GetObjectType();
        if (type != ObjectType::Player)
            continue;

        if (hitboxCollider->Intersects(collider))
        {
            auto controller = collider->GetGameObject()->GetController();
            if (!controller)
                continue;

            auto player = dynamic_pointer_cast<PlayerController>(controller);
            if (player)
                _hit = true;
        }
    }
}

void FinalBossMonsterFirstPhaseController::ResetHit()
{
    hasDealing = false;
    _hit = false;
    _hitbox->GetCollider()->SetActive(false);
}

