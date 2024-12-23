#pragma once
#include "MonoBehaviour.h"
#include "EnginePch.h"
#include "CreatureController.h"
#include "MonsterController.h"
#include "Model.h"
#include "ModelRenderer.h"
#include "ModelAnimator.h"
#include "Bullet.h"

class Model;
class ModelRenderer;
class ModelAnimator;

/*
    각 몬스터의 개별 동작을 추가로 정의.
*/
class ShootingMonsterController : public MonsterController
{
    using Super = MonsterController;

    shared_ptr<Shader> renderShader = make_shared<Shader>(L"23. RenderDemo.fx");

    void Start() override;
    void Update() override;

public:
    MonoBehaviourType GetMonoBehaviourType() const override { return MonoBehaviourType::ShootingMonster; }

    shared_ptr<Model> GetEnemy() { return _enemy; }
    void SetEnemy(shared_ptr<Model> enemy) { _enemy = enemy; }
    shared_ptr<ModelRenderer> GetModelRenderer() { return _modelRenderer; }
    shared_ptr<ModelAnimator> GetModelAnimator() { return _modelAnimator; }
    void SetModelRenderer(shared_ptr<ModelRenderer> modelRenderer) { _modelRenderer = modelRenderer; }
    void SetModelAnimator(shared_ptr<ModelAnimator> modelAnimator) { _modelAnimator = modelAnimator; }
    void SetAnimationState(AnimationState state);

    bool GetHit() { return _hit; }
    void SetHit(bool hit) { _hit = hit; }

    void Aggro();
    void Move(Vec3 objPos, Vec3 targetPos, float speed);
    void Rota(Vec3 objPos, Vec3 targetPos);
    void Shoot();
    //void Tracking(Vec3 pos, const std::vector<Node3D>& path);
    void ResetToIdleState();
    void Patrol(Vec3 Target);
    void AddBullet(Vec3 Pos, Vec3 dir);

public:
    void OnDeath() override;

public:
    float distance;
    Vec3 CurForward;
    Vec3 EnemyPos;
    Vec3 PlayerPos;
    Vec3 patrolTarget;
    int atkType = 1;
    bool _isAnimating = false;

private:
    float _speed = 10.f;
    float _hp = 100.0f;
    float _atk = 30.0f;
    bool onTarget = false;
    bool onRange = false;
    bool BackToStart = false;
    float dt;
    float _FPS;
    Vec3 StartPos;
    float rangeDis;
    bool onAttack = false;
    float _attackDuration;
    float _aggroDuration;
    bool isFirstAggro = true;
    float animPlayingTime = 0.0f;
    bool hasPatrolTarget = false;
    Vec3 EnemyToPlayerdir;
    float EnemyToPlayerdistance;
    bool shootCount = true;

    // 히트 상태
    bool _hit = false;

    shared_ptr<Model> _enemy;
    shared_ptr<ModelRenderer> _modelRenderer;
    shared_ptr<ModelAnimator> _modelAnimator;
    shared_ptr<Transform> _transform;
    shared_ptr<GameObject> _player;
    AnimationState _currentAnimationState = AnimationState::Idle;
};

