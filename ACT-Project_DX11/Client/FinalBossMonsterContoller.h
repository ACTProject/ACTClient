#pragma once
#include "MonoBehaviour.h"
#include "EnginePch.h"
#include "A_Star.h"
#include "Model.h"
#include "ModelAnimator.h"
#include "ModelRenderer.h"
#include "ModelMesh.h"
#include "HitBox.h"

class Model;
class ModelAnimator;
class ModelRenderer;
struct ModelBone;

class FinalBossMonsterContoller : public MonoBehaviour
{
    shared_ptr<Shader> renderShader = make_shared<Shader>(L"23. RenderDemo.fx");

    virtual void Start() override;
    virtual void Update() override;

public:
    shared_ptr<Model> GetEnemy() { return _enemy; }
    void SetEnemy(shared_ptr<Model> enemy) { _enemy = enemy; }
    shared_ptr<ModelRenderer> GetModelRenderer() { return _modelRenderer; }
    shared_ptr<ModelAnimator> GetModelAnimator() { return _modelAnimator; }
    void SetModelRenderer(shared_ptr<ModelRenderer> modelRenderer) { _modelRenderer = modelRenderer; }
    void SetModelAnimator(shared_ptr<ModelAnimator> modelAnimator) { _modelAnimator = modelAnimator; }
    void SetHitBox(shared_ptr<GameObject> hitbox) { _hitbox = hitbox; }
    void SetAnimationState(AnimationState state);

    void ResetToIdleState();

private:
    bool PlayCheckAnimating(AnimationState state);      // 해당 anim 플레이 , 플레이 중일시 true 아닐시 false
    void Phase_1();                                     // 1페이즈
    void Phase_2();                                     // 2페이즈
    void Appear();                                      // 조우시 모션용 어그로와 같음
    void Move(Vec3 objPos, Vec3 targetPos, float speed);// 타겟 방향으로 이동
    void Walk();
    void Rota(Vec3 objPos, Vec3 targetPos);             // 타겟 방향으로 회전
    void Die();                                         // 죽음
    void Punch();                            // 펀치 공격
    void Fireball();
    void FireMoney();
    void Choke_lift();
    void GrabSlam();
    void Hurricane();
    Matrix CalculateWorldTransform(shared_ptr<ModelBone> bone);
    void makeBubble(Vec3 pos, Vec3 dir);
    void makeCash(Vec3 pos, Vec3 dir);

    float currentTime = 0.f;            //현재 게임 시간
    float lastTime = 0.f;               //마지막 애니메이션 시간
    float _FPS;                         //게임 FPS = 60
    float dt;                           //deltaTime = 프레임당 시간

    Vec3 bossPos;                       //보스 위치
    Vec3 playerPos;                     //플레이어 위치
    float distance;                     //플레이어 - 보스 거리
    Vec3 direction;                     //플레이어 - 보스 방향
    float hp = 100.f;                  //보스 hp
    float speed = 5.0f;

    float animPlayingTime = 0.0f;       //애니메이션 플레이 타임
    int myPhase = 1;                    //1페이즈 2페이즈 구분용
    int patternCnt = 4;
    float shootTime = 0.0f;
    int randType;                       //랜덤한 타입
    int randPunchType;                  //랜덤한 펀치 타입
    Vec3 lastPos;

    // 플래그
    bool isFirstTime = false;            //조우인지 여부 ( Appear 용 )
    bool postpone = false;              //패턴 끝나고 잠깐의 공백 시간
    bool Phase2Flag = false;
    bool isExecuted = false;
    bool isExecuted_2 = false;
    bool isExecuted_3 = false;

    // 상태
    bool chaseState = false;             //추격
    bool punchState = false;             //펀치 
    bool shootState = false;
    bool attackState = false;

    shared_ptr<Model> _enemy;
    shared_ptr<GameObject> _hitbox;
    shared_ptr<ModelRenderer> _modelRenderer;
    shared_ptr<ModelAnimator> _modelAnimator;
    shared_ptr<Transform> _transform;
    shared_ptr<GameObject> _player;
    shared_ptr<ModelBone> rightHand;
    AnimationState _currentAnimationState = AnimationState::Idle;
};

