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
    void SetHpBar(shared_ptr<GameObject> hpBar) { _hpBar = hpBar; }
    string GetObjID() { return objID; }
    void SetObjID(string str) { objID = str; }
    void SetHitBox(shared_ptr<GameObject> hitbox) { _hitbox = hitbox; }

    void ResetToIdleState();
    bool PlayCheckAnimating(AnimationState state);

    bool GetHit() { return _hit; }
    void SetHit(bool hit) { _hit = hit; }

    void Aggro();
    void Move(Vec3 objPos, Vec3 targetPos, float speed);
    void Rota(Vec3 objPos, Vec3 targetPos);
    void Patrol(Vec3 Target);
    void AddBullet(Vec3 Pos, Vec3 dir);

public:
    void OnDeath() override;
    void DropItem();
    bool PlayingHitMotion = false;
private:
    float _speed;

    float duration;
    float distance;
    Vec3 direction;
    float rangeDis;

    Vec3 playerPos;                     //플레이어 위치
    Vec3 CurForward;
    Vec3 EnemyPos;
    Vec3 PlayerPos;
    Vec3 patrolTarget;
    Vec3 StartPos;

    bool hasPatrolTarget = false;
    bool shootCount = false;
    bool isFirstTime = false;
    bool playingSound = false;

    bool BackToStart = false;
    bool chaseState = true;
    bool shootState = false;

    // 히트 상태
    bool _hit = false;
    bool _isDead = false;

    bool isPauseAfterPunch = false;
    float pauseEndTime = 0.0f;

    string objID;
    shared_ptr<Model> _enemy;
    shared_ptr<GameObject> _hitbox;
    shared_ptr<ModelRenderer> _modelRenderer;
    shared_ptr<ModelAnimator> _modelAnimator;
    shared_ptr<Transform> _transform;
    shared_ptr<GameObject> _player;
    shared_ptr<GameObject> _hpBar;
    AnimationState _currentAnimationState = AnimationState::Idle;
};

