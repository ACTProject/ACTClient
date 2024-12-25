#include "pch.h"
#include "FinalBossMonsterSecondPhaseController.h"
#include "PlayerController.h"

#define AttackRange 7.0f

void FinalBossMonsterSecondPhaseController::SetAnimationState(AnimationState state)
{
    _modelAnimator->ChangeAnimation(state);
    _currentAnimationState = state;
}

void FinalBossMonsterSecondPhaseController::ResetToIdleState()
{
    SetAnimationState(AnimationState::Combat);
}

bool FinalBossMonsterSecondPhaseController::PlayCheckAnimating(AnimationState state)
{
    SetAnimationState(state);

    animPlayingTime += DT;
    float duration = _enemy->GetAnimationDuration(state) / _FPS;

    if (animPlayingTime >= duration)
    {
        animPlayingTime = 0.0f;
        ResetToIdleState();
        return false;
    }

    /*MyCoroutine attackCoroutine = EnemyCoroutine(this, duration);
    currentEnemyCoroutine = attackCoroutine.GetHandler();
    currentEnemyCoroutine.resume();*/

    return true; // 플레이 중
}

void FinalBossMonsterSecondPhaseController::Start()
{
    Super::Start();

    SetAnimationState(AnimationState::GetUP2);
    _maxHp = 500.f;
    _hp = 500.0f;
    _atk = 50.0f;
    speed = 10.0f;
    _transform = GetTransform();
    _player = SCENE->GetCurrentScene()->GetPlayer();
    randPunchType = rand() % 4;
    randType = rand() % 10;

    shared_ptr<ModelBone> rightHand = _enemy->GetBoneByName(L"mixamorig:RightHand");
}

void FinalBossMonsterSecondPhaseController::Update()
{
    Super::Update();
    currentTime = TIME->GetGameTime(); // 현재 게임 시간

    _transform = GetTransform();
    _player = SCENE->GetCurrentScene()->GetPlayer();
    if (INPUT->GetButton(KEY_TYPE::KEY_4))
    {
        int a = 0;
    }

    if (_isDead)
    {
        if (PlayCheckAnimating(AnimationState::Down2))
        {
            return;
        }
        Super::OnDeath();
        std::cout << "FinalBoss has been defeated! Game Over!" << std::endl;
        return;
    }

    _FPS = static_cast<float>(TIME->GetFps());

    bossPos = _transform->GetPosition();
    playerPos = _player->GetTransform()->GetPosition();

    direction = bossPos - playerPos;
    distance = direction.Length();

    Rota(bossPos, playerPos);

    Phase_2();
}

void FinalBossMonsterSecondPhaseController::Phase_2()
{
    if (!isFirstTime) // 2페이즈 시작
    {
        if (!Phase2Flag) // 한번만 실행
        {
            _hp = 500.0f;
            DEBUG->Log(L"Boss 2nd Phase Start");
            Phase2Flag = true;
        }
        if (!isExecuted)
        {
            if (PlayCheckAnimating(AnimationState::GetUP2))
            {
                return;
            }
            isExecuted = true;
        }
        Rota(bossPos, playerPos);
        if (PlayCheckAnimating(AnimationState::Roar))
        {
            return;
        }
        isFirstTime = true; // 플래그
        randType = 1;
    }

    switch (randType)
    {
    case 1: // 펀치 콤보
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
                if (!punchExecuted)
                {
                    punchState = true;
                    randType = 1;
                    punchExecuted = true;
                }
                else
                {
                    punchState = false;
                    randType = 0;
                    punchExecuted = false;
                }
            }
        }
        else
        {
            Sprint();
            Rota(bossPos, playerPos);
        }
        break;
    case 2: // 초크
        Move(bossPos, playerPos, speed);
        Rota(bossPos, playerPos);
        if (distance < 5.f)
        {
            attackState = true;
        }
        if (attackState)
        {
            if (PlayCheckAnimating(AnimationState::Skill1))
            {
                Choke_lift();
                return;
            }
            else
            {
                attackState = false;
                randType = rand() % 10;
            }
        }
        break;
    case 3: // 버블 발사
        if (PlayCheckAnimating(AnimationState::Skill2))
        {
            Fireball();
            return;
        }
        else
        {
            shootTime = 0.0f;
            randType = rand() % 10;
        }
        break;
    case 4: // 돈다발 발사
        if (PlayCheckAnimating(AnimationState::Skill3))
        {
            FireMoney();
            return;
        }
        else
        {
            shootState = false;
            shootTime = 0.0f;
            randType = rand() % 10;
        }
        break;
    case 5: // 큰 펀치(slash)
        Move(bossPos, playerPos, speed);
        Rota(bossPos, playerPos);
        if (distance < AttackRange)
        {
            punchState = true;
        }
        if (punchState)
        {
            if (PlayCheckAnimating(AnimationState::Skill5))
            {
                Punch(); // 히트 및 데미지 처리
                return;
            }
            else
            {
                punchState = false;
                randType = rand() % 10;
            }
        }
        break;
    case 6: // 어퍼컷
        Move(bossPos, playerPos, speed);
        Rota(bossPos, playerPos);
        if (distance < AttackRange)
        {
            punchState = true;
        }
        if (punchState)
        {
            if (PlayCheckAnimating(AnimationState::Skill6))
            {
                Punch(); // 히트 및 데미지 처리
                return;
            }
            else
            {
                punchState = false;
                randType = rand() % 10;
            }
        }
        break;
    case 7: // 깔아 뭉게기
        Move(bossPos, playerPos, speed);
        Rota(bossPos, playerPos);
        if (distance < 10.0f)
        {
            punchState = true;
        }
        if (punchState)
        {
            if (!isExecuted_2)
            {
                lastPos = playerPos;
                isExecuted_2 = true;
            }
            if (PlayCheckAnimating(AnimationState::Skill7))
            {
                Punch(); // 히트 및 데미지 처리
                return;
            }
            else
            {
                _transform->SetLocalPosition(lastPos);
                punchState = false;
                randType = rand() % 10;
            }
        }
        break;
    case 8: // 업어치기
        if (PlayCheckAnimating(AnimationState::Skill8))
        {
            GrabSlam();
            return;
        }
        else
        {
            randType = rand() % 10;
        }
        break;
    case 9: // 허리케인
        if (PlayCheckAnimating(AnimationState::Skill9))
        {
            Hurricane();
            return;
        }
        else
        {
            randType = rand() % 10;
        }
        break;

    }

}

void FinalBossMonsterSecondPhaseController::Appear()
{
    if (PlayCheckAnimating(AnimationState::Appear))
    {
        return;
    }
    isFirstTime = false;
    lastTime = currentTime;
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

    _transform->SetPosition(_transform->GetPosition() + dir * 10.0f * DT);  // 일정 거리만큼 이동

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
    UpdateHitBox();
    if (_hit && !hasDealing)
    {
        auto player = dynamic_pointer_cast<PlayerController>(_player->GetController());
        player->OnDamage(_atk);
        hasDealing = true;
    }
}

void FinalBossMonsterSecondPhaseController::Fireball()
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
    }
}

void FinalBossMonsterSecondPhaseController::FireMoney()
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

        shootState = true;
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
    bulletComponent->SetDirection(dir);
    bulletComponent->SetSpeed(30.0f);
    bulletComponent->Add(objModel);

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

void FinalBossMonsterSecondPhaseController::makeCash(Vec3 pos, Vec3 dir)
{
    auto bullet = make_shared<GameObject>(); // bullet

    bullet->GetOrAddTransform()->SetPosition({ pos.x, pos.y + 3.f, pos.z });
    bullet->GetOrAddTransform()->SetLocalRotation({ 0,0,0 }); // XMConvertToRadians()
    bullet->GetOrAddTransform()->SetScale(Vec3(0.003f));

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
    hitbox->Craete(bullet, Vec3(0.005f));
    CUR_SCENE->Add(hitboxGO);
    bulletComponent->SetHitBox(hitboxGO);
    bullet->AddComponent(bulletComponent);

    CUR_SCENE->Add(bullet);
}

void FinalBossMonsterSecondPhaseController::Choke_lift()
{

}

void FinalBossMonsterSecondPhaseController::GrabSlam()
{
    //vector<AnimTransform> animTransforms = _modelAnimator->GetTransform()->

    Matrix worldTransform = CalculateWorldTransform(rightHand);

    Vec3 handPos = { worldTransform._41, worldTransform._42, worldTransform._43 };
}

Matrix FinalBossMonsterSecondPhaseController::CalculateWorldTransform(shared_ptr<ModelBone> bone) {
    if (!bone) return Matrix::Identity; // 기본값 반환

    // 부모 본의 월드 변환 행렬을 재귀적으로 계산
    Matrix parentWorld = Matrix::Identity;
    if (bone->parent) {
        parentWorld = CalculateWorldTransform(bone->parent);
    }

    // 로컬 변환과 부모의 월드 변환을 곱하여 월드 변환 계산
    return bone->transform * parentWorld;
}

void FinalBossMonsterSecondPhaseController::Hurricane()
{
    Vec3 direction = playerPos - bossPos;
    if (direction.LengthSquared() < 5.f) // EPSILON 사용
    {
        return;
    }

    direction.Normalize();  // 방향 벡터를 단위 벡터로 정규화

    _transform->SetPosition(_transform->GetPosition() + direction * 8.0f * DT);  // 일정 거리만큼 이동
}

void FinalBossMonsterSecondPhaseController::OnDeath()
{
    _isDead = true;
    animPlayingTime = 0.0f;
}

void FinalBossMonsterSecondPhaseController::UpdateHitBox()
{
    if (_hit)
        return;

    auto hitboxCollider = _hitbox->GetCollider();
    hitboxCollider->SetActive(true);

    _hitbox->GetTransform()->SetPosition(_transform->GetPosition()
        + _hitbox->GetHitBox()->GetOffSet() + _transform->GetLook() * 5.0f);

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

void FinalBossMonsterSecondPhaseController::ResetHit()
{
    hasDealing = false;
    _hit = false;
    _hitbox->GetCollider()->SetActive(false);
}

