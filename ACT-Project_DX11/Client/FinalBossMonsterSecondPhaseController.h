#pragma once
#include "MonoBehaviour.h"
#include "EnginePch.h"
#include "CreatureController.h"
#include "MonsterController.h"
#include "Model.h"
#include "ModelAnimator.h"
#include "ModelRenderer.h"
#include "ModelMesh.h"
#include "HitBox.h"

class Model;
class ModelAnimator;
class ModelRenderer;
struct ModelBone;

/*
    각 몬스터의 개별 동작을 추가로 정의.
*/
class FinalBossMonsterSecondPhaseController : public MonsterController
{
    using Super = MonsterController;

    void Start() override;
    void Update() override;

public:
    MonoBehaviourType GetMonoBehaviourType() const override { return MonoBehaviourType::FinalBossMonster_2; }

    using PhaseFunction = void (FinalBossMonsterSecondPhaseController::*)();
    std::vector<PhaseFunction> phaseActions = {
    &FinalBossMonsterSecondPhaseController::Punch,          // 0
    &FinalBossMonsterSecondPhaseController::Choke_lift,     // 1
    &FinalBossMonsterSecondPhaseController::Fireball,       // 2
    &FinalBossMonsterSecondPhaseController::FireMoney,      // 3
    &FinalBossMonsterSecondPhaseController::Slash,          // 4
    &FinalBossMonsterSecondPhaseController::Slam,           // 5
    &FinalBossMonsterSecondPhaseController::Hurricane,      // 6
    };

    shared_ptr<Model> GetEnemy() { return _enemy; }
    void SetEnemy(shared_ptr<Model> enemy) { _enemy = enemy; }
    shared_ptr<ModelRenderer> GetModelRenderer() { return _modelRenderer; }
    shared_ptr<ModelAnimator> GetModelAnimator() { return _modelAnimator; }
    void SetModelRenderer(shared_ptr<ModelRenderer> modelRenderer) { _modelRenderer = modelRenderer; }
    void SetModelAnimator(shared_ptr<ModelAnimator> modelAnimator) { _modelAnimator = modelAnimator; }
    void SetHitBox(shared_ptr<GameObject> hitbox) { _hitbox = hitbox; }
    void SetSlamHitBox(shared_ptr<GameObject> slamhitbox) { _slamhitbox = slamhitbox; }
    void SetHurricaneHitBox(shared_ptr<GameObject> hurricaneHitbox) { _hurricaneHitbox = hurricaneHitbox; }
    void SetChokeHitBox(shared_ptr<GameObject> chokeHitbox) { _chokeHitbox = chokeHitbox; }
    void SetAnimationState(AnimationState state);
    void SetHpBar(shared_ptr<GameObject> hpBar) { _hpBar = hpBar; }
    string GetObjID() { return objID; }
    void SetObjID(string str) { objID = str; }

    void ResetToIdleState();
    bool PlayingHitMotion = false;

private:
    bool PlayCheckAnimating(AnimationState state);      // 해당 anim 플레이 , 플레이 중일시 true 아닐시 false
    void Phase_2();                                     // 2페이즈

    void Appear();                                      // 조우시 모션용 어그로와 같음
    void Move(Vec3 objPos, Vec3 targetPos, float speed);// 타겟 방향으로 이동
    void Rota(Vec3 objPos, Vec3 targetPos);             // 타겟 방향으로 회전
    void BackSprint();
    void Sprint();
    void Run(float speed);

    void Slash();
    void Slam();
    void Punch();                            // 펀치 공격
    void Fireball();
    void FireMoney();
    void Choke_lift();
    void Hurricane();

    void UpdateHitBox(float f, float damage);
    void UpdateSlamHitBox();
    void UpdateHurricaneHitBox();
    void UpdateChokeHitBox();
    void makeBubble(Vec3 pos, Vec3 dir);
    void makeCash(Vec3 pos, Vec3 dir);
    void checkHit(shared_ptr<BaseCollider> hitboxCollider, float damage);

public:
    void OnDeath() override;
    float lastTime = 0.f;               //마지막 애니메이션 시간
    float duration;

    Vec3 bossPos;                       //보스 위치
    Vec3 playerPos;                     //플레이어 위치
    float distance;                     //플레이어 - 보스 거리
    Vec3 direction;                     //플레이어 - 보스 방향
    float hp;                  //보스 hp
    float speed;

    float shootTime = 0.0f;
    int randType;                       //랜덤한 타입
    int randPunchType;                  //랜덤한 펀치 타입

    // 플래그
    bool isFirstTime = false;            //조우인지 여부 ( Appear 용 )
    bool postpone = false;              //패턴 끝나고 잠깐의 공백 시간
    bool Phase2Flag = false;
    bool isExecuted = false;
    bool isExecuted_2 = false;
    bool isExecuted_3 = false;
    bool isExecuted_4 = false;
    bool punchExecuted = false;
    bool playingSound = false;

    // 상태
    bool chaseState = false;             //추격
    bool punchState = false;             //펀치 
    bool shootState = false;
    bool attackState = false;
    bool _isDead = false;
    bool _hit = false;

    shared_ptr<Model> _enemy;
    shared_ptr<GameObject> _hitbox;
    shared_ptr<GameObject> _slamhitbox;
    shared_ptr<GameObject> _hurricaneHitbox;
    shared_ptr<GameObject> _chokeHitbox;
    shared_ptr<ModelRenderer> _modelRenderer;
    shared_ptr<ModelAnimator> _modelAnimator;
    shared_ptr<Transform> _transform;
    shared_ptr<GameObject> _player;
    shared_ptr<ModelBone> rightHand;
    shared_ptr<GameObject> _hpBar;
    string objID;
    AnimationState _currentAnimationState = AnimationState::Idle;
};

