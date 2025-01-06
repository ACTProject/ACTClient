#include "pch.h"
#include "FinalBossMonsterSecondPhaseController.h"
#include "PlayerController.h"

#define AttackRange 8.0f

void FinalBossMonsterSecondPhaseController::SetAnimationState(AnimationState state)
{
    _modelAnimator->ChangeAnimation(state);
    _currentAnimationState = state;
}

void FinalBossMonsterSecondPhaseController::ResetToIdleState()
{
    SetAnimationState(AnimationState::Idle);
}

bool FinalBossMonsterSecondPhaseController::PlayCheckAnimating(AnimationState state)
{
    SetAnimationState(state);

    animPlayingTime += DT;
    duration = _enemy->GetAnimationDuration(state) / _FPS;

    if (animPlayingTime >= duration)
    {
        animPlayingTime = 0.0f;
        if (state != AnimationState::Roar)
        {
            randType = 1;
            std::cout << randType << std::endl;
        }
        ResetToIdleState();
        return false;
    }

    return true; // 플레이 중
}

void FinalBossMonsterSecondPhaseController::Start()
{
    Super::Start();

    _maxHp = 20.f;
    _hp = 20.0f;
    _atk = 50.0f;
    speed = 15.0f;
    randPunchType = rand() % 4;
    randType = rand() % 7;
}

void FinalBossMonsterSecondPhaseController::Update()
{
    Super::Update();

    if (INPUT->GetButton(KEY_TYPE::KEY_4))
    {
        int a = 0;
    }
    _isDead = true;
    _transform = GetTransform();
    _player = SCENE->GetCurrentScene()->GetPlayer();
    playerPos = _player->GetTransform()->GetPosition();
    bossPos = _transform->GetPosition();

    if (_isDead)
    {
        _transform->SetPosition(Vec3(31.0f, 0.0f, 46.0f));

        auto camera = CUR_SCENE->GetMainCamera()->GetCamera();
        Vec3 start(37.0577f, 7.88812f, 66.2076f);
        Vec3 end(37.0577f, 7.88812f, 66.2076f);
        Vec3 focus(-0.209137f, - 0.16879f, - 0.963209f);
        float duration = 10.0f;

        camera->StartCutscene(start, end, focus, duration);
        _player->GetTransform()->SetPosition(Vec3(50.0f, 0.f, 93.0f));

        if (!playingSound)
        {
            SOUND->Stop(L"bgm");
            SOUND->PlayEffect(L"boss_die");
            playingSound = true;
        }

        SetAnimationState(AnimationState::Die);
        animPlayingTime += DT;
        duration = _enemy->GetAnimationDuration(AnimationState::Die) / _FPS;
        if (animPlayingTime < duration)
        {
            lastTime = GAMETIME;
            return;
        }
        cout << currentTime << "  ----  " << lastTime << endl;
        if (currentTime - lastTime > 3.0f)
        {
            auto player = dynamic_pointer_cast<PlayerController>(_player->GetController());
            if (player)
            {
                player->FinishGame();
                std::cout << "FinalBoss has been defeated! Game Over!" << std::endl;
            }
        }
        return;
    }

    direction = bossPos - playerPos;
    distance = direction.Length();

    Rota(bossPos, playerPos);

    SOUND->SetVolume(L"bgm", 0.3f);
    Phase_2();
}

void FinalBossMonsterSecondPhaseController::Phase_2()
{
    if (!isFirstTime) // 2페이즈 시작 (안되게 해놨음)
    {
        if (!Phase2Flag) // 한번만 실행
        {
            SOUND->Stop(L"bgm");
            SOUND->Load(L"bgm", L"bgm/Nephro ogg 117bpm final");
            SOUND->Play(L"bgm");
            DEBUG->Log(L"Boss 2nd Phase Start");
            Phase2Flag = true;
            _hpBar->SetActive(true);
        }
        if (!isExecuted)
        {
            if (PlayCheckAnimating(AnimationState::GetUP2))
            {
                if (!playingSound)
                {
                    SOUND->PlayEffect(L"boss_narrate_laugh_full");
                    playingSound = true;
                }
                return;
            }
            playingSound = false;
            isExecuted = true;
        }
        Rota(bossPos, playerPos);
        if (PlayCheckAnimating(AnimationState::Roar))
        {
            if (!playingSound)
            {
                SOUND->PlayEffect(L"boss_roar_1");
                playingSound = true;
            }
            return;
        }
        playingSound = false;
        isFirstTime = true; // 플래그
    }

    if (randType >= 0 && randType < phaseActions.size()) {
        (this->*phaseActions[randType])();
    }

}

void FinalBossMonsterSecondPhaseController::Appear()
{
    if (PlayCheckAnimating(AnimationState::Appear))
    {
        return;
    }
    isFirstTime = false;
}

void FinalBossMonsterSecondPhaseController::Move(Vec3 objPos, Vec3 targetPos, float speed)
{
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

void FinalBossMonsterSecondPhaseController::Rota(Vec3 objPos, Vec3 targetPos)
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

void FinalBossMonsterSecondPhaseController::Sprint()
{
    SetAnimationState(AnimationState::Run2);

    Vec3 dir = playerPos - bossPos;
    if (dir.LengthSquared() < 5.f) // EPSILON 사용
    {
        ResetToIdleState();
        return;
    }

    dir.Normalize();  // 방향 벡터를 단위 벡터로 정규화

    _transform->SetPosition(_transform->GetPosition() + dir * 25.0f * DT);  // 일정 거리만큼 이동

}

void FinalBossMonsterSecondPhaseController::BackSprint()
{
    SetAnimationState(AnimationState::Run3);

    Vec3 direction = bossPos - playerPos;
    if (distance > 20.f) //
    {
        ResetToIdleState();
        return;
    }

    direction.Normalize();  // 방향 벡터를 단위 벡터로 정규화

    _transform->SetPosition(_transform->GetPosition() + direction * 10.0f * DT);  // 일정 거리만큼 이동
}

void FinalBossMonsterSecondPhaseController::Run(float speed)
{
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

void FinalBossMonsterSecondPhaseController::Punch()
{
    if (distance < AttackRange)
    {
        punchState = true;
    }
    if (punchState)
    {
        if (PlayCheckAnimating(static_cast<AnimationState>((int)AnimationState::Attack1 + randPunchType)))
        {
            if (animPlayingTime >= duration / 3.0f)
            {
                UpdateHitBox(5.0f, _atk);
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
            return;
        }
        else
        {
            randPunchType = rand() % 4;
            punchState = false;
            _hitbox->GetCollider()->SetActive(false);
            _hit = false;
            playingSound = false;
            isExecuted_3 = false;
        }
    }
    else
    {
        if (!isExecuted_3 && (randPunchType == 1 || randPunchType == 2))
        {
            int randvoice = rand() % 7 + 1;
            wstring s = L"boss_narrate_" + std::to_wstring(randvoice);
            SOUND->PlayEffect(s);
            isExecuted_3 = true;
        }
        Run(20.0f);
        Rota(bossPos, playerPos);
    }
}

void FinalBossMonsterSecondPhaseController::Fireball()
{
    if (PlayCheckAnimating(AnimationState::Skill2))
    {
        Rota(bossPos, playerPos);
        shootTime += DT;
        float startTime = 208 / 60 - 0.6;
        float endTime = 208 / 60 - 0.4;
        if ((shootTime <= endTime) && (shootTime > startTime))
        {
            float randX = rand() % 10 / 5.f; // 0 ~ 1.8
            float randY = rand() % 10 / 5.f;
            float randZ = rand() % 10 / 5.f;
            makeBubble({ bossPos.x + randX, bossPos.y + randY , bossPos.z + randZ }, playerPos - bossPos);
            SOUND->PlayEffect(L"boss_bubbleSpawn");
            if (!playingSound)
            {
                SOUND->Play(L"boss_bubbleMove", true);
                SOUND->PlayEffect(L"boss_bubbleBullet_vo");
                SOUND->PlayEffect(L"boss_narrate_laugh_full");
                playingSound = true;
            }
        }
        return;
    }
    else
    {
        shootTime = 0.0f;
        playingSound = false;
    }
    
}

void FinalBossMonsterSecondPhaseController::FireMoney()
{
    if (PlayCheckAnimating(AnimationState::Skill3))
    {
        Rota(bossPos, playerPos);
        shootTime += DT;
        float startTime = 160 / 60;
        if ((shootTime > startTime) && !shootState)
        {
            Vec3 forward = _transform->GetLook();
            Vec3 rightVec = _transform->GetRight();
            Vec3 upVec = _transform->GetUp();

            rightVec.Normalize();
            upVec.Normalize();

            Vec3 dir = playerPos - bossPos;
            dir.y -= 0.5f;
            makeCash({ bossPos.x, bossPos.y + upVec.y / 0.5f , bossPos.z }, dir);
            makeCash({ bossPos.x, bossPos.y - upVec.y / 0.5f , bossPos.z }, dir);
            makeCash({ bossPos.x, bossPos.y , bossPos.z }, dir);
            makeCash({ bossPos.x + rightVec.x / 0.5f, bossPos.y, bossPos.z }, dir);
            makeCash({ bossPos.x - rightVec.x / 0.5f, bossPos.y, bossPos.z }, dir);

            SOUND->PlayEffect(L"boss_moenySpawn");
            shootState = true;
        }
        if (!playingSound)
        {
            SOUND->Play(L"boss_bubbleMove", true);
            SOUND->PlayEffect(L"boss_bubbleBullet_vo");
            SOUND->PlayEffect(L"boss_narrate_laugh_full2");
            playingSound = true;
        }
        return;
    }
    else
    {
        playingSound = false;
        shootState = false;
        shootTime = 0.0f;
    }
    
}

void FinalBossMonsterSecondPhaseController::makeBubble(Vec3 pos, Vec3 dir)
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
    bulletComponent->Add(objModel);

    // HitBox
    shared_ptr<GameObject> hitboxGO = make_shared<GameObject>();
    shared_ptr<HitBox> hitbox = make_shared<HitBox>();
    hitboxGO->AddComponent(hitbox);
    hitbox->SetOffSet(Vec3(0.f, 0.6f, 0.f));
    hitbox->Craete(bullet, Vec3(1.0f));
    CUR_SCENE->Add(hitboxGO);
    bulletComponent->SetHitBox(hitboxGO);
    bullet->AddComponent(bulletComponent);

    CUR_SCENE->Add(bullet);
}

void FinalBossMonsterSecondPhaseController::makeCash(Vec3 pos, Vec3 dir)
{
    auto bullet = make_shared<GameObject>(); // bullet

    bullet->GetOrAddTransform()->SetPosition({ pos.x, pos.y + 3.f, pos.z });
    bullet->GetOrAddTransform()->SetLocalRotation({ 0,0,0 }); // XMConvertToRadians()
    bullet->GetOrAddTransform()->SetScale(Vec3(0.004f));

    shared_ptr<Model> objModel = make_shared<Model>();
    // Model
    objModel->ReadModel(L"Enemy/money");
    objModel->ReadMaterial(L"Enemy/money");

    bullet->AddComponent(make_shared<ModelRenderer>(renderShader));
    {
        bullet->GetModelRenderer()->SetModel(objModel);
        bullet->GetModelRenderer()->SetPass(4);
    }

    shared_ptr<Bullet> bulletComponent = make_shared<Bullet>();
    bulletComponent->Add(objModel);
    bulletComponent->SetDirection(dir);
    bulletComponent->SetSpeed(30.0f);
    bulletComponent->Add(objModel);

    // HitBox
    shared_ptr<GameObject> hitboxGO = make_shared<GameObject>();
    shared_ptr<HitBox> hitbox = make_shared<HitBox>();
    hitboxGO->AddComponent(hitbox);
    hitbox->SetOffSet(Vec3(0.f, 0.0f, 0.f));
    hitbox->Craete(bullet, Vec3(1.0f));
    CUR_SCENE->Add(hitboxGO);
    bulletComponent->SetHitBox(hitboxGO);
    bullet->AddComponent(bulletComponent);

    CUR_SCENE->Add(bullet);
}

void FinalBossMonsterSecondPhaseController::Choke_lift()
{
    if (!isExecuted_2)
    {
        if (PlayCheckAnimating(AnimationState::Roar))
        {
            if (!playingSound)
            {
                SOUND->PlayEffect(L"boss_roar_2");
                playingSound = true;
            }
            return;
        }
        isExecuted_2 = true;
        playingSound = false;
    }

    if (distance < 5.f)
    {
        attackState = true;
    }
    if (attackState)
    {
        if (PlayCheckAnimating(AnimationState::Skill1))
        {
            if (animPlayingTime >= duration / 5.0f)
            {
                UpdateChokeHitBox();
                if (_hit && animPlayingTime >= duration * 5.0f / 7.0f)
                {
                    auto player = dynamic_pointer_cast<PlayerController>(_player->GetController());
                    player->_playerActive = true;
                    player->startChoke = false;
                }
            }
            return;
        }
        else
        {
            _chokeHitbox->GetCollider()->SetActive(false);
            isExecuted_3 = false;
            isExecuted_2 = false;
            attackState = false;
            _hit = false;
        }
    }
    else
    {
        if (!isExecuted_3)
        {
            SOUND->PlayEffect(L"boss_narrate_1");
            isExecuted_3 = true;
        }
        Sprint();
        Rota(bossPos, playerPos);
    }
}

void FinalBossMonsterSecondPhaseController::Slash()
{
    if (distance < AttackRange + 3.0f)
    {
        punchState = true;
    }
    if (punchState)
    {
        if (PlayCheckAnimating(AnimationState::Skill5))
        {
            if (!playingSound)
            {
                SOUND->PlayEffect(L"boss_atk1");
                playingSound = true;
            }
            if (animPlayingTime >= duration / 2.0f)
            {
                UpdateHitBox(10.0f, _atk * 2.0f);
            }
            return;
        }
        else
        {
            isExecuted_3 = false;
            playingSound = false;
            _hitbox->GetCollider()->SetActive(false);
            punchState = false;
            _hit = false;
        }
    }
    else
    {
        if (!isExecuted_3)
        {
            int randvoice = rand() % 9 + 1;
            wstring s = L"boss_narrate_" + std::to_wstring(randvoice);
            SOUND->PlayEffect(s);
            isExecuted_3 = true;
        }
        Run(20.0f);
        Rota(bossPos, playerPos);
    }
    
}

void FinalBossMonsterSecondPhaseController::Hurricane()
{
    if (!isExecuted_2)
    {
        if (PlayCheckAnimating(AnimationState::Roar))
        {
            if (!playingSound)
            {
                SOUND->PlayEffect(L"boss_roar_3");
                playingSound = true;
            }
            return;
        }
        isExecuted_2 = true;
        playingSound = false;
    }

    if (distance < AttackRange + 5.0f)
    {
        attackState = true;
    }
    if (attackState)
    {
        
        if (PlayCheckAnimating(AnimationState::Skill9))
        {
            UpdateHurricaneHitBox();
            Vec3 direction = playerPos - bossPos;
            if (direction.LengthSquared() < 5.f) // EPSILON 사용
            {
                return;
            }

            direction.Normalize();  // 방향 벡터를 단위 벡터로 정규화

            _transform->SetPosition(_transform->GetPosition() + direction * 10.0f * DT);  // 일정 거리만큼 이동

            if (!playingSound)
            {
                SOUND->Play(L"boss_hurricane");
                SOUND->PlayEffect(L"boss_hurricane_vo");
                playingSound = true;
            }
            return;
        }
        else
        {
            SOUND->Stop(L"boss_hurricane");
            isExecuted_2 = false;
            isExecuted_3 = false;
            playingSound = false;
            _hurricaneHitbox->GetCollider()->SetActive(false);
            _hit = false;
        }
    }
    else
    {
        if (!isExecuted_3)
        {
            SOUND->PlayEffect(L"boss_narrate_3");
            isExecuted_3 = true;
        }
        Sprint();
        Rota(bossPos, playerPos);
    }
    
}

void FinalBossMonsterSecondPhaseController::OnDeath()
{
    _isDead = true;
    animPlayingTime = 0.0f;
}

void FinalBossMonsterSecondPhaseController::UpdateHitBox(float f, float damage)
{
    if (!_hitbox || _hit)
        return;

    auto hitboxCollider = _hitbox->GetCollider();
    hitboxCollider->SetActive(true);

    _hitbox->GetTransform()->SetPosition(_transform->GetPosition()
        + _hitbox->GetHitBox()->GetOffSet() + _transform->GetLook() * f);

    checkHit(hitboxCollider, damage);
}

void FinalBossMonsterSecondPhaseController::Slam()
{
    if (distance < 10.0f)
    {
        punchState = true;
    }
    if (punchState)
    {
        if (PlayCheckAnimating(AnimationState::Skill7))
        {
            if (animPlayingTime >= duration / 2.0f)
            {
                UpdateSlamHitBox();
                if (!playingSound)
                {
                    SOUND->PlayEffect(L"boss_slam");
                    SOUND->PlayEffect(L"boss_slam2");
                    playingSound = true;
                }
            }
            return;
        }
        else
        {
            _transform->SetLocalPosition(_slamhitbox->GetTransform()->GetPosition());
            punchState = false;
            _hit = false;
            _slamhitbox->GetCollider()->SetActive(false);
            playingSound = false;
            isExecuted_3 = false;
        }
    }
    else
    {
        if (!isExecuted_3)
        {
            SOUND->PlayEffect(L"boss_narrate_5");
            isExecuted_3 = true;
        }
        Sprint();
        Rota(bossPos, playerPos);
    }
    
}

void FinalBossMonsterSecondPhaseController::UpdateSlamHitBox()
{
    if (!_slamhitbox || _hit)
        return;

    auto hitboxCollider = _slamhitbox->GetCollider();
    hitboxCollider->SetActive(true);

    Vec3 bLeft = DirectX::XMVectorNegate(_transform->GetRight());

    _slamhitbox->GetTransform()->SetPosition(_transform->GetPosition()
        + _slamhitbox->GetHitBox()->GetOffSet() + (_transform->GetLook() + bLeft) * 8.0f);

    checkHit(hitboxCollider, _atk * 2.0f);
}

void FinalBossMonsterSecondPhaseController::UpdateHurricaneHitBox()
{
    if (_hit || !_hurricaneHitbox)
        return;

    auto hitboxCollider = _hurricaneHitbox->GetCollider();
    hitboxCollider->SetActive(true);

    _hurricaneHitbox->GetTransform()->SetPosition(_transform->GetPosition() + _hurricaneHitbox->GetHitBox()->GetOffSet());

    checkHit(hitboxCollider, _atk * 2.0f);
}

void FinalBossMonsterSecondPhaseController::UpdateChokeHitBox()
{
    if (!_chokeHitbox || _hit)
        return;

    auto hitboxCollider = _chokeHitbox->GetCollider();
    hitboxCollider->SetActive(true);

    _chokeHitbox->GetTransform()->SetPosition(_transform->GetPosition()
        + _chokeHitbox->GetHitBox()->GetOffSet() + _transform->GetLook() * 5.0f);

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
            if (player && !player->GetIsInvincible())
            {
                _hit = true;
                player->_playerActive = false;
                player->startChoke = true;
                Vec3 d = direction.Cross(Vec3(0.0f, 1.0f, 0.0f));
                d.Normalize();
                player->fixedPos = d;
            }
        }
    }
}

void FinalBossMonsterSecondPhaseController::checkHit(shared_ptr<BaseCollider> hitboxCollider, float damage)
{
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
            {
                _hit = true;
                player->OnDamage(GetGameObject(), damage);
            }
        }
    }
}