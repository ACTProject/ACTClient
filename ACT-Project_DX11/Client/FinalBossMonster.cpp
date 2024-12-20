#include "pch.h"
#include "FinalBossMonster.h"

#define AttackRange 5.0f

void FinalBossMonster::SetAnimationState(AnimationState state)
{
    _modelAnimator->ChangeAnimation(state);
    _currentAnimationState = state;
}

void FinalBossMonster::Start()
{
    _transform = GetTransform();
    _player = SCENE->GetCurrentScene()->GetPlayer();
    SetAnimationState(AnimationState::Idle);
    comboCnt = 1;
    patternCnt = 1;
}

void FinalBossMonster::Update()
{
    if (INPUT->GetButton(KEY_TYPE::KEY_4))
    {
        int a = 0;
    }

    dt = DT;
    _FPS = static_cast<float>(TIME->GetFps());
    bossPos = _transform->GetPosition();

    playerPos = _player->GetTransform()->GetPosition();
    currentTime = TIME->GetGameTime(); // 현재 게임 시간

    direction = bossPos - playerPos;
    distance = direction.Length();

    if (myPhase == 1)
    {
        if (distance < AttackRange)
        {
            punchState = true;
        }
        else
        {
            chaseState = true;
        }
        Phase_1();
    }
    else if (myPhase == 2)
    {
        Phase_2();
    }
}



void FinalBossMonster::Phase_1()
{
    if (!isFirstTime) // 조우 ( 편의상 안되게 해놨음 )
    {
        if (currentTime > 8.f) // 실행되는데 걸리는 시간으로 인한 애니메이션이 짤리는 현상때문에 설정
        {
            Appear();
            return;
        }
        else
        {
            return;
        }
    }

    if (currentTime - lastTime > 2.0f)
    {
        postpone = true;
    }

    DEBUG->LogVec3ToConsole({ (float)patternCnt , 0, 0 }, "patternCnt");
    if (postpone)
    {
        if (patternCnt < 4)
        {
            if (punchState)
            {
                if (PlayCheckAnimating(static_cast<AnimationState>((int)AnimationState::Attack1 + randType)))
                {
                    Punch();
                    return;
                }
                else
                {
                    randType = rand() % 4;
                    patternCnt++;
                    punchState = false;
                }
            }
            else if (chaseState)
            {
                Walk(bossPos, playerPos, 5.0f);
                Rota(bossPos, playerPos);
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
                lastTime = currentTime;
                shootTime = 0.0f;
                patternCnt = 1;
                SetAnimationState(AnimationState::Combat);
            }
        }
    }
    else
    {
        Rota(bossPos, playerPos);
        SetAnimationState(AnimationState::Combat);
    }
}

void FinalBossMonster::Phase_2()
{
}

void FinalBossMonster::Appear()
{
    if (PlayCheckAnimating(AnimationState::Appear))
    {
        return;
    }
    isFirstTime = false;
    lastTime = currentTime;
}

void FinalBossMonster::Walk(Vec3 objPos, Vec3 targetPos, float speed)
{
    SetAnimationState(AnimationState::Walk);
    //CREATURE->Move(objPos, targetPos, speed);
    Vec3 direction = targetPos - objPos;
    if (direction.LengthSquared() < 5.f) // EPSILON 사용
    {
        SetAnimationState(AnimationState::Combat);
        return;
    }

    direction.Normalize();  // 방향 벡터를 단위 벡터로 정규화

    _transform->SetPosition(_transform->GetPosition() + direction * speed * dt);  // 일정 거리만큼 이동
}

void FinalBossMonster::Rota(Vec3 objPos, Vec3 targetPos)
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

void FinalBossMonster::Die()
{
    CUR_SCENE->Remove(GetGameObject());
    OCTREE->RemoveCollider(GetGameObject()->GetCollider());
    return;
}

void FinalBossMonster::Punch()
{
    
}

void FinalBossMonster::Fireball()
{
    Rota(bossPos, playerPos);
    shootTime += dt;
    float startTime = 208 / 60 - 0.5;
    float endTime = 208 / 60 - 0.3;
    if ((shootTime <= endTime) && (shootTime > startTime))
    {
        float randX = rand() % 10 / 5.f; // 0 ~ 1.8
        float randY = rand() % 10 / 5.f;
        float randZ = rand() % 10 / 5.f;
        makeBubble({ bossPos.x + randX, bossPos.y + randY , bossPos.z + randZ }, playerPos - bossPos);
    }
}

void FinalBossMonster::makeBubble(Vec3 pos, Vec3 dir)
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
        bullet->GetModelRenderer()->SetPass(1);
    }

    //// Collider
    //auto collider = make_shared<AABBBoxCollider>();
    //collider->SetBoundingBox(BoundingBox(Vec3(0.f), Vec3(0.5f)));
    //collider->SetOffset(Vec3(0.f, 0.1f, 0.f));
    //OCTREE->InsertCollider(collider);
    //bullet->AddComponent(collider);

    //shared_ptr<Rigidbody> rigidBody = make_shared<Rigidbody>();
    //rigidBody->SetUseGravity(false);
    //rigidBody->SetMass(0.1f);
    //bullet->AddComponent(rigidBody);

    shared_ptr<Bullet> bulletComponent = make_shared<Bullet>();
    bulletComponent->Add(objModel);
    bullet->AddComponent(bulletComponent);

    // HitBox
    shared_ptr<GameObject> hitboxGO = make_shared<GameObject>();
    shared_ptr<HitBox> hitbox = make_shared<HitBox>();
    hitboxGO->AddComponent(hitbox);
    hitbox->SetOffSet(Vec3(0.f, 0.6f, 0.f));
    hitbox->Craete(bullet, Vec3(0.3f));
    CUR_SCENE->Add(hitboxGO);

    /*COLLISION->AddRigidbody(rigidBody);
    COLLISION->AddCollider(collider);*/

    CUR_SCENE->Add(bullet);
}


bool FinalBossMonster::PlayCheckAnimating(AnimationState state)
{
    SetAnimationState(state);

    animPlayingTime += dt;
    float duration = _enemy->GetAnimationDuration(state) / _FPS;

    if (animPlayingTime >= duration)
    {
        animPlayingTime = 0.0f;
        SetAnimationState(AnimationState::Combat);
        return false;
    }

    // 코루틴 실행
    MyCoroutine attackCoroutine = EnemyCoroutine(this, duration);
    currentEnemyCoroutine = attackCoroutine.GetHandler();
    currentEnemyCoroutine.resume();

    return true; // 플레이 중
}
