#pragma once
#include "MonoBehaviour.h"
#include "EnginePch.h"
#include "CreatureController.h"
#include "MonsterController.h"
#include "Model.h"
#include "ModelAnimator.h"
#include "UI.h"

class Model;
class ModelAnimator;
/*
    각 몬스터의 개별 동작을 추가로 정의.
*/
class MelleMonsterController : public MonsterController
{
    using Super = MonsterController;

	void Start() override;
	void Update() override;

public:
    MonoBehaviourType GetMonoBehaviourType() const override { return MonoBehaviourType::MelleMonster; }

	shared_ptr<Model> GetEnemy() { return _enemy; }
	void SetEnemy(shared_ptr<Model> enemy) { _enemy = enemy; }
	shared_ptr<ModelAnimator> GetModelAnimator() { return _modelAnimator; }
	void SetModelAnimator(shared_ptr<ModelAnimator> modelAnimator) { _modelAnimator = modelAnimator; }
	void SetAnimationState(AnimationState state);
    void SetHitBox(shared_ptr<GameObject> hitbox) { _hitbox = hitbox; }
    void SetHpBar(shared_ptr<GameObject> hpBar) { _hpBar = hpBar; }
    string GetObjID() { return objID; }
    void SetObjID(string str) { objID = str; }
    void UpdateHitBox();

public:
    void OnDeath();
    void DropItem();
    bool PlayingHitMotion = false;  
private:
    bool PlayCheckAnimating(AnimationState state);      // 해당 anim 플레이 , 플레이 중일시 true 아닐시 false
    void Aggro();
    void Move(Vec3 objPos, Vec3 targetPos, float speed);
    void Rota(Vec3 objPos, Vec3 targetPos);
    void Punch(int type);
    void ResetToIdleState();
    void ResetHit();

    void CreateEffect();
    void ActiveEffect();
    shared_ptr<GameObject> _effectObj;

	float _speed;
    float distance;
    Vec3 direction;
    float rangeDis;
    float animDuration;
    int atkType = 0;

	bool hasPatrolTarget = false;
    bool BackToStart = false;
    bool isFirstTime = false;
    bool chaseState = true;              //추격
    bool punchState = false;             //펀치 
    bool isExecuted_1 = false;
    bool playingSound = false;

    // 히트 상태
    bool _hit = false;
    bool _isDead = false;

    bool isPauseAfterPunch = false;
    float pauseEndTime = 0.0f;

    Vec3 playerPos;                     //플레이어 위치
    Vec3 StartPos;
    Vec3 CurForward;
    Vec3 EnemyPos;
    Vec3 patrolTarget;

    string objID;
	shared_ptr<Model> _enemy;
    shared_ptr<GameObject> _hitbox;
	shared_ptr<ModelAnimator> _modelAnimator;
	shared_ptr<Transform> _transform;
	shared_ptr<GameObject> _player;
	shared_ptr<GameObject> _hpBar;
	AnimationState _currentAnimationState = AnimationState::Idle;
};

