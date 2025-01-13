#include "pch.h"
#include "MelleMonsterController.h"
#include "PlayerController.h"
#include "Particle.h"
#include "Camera.h"
#include "MathUtils.h"

#define AggroRange 30.0f
#define AttackRange 5.0f

void MelleMonsterController::Move(Vec3 objPos, Vec3 targetPos, float speed)
{
    SetAnimationState(AnimationState::Run);
    Vec3 direction = targetPos - objPos;
    if (direction.LengthSquared() < EPSILON) // EPSILON 사용
    {
        return;
    }

    direction.Normalize();  // 방향 벡터를 단위 벡터로 정규화

    _transform->SetPosition(_transform->GetPosition() + direction * speed * DT);  // 일정 거리만큼 이동
}

void MelleMonsterController::Rota(Vec3 objPos, Vec3 targetPos)
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

void MelleMonsterController::Punch(int type)
{
    if (animPlayingTime >= animDuration / 1.4f)
    {
        if (!playingSound)
        {
            wstring s = L"melle_swing" + std::to_wstring(type);
            SOUND->PlayEffect(s);
            playingSound = true;
        }
        if (type < 2)
            type = 1;
        else
            type = 2;
        if (_bubble)
        {
            CreateBubbleEffect(15, Vec3(3.f, 1.f, 1.f), 1.f, 3.f);
            _bubble = false;
        }
        
        UpdateHitBox();
        
    }
}

void MelleMonsterController::Aggro()
{
    if (!playingSound)
    {
        SOUND->PlayEffect(L"melle_aggro");
        playingSound = true;
    }
    if (PlayCheckAnimating(AnimationState::Aggro))
    {
        return;
    }
    isFirstTime = true;
    playingSound = false;
    _hpBar->SetActive(true);
}

void MelleMonsterController::Start()
{
    Super::Start();

    // 근접 몬스터 스탯 초기화
    _maxHp = 100.0f;
    _hp = 100.0f;
    _atk = 25.0f;
    _speed = 8.0f;

    _transform = GetTransform();
    StartPos = _transform->GetPosition();
    patrolTarget = StartPos;
    _player = SCENE->GetCurrentScene()->GetPlayer();
    CreateEffect();

    std::cout << "MelleMonsterController [" << objID << "] Start()" << std::endl;
}

void MelleMonsterController::Update()
{
    Super::Update();


    if (CUR_SCENE->GetMainCamera()->GetCamera()->IsCutSceneActive() == true)
    {
        return;
    }

    // 플레이어 위치 계산
    playerPos = _player->GetTransform()->GetPosition();
    EnemyPos = _transform->GetPosition();

    static float lastPatrolTime = 0.0f; // 마지막 목표 생성 시간

    direction = playerPos - EnemyPos;
    distance = direction.Length();
    rangeDis = (EnemyPos - StartPos).Length();

    if (_isDead)
    {
        if (!isDrop)
            DropItem();

        // 공격자로 부터 밀리는 로직
        Vec3 knockbackDirection = EnemyPos - playerPos;                  // 공격자 -> 몬스터 방향
        knockbackDirection.Normalize();                                  // 방향 벡터 정규화

        float knockbackForce = 150.0f;                                   // 밀리는 힘
        GetGameObject()->GetRigidbody()->Addforce(knockbackDirection * knockbackForce);       // 힘 적용

        if (!playingSound)
        {
            SOUND->PlayEffect(L"melle_die");
            playingSound = true;
        }
        if (PlayCheckAnimating(AnimationState::Die))
        {
            return;
        }
        playingSound = false;

        CUR_SCENE->Remove(_hpBar);
        std::cout << "Melle Monster [" << objID << "] Died!" << std::endl;
        _hpBar->Destroy();
        
        Super::OnDeath();

        return;
    }

    // 범위 검사
    if (rangeDis > 50.f) // 초기 위치에서 너무 멀리 떨어지면 복귀
    {
        BackToStart = true;
    }

    if (BackToStart)
    {
        Move(EnemyPos, StartPos, _speed);
        Rota(EnemyPos, StartPos);
        if (abs(rangeDis) < 1.f)
        {
            _hp = 80.0f;            // 돌아가면 체력 회복
            BackToStart = false;
            chaseState = true;
            isFirstTime = false;
            _hpBar->SetActive(false);
        }
        return;
    }

    if (distance <= AggroRange)
    {
        _hpBar->SetActive(true);
        chaseState = false;
    } // 탐지 범위 안에 있을 때


    // 상태별 애니메이션 실행
    if (!isFirstTime && !chaseState)
    {
        Aggro();
        return;
    }

    if (isPauseAfterPunch) // 멈춤 상태 처리
    {
        if (currentTime >= pauseEndTime) // 멈춤 시간이 끝났는지 확인
        {
            isPauseAfterPunch = false; // 멈춤 상태 해제
        }
        else
        {
            isPauseAfterPunch = true; // 멈춤 상태에서는 다른 동작 수행하지 않음
        }
    }

    if (isPauseAfterPunch)
    {
        if (PlayingHitMotion)
        {
            if (PlayCheckAnimating(AnimationState::Hit1))
            {
                if (!playingSound)
                {
                    SOUND->PlayEffect(L"melle_hit");
                    playingSound = true;
                }
                return;
            }
            playingSound = false;
            PlayingHitMotion = false;
            pauseEndTime = currentTime + 0.5f;
        }
    }

    if (!chaseState && !isPauseAfterPunch)
    {
        if (distance < AttackRange)
        {
            punchState = true;
        }

        if (punchState)
        {
            if (PlayCheckAnimating(static_cast<AnimationState>((int)AnimationState::Attack1 + atkType)))
            {
                Punch(atkType);
                return;
            }
            else
            {
                isPauseAfterPunch = true;
                pauseEndTime = currentTime + 1.0f;
                atkType = rand() % 3;
                playingSound = false;
                punchState = false;
                _bubble = true;
                ResetHit();
            }
        }
        else
        {
            Rota(EnemyPos, playerPos);
            Move(EnemyPos, playerPos, _speed);
        }
    }
    else
    {
        if (currentTime - lastPatrolTime > 2.f) // 3~6초 간격
        {
            Move(EnemyPos, patrolTarget, _speed / 2.f);
            Rota(EnemyPos, patrolTarget);
            if (sqrt(powf(EnemyPos.x - patrolTarget.x, 2) + powf(EnemyPos.z - patrolTarget.z, 2)) < 1.f)
            {
                lastPatrolTime = currentTime;
                isExecuted_1 = false;
            }
        }
        else
        {
            ResetToIdleState();
            float radius = 5.f; // 배회 반경
            float randomX = StartPos.x + (rand() % 2000 / 1000.0f - 1.0f) * radius;
            float randomZ = StartPos.z + (rand() % 2000 / 1000.0f - 1.0f) * radius;
            patrolTarget = Vec3(randomX, EnemyPos.y, randomZ);
        }
    }
    if (_hpBar->IsActive())
    {
        if (distance >= AggroRange)
        {
            _hpBar->SetActive(false);
        }
        auto camera = CUR_SCENE->GetMainCamera()->GetCamera();
        Vec3 cameraPos = camera->GetCameraPosition();
        Vec3 monsterPos = _transform->GetPosition();
        Vec3 toMonster = monsterPos - cameraPos;
        toMonster.Normalize();
        // 카메라의 전방 벡터 (Forward Vector)
        Vec3 cameraForward = camera->GetForward();
        cameraForward.Normalize();

        // 몬스터가 카메라 뒤에 있는지 확인
        float dotProduct = cameraForward.Dot(toMonster);
        if (dotProduct < 0.0f) // 내적이 음수면 카메라 뒤에 있음
        {
            _hpBar->SetActive(false);
        }
    }

}

void MelleMonsterController::UpdateHitBox()
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
                player->OnDamage(GetGameObject(), _atk);
            _hit = true;
        }
    }
}

bool MelleMonsterController::PlayCheckAnimating(AnimationState state)
{
    SetAnimationState(state);

    animPlayingTime += DT;
    animDuration = _enemy->GetAnimationDuration(state) / _FPS;

    if (animPlayingTime >= animDuration)
    {
        animPlayingTime = 0.0f;
        ResetToIdleState();
        return false;
    }

    return true; // 플레이 중
}

void MelleMonsterController::SetAnimationState(AnimationState state)
{
    _modelAnimator->ChangeAnimation(state);
    _currentAnimationState = state;
}

void MelleMonsterController::ResetToIdleState()
{
    SetAnimationState(AnimationState::Idle);
}

void MelleMonsterController::ResetHit()
{
    _hit = false;
    _hitbox->GetCollider()->SetActive(false);
}

void MelleMonsterController::OnDeath()
{
    _isDead = true;
    animPlayingTime = 0.0f;
}

void MelleMonsterController::DropItem()
{
    auto item = make_shared<GameObject>();
    item->SetObjectType(ObjectType::Spoils);
    item->GetOrAddTransform()->SetPosition(EnemyPos);
    item->GetOrAddTransform()->SetLocalRotation(Vec3(XMConvertToRadians(90),0,0));
    item->GetOrAddTransform()->SetScale(Vec3(0.05f));

    std::cout << "item drop" << std::endl;
    shared_ptr<Model> objModel = make_shared<Model>();
    // Model
    objModel->ReadModel(L"Enemy/can");
    objModel->ReadMaterial(L"Enemy/can");

    item->AddComponent(make_shared<ModelRenderer>(renderShader));
    {
        item->GetModelRenderer()->SetModel(objModel);
        item->GetModelRenderer()->SetPass(7);
    }

    auto collider = make_shared<AABBBoxCollider>();
    collider->SetBoundingBox(BoundingBox(Vec3(0.f), Vec3(0.5f)));
    collider->SetOffset(Vec3(0.f, 0.5f, 0.f));
    OCTREE->InsertCollider(collider);
    item->AddComponent(collider);

    isDrop = true;

    CUR_SCENE->Add(item);
}

void MelleMonsterController::CreateEffect()
{
    auto effectObj = make_shared<GameObject>();

    _effectObj = effectObj;
    _effectObj->GetOrAddTransform()->SetLocalPosition(Vec3(0.f));
    _effectObj->AddComponent(make_shared<Particle>());
    {
        auto shader = make_shared<Shader>(L"MonsterEffect.fx");
        shared_ptr<Material> material = make_shared<Material>();
        material->SetShader(shader);
        auto texture = RESOURCES->Load<Texture>(L"MelleAttack1", L"..\\Resources\\Textures\\Effect\\MelleEffect1.png");
        material->SetDiffuseMap(texture);

        MaterialDesc& desc = material->GetMaterialDesc();
        desc.ambient = Vec4(1.f);
        desc.diffuse = Vec4(1.f);
        desc.specular = Vec4(1.f);

        RESOURCES->Add(L"MelleAttack1", material);
        _effectObj->GetParticle()->SetMaterial(material);
    }
    {
        auto shader = make_shared<Shader>(L"Bubble.fx");
        shared_ptr<Material> material = make_shared<Material>();
        material->SetShader(shader);
        auto texture = RESOURCES->Load<Texture>(L"Bubble", L"..\\Resources\\Textures\\Effect\\bubble.png");
        material->SetDiffuseMap(texture);

        MaterialDesc& desc = material->GetMaterialDesc();
        desc.ambient = Vec4(1.f);
        desc.diffuse = Vec4(1.f);
        desc.specular = Vec4(1.f);

        RESOURCES->Add(L"Bubble", material);
        _bubbleMaterial = material;
        _effectObj->GetParticle()->SetMaterial(material);
    }
    
    _effectObj->GetParticle()->SetDelayTime(0.f);
    _effectObj->GetParticle()->SetLifetime(0.3f);
    _effectObj->GetParticle()->SetfadeStart(0.0f);
    _effectObj->GetParticle()->SetReuse(true);
    _effectObj->GetParticle()->Add(Vec3(0.f), Vec2(3.0f, 3.0f));
    CUR_SCENE->Add(_effectObj);
}

void MelleMonsterController::ActiveEffect()
{
    Vec3 effectTransform = _transform->GetPosition();
    effectTransform += _transform->GetLook() * 4.f;
    effectTransform.y += 1.5f;
   
    _effectObj->GetOrAddTransform()->SetPosition(effectTransform);
    _effectObj->GetParticle()->SetElapsedTime(0.0f);
}

void MelleMonsterController::CreateBubbleEffect(int numBubbles, Vec3 bubbleSpread, float positionY, float positionLook)
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
