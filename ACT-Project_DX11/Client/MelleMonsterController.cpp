#include "pch.h"
#include "MelleMonsterController.h"
#include "PlayerController.h"

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

//void MelleMonsterController::Tracking(Vec3 pos, const std::vector<Node3D>& path)
//{
//	if (path.empty()) {
//		return;
//	}
//
//	// 경로 상의 각 노드를 따라 이동
//	for (size_t i = 0; i < path.size(); ++i) {
//		// 현재 위치가 목표 노드에 도달했다면 다음 노드로 이동
//		if (i + 1 < path.size()) {
//			//Move(path[i + 1].pos);
//		}
//	}
//}

void MelleMonsterController::Punch(int type)
{
    if (animPlayingTime >= animDuration / 2.0f)
    {
        UpdateHitBox();
    }
    if (_hit && !hasDealing)
    {
        auto player = dynamic_pointer_cast<PlayerController>(_player->GetController());
        player->OnDamage(_atk);
        hasDealing = true;
    }
}

void MelleMonsterController::Aggro()
{
    if (PlayCheckAnimating(AnimationState::Aggro))
    {
        return;
    }
    isFirstTime = true;
}

void MelleMonsterController::Start()
{
    Super::Start();

    // 근접 몬스터 스탯 초기화
    _maxHp = 80.0f;
    _hp = 80.0f;
    _atk = 15.0f;
    _speed = 8.0f;

    _transform = GetTransform();
    StartPos = _transform->GetPosition();
    patrolTarget = StartPos;

    std::cout << "MelleMonsterController Start()" << std::endl;
}

void MelleMonsterController::Update()
{
    Super::Update();

    // 플레이어 위치 계산4
    _player = SCENE->GetCurrentScene()->GetPlayer();
    PlayerPos = _player->GetTransform()->GetPosition();
    EnemyPos = _transform->GetPosition();

    static float lastPatrolTime = 0.0f; // 마지막 목표 생성 시간
    float currentTime = TIME->GetGameTime(); // 현재 게임 시간

    direction = PlayerPos - EnemyPos;
    distance = direction.Length();
    rangeDis = (EnemyPos - StartPos).Length();

    if (_hp <= 0.0f)
    {
        if (PlayCheckAnimating(AnimationState::Die))
        {
            return;
        }
        Super::OnDeath();
        std::cout << "Melle Monster Died!" << std::endl;

        return;
    }

    if (INPUT->GetButton(KEY_TYPE::KEY_4))
    {
        int a = 0;
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
        _hp = 80.0f;            // 돌아가면 체력 회복
        if (abs(rangeDis) < 1.f)
        {
            BackToStart = false;
            chaseState = true;
            isFirstTime = false;
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
        if (distance < AttackRange)
        {
            punchState = true;
        }
        if(punchState)
        {
            if (PlayCheckAnimating(static_cast<AnimationState>((int)AnimationState::Attack1 + atkType)))
            {
                Punch(atkType);
                return;
            }
            else
            {
                atkType = rand() % 3;
                punchState = false;
                ResetHit();
            }
        }
        else
        {
            if (PlayingHitMotion && rand() % 3  == 0)
            {
                if (PlayCheckAnimating(AnimationState::Hit1))
                {
                    return;
                }
                PlayingHitMotion = false;
            }
            Rota(EnemyPos, PlayerPos);
            Move(EnemyPos, PlayerPos, _speed);
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
            if (!isExecuted_1)
            {
                ResetToIdleState();
                isExecuted_1 = true;
            }
            float radius = 5.f; // 배회 반경
            float randomX = StartPos.x + (rand() % 2000 / 1000.0f - 1.0f) * radius;
            float randomZ = StartPos.z + (rand() % 2000 / 1000.0f - 1.0f) * radius;
            patrolTarget = Vec3(randomX, EnemyPos.y, randomZ);
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
                player->OnDamage(_atk);
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
    hasDealing = false;
    _hit = false;
    _hitbox->GetCollider()->SetActive(false);
}