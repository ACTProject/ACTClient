#include "pch.h"
#include "ShootingMonsterController.h"
#include <string>
#include "Bullet.h"

#define AggroRange 30.0f
#define ShootingRange 15.0f

void ShootingMonsterController::Move(Vec3 objPos, Vec3 targetPos, float speed)
{
    Vec3 direction = targetPos - objPos;
    if (direction.LengthSquared() < EPSILON) // EPSILON 사용
    {
        return;
    }

    direction.Normalize();  // 방향 벡터를 단위 벡터로 정규화
    direction.y = 0.f;

    _transform->SetPosition(_transform->GetPosition() + direction * speed * DT);  // 일정 거리만큼 이동
}

void ShootingMonsterController::Rota(Vec3 objPos, Vec3 targetPos)
{
    CurForward = _transform->GetLook();
    Vec3 direction = targetPos - objPos;
    direction.Normalize();

    // 외적을 이용한 회전 축 Mov계산
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

//void ShootingMonsterController::Tracking(Vec3 pos, const std::vector<Node3D>& path)
//{
//    if (path.empty()) {
//        return;
//    }
//
//    // 경로 상의 각 노드를 따라 이동
//    for (size_t i = 0; i < path.size(); ++i) {
//        // 현재 위치가 목표 노드에 도달했다면 다음 노드로 이동
//        if (i + 1 < path.size()) {
//            //Move(path[i + 1].pos);
//        }
//    }
//}

void ShootingMonsterController::AddBullet(Vec3 Pos, Vec3 dir)
{
    auto bullet = make_shared<GameObject>(); // bullet

    bullet->GetOrAddTransform()->SetPosition({ Pos.x, Pos.y + 3.f, Pos.z });
    bullet->GetOrAddTransform()->SetLocalRotation(dir); // XMConvertToRadians()
    bullet->GetOrAddTransform()->SetScale(Vec3(0.0001f));

    shared_ptr<Model> objModel = make_shared<Model>();
    // Model
    objModel->ReadModel(L"Enemy/bullet");
    objModel->ReadMaterial(L"Enemy/bullet");

    bullet->AddComponent(make_shared<ModelRenderer>(renderShader));
    {
        bullet->GetModelRenderer()->SetModel(objModel);
        bullet->GetModelRenderer()->SetPass(4);
    }

    // HitBox
    shared_ptr<GameObject> hitboxGO = make_shared<GameObject>();
    shared_ptr<HitBox> hitbox = make_shared<HitBox>();
    hitboxGO->AddComponent(hitbox);
    hitbox->SetOffSet(Vec3(0.f, 0.6f, 0.f));
    hitbox->Craete(bullet, Vec3(0.05f));
    CUR_SCENE->Add(hitboxGO);

    shared_ptr<Bullet> bulletComponent = make_shared<Bullet>();
    bulletComponent->Add(objModel);
    bulletComponent->SetSpeed(50.0f);
    bulletComponent->SetDirection(dir);
    bulletComponent->SetHitBox(hitboxGO);
    bullet->AddComponent(bulletComponent);

    CUR_SCENE->Add(bullet);
}

void ShootingMonsterController::Aggro()
{
    if (PlayCheckAnimating(AnimationState::Aggro))
    {
        return;
    }
    isFirstTime = true;
    _hpBar->SetActive(true);
}

void ShootingMonsterController::Patrol(Vec3 Target)
{
    Move(EnemyPos, Target, _speed / 2.f);
    Rota(EnemyPos, Target);
}

void ShootingMonsterController::Start()
{
    Super::Start();

    // 원거리 몬스터 스탯 초기화
    _maxHp = 50.0f;
    _hp = 50.0f;
    _atk = 25.0f;
    _speed = 8.0f;

    _transform = GetTransform();
    StartPos = _transform->GetPosition();
    patrolTarget = StartPos;

    std::cout << "ShootingMonsterController Start()" << std::endl;
}

void ShootingMonsterController::Update()
{
    Super::Update();

    if (_isDead)
    {
        if (PlayCheckAnimating(AnimationState::Die))
        {
            return;
        }

        Super::OnDeath();
        std::cout << "Shooting Monster Died!" << std::endl;

        return;
    }

    _FPS = static_cast<float>(TIME->GetFps());
    // 플레이어 위치 계산
    _player = SCENE->GetCurrentScene()->GetPlayer();
    PlayerPos = _player->GetTransform()->GetPosition();
    EnemyPos = _transform->GetPosition();

    direction = PlayerPos - EnemyPos;
    distance = direction.Length();
    rangeDis = (EnemyPos - StartPos).Length();

    static float lastPatrolTime = 0.0f; // 마지막 목표 생성 시간
    float currentTime = TIME->GetGameTime(); // 현재 게임 시간

    if (_hp < 0.f)
    {
        animPlayingTime += DT;
        SetAnimationState(AnimationState::Die);
        if (animPlayingTime >= (_enemy->GetAnimationDuration(AnimationState::Die) / _FPS))
        {
            Remove(GetGameObject());
        }
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
        _hp = 50.0f;            // 돌아가면 체력 회복
        if (abs(rangeDis) < 1.f)
        {
            BackToStart = false;
            chaseState = true;
            isFirstTime = false;
            _hpBar->SetActive(false);
        }
        return;
    }

    if (distance <= AggroRange)
    {
        chaseState = false;
    } // 탐지 범위 안에 있을 때


    // 상태별 애니메이션 실행
    if (!isFirstTime && !chaseState)
    {
        Aggro();
        return;
    }

    if (!chaseState)
    {
        Rota(EnemyPos, PlayerPos);
        if (distance < ShootingRange)
        {
            shootState = true;
        }
        if (shootState)
        {
            if (PlayCheckAnimating(AnimationState::Attack1))
            {
                if (!shootCount && animPlayingTime >= duration / 2.0f)
                {
                    AddBullet(EnemyPos, direction);
                    shootCount = true;
                }
                return;
            }
            shootCount = false;
            shootState = false;
        }
        else
        {
            int randomHit = rand() % 3;
            if (PlayingHitMotion && randomHit == 0)
            {
                if (PlayCheckAnimating(AnimationState::Hit1))
                {
                    return;
                }
            }
            PlayingHitMotion = false;
            Move(EnemyPos, PlayerPos, _speed);
        }
    }
    else
    {
        if (currentTime - lastPatrolTime > 2.f) // 3~6초 간격
        {// 새로운 랜덤 목표 지점 생성
            Patrol(patrolTarget);
            if (sqrt(powf(EnemyPos.x - patrolTarget.x, 2) + powf(EnemyPos.z - patrolTarget.z, 2)) < 1.f)
            {
                lastPatrolTime = currentTime;
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
}

void ShootingMonsterController::SetAnimationState(AnimationState state)
{
    _modelAnimator->ChangeAnimation(state);
    _currentAnimationState = state;
}

void ShootingMonsterController::ResetToIdleState() {
    SetAnimationState(AnimationState::Idle);
}

void ShootingMonsterController::OnDeath()
{
    _isDead = true;
    animPlayingTime = 0.0f;
}

bool ShootingMonsterController::PlayCheckAnimating(AnimationState state)
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
