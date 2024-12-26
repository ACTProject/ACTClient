#pragma once
#include "MonoBehaviour.h"
#include "EnginePch.h"
#include "CreatureController.h"
#include "MonsterController.h"
#include "Model.h"
#include "ModelAnimator.h"

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
    void UpdateHitBox();

public:
    bool PlayingHitMotion = false;
private:
    bool PlayCheckAnimating(AnimationState state);      // 해당 anim 플레이 , 플레이 중일시 true 아닐시 false
    void Aggro();
    void Move(Vec3 objPos, Vec3 targetPos, float speed);
    void Rota(Vec3 objPos, Vec3 targetPos);
    void Punch(int type);
    //void Tracking(Vec3 pos, const std::vector<Node3D>& path);
    void ResetToIdleState();
    void ResetHit();

	float _speed;
	float _hp;
	float _atk;
	float dt;
	float _FPS;
	float animPlayingTime = 0.0f;
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

    // 히트 상태
    bool _hit = false;
    bool _isDead = false;
    bool hasDealing = false;

    Vec3 StartPos;
    Vec3 CurForward;
    Vec3 EnemyPos;
    Vec3 PlayerPos;
    Vec3 patrolTarget;

	shared_ptr<Model> _enemy;
    shared_ptr<GameObject> _hitbox;
	shared_ptr<ModelAnimator> _modelAnimator;
	shared_ptr<Transform> _transform;
	shared_ptr<GameObject> _player;
	AnimationState _currentAnimationState = AnimationState::Idle;
};

