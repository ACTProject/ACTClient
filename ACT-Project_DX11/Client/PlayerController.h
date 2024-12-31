#pragma once
#include "MonoBehaviour.h"
#include "Camera.h"
#include "CreatureController.h"

class Model;
class ModelAnimator;
/*
    PlayerController는 플레이어의 고유 동작을 담당
*/
class PlayerController : public CreatureController
{
    using Super = CreatureController;

	void Start() override;
	void Update() override;

public:
    MonoBehaviourType GetMonoBehaviourType() const override { return MonoBehaviourType::Player; }

	shared_ptr<Model> GetPlayer() { return _player; }
	void SetPlayer(shared_ptr<Model> player) { _player = player; }	
	shared_ptr<ModelAnimator> GetModelAnimator() { return _modelAnimator; }
	void SetModelAnimator(shared_ptr<ModelAnimator> modelAnimator) { _modelAnimator = modelAnimator; }
	void SetAnimationState(AnimationState state);
	void InteractWithShell(shared_ptr<GameObject> gameObject);
	void SetHitBox(shared_ptr<GameObject> hitbox) { _hitbox = hitbox; }
	void SetAirHitBox(shared_ptr<GameObject> hitbox) { _airhitbox = hitbox; }
	void SetChargeHitBox(shared_ptr<GameObject> hitbox) { _chargehitbox = hitbox; }
	void SetCamera(shared_ptr<GameObject> camera) { _camera = camera; }
    void SetDust(shared_ptr<Material> dust);
    void SetBubble(shared_ptr<Material> bubble);
    void SetEffect(shared_ptr<GameObject> effect) { _effect = effect; }
    void SetHitEffect(shared_ptr<GameObject> hitEffect) { _hitEffect = hitEffect; }

    // Handle
    void HandleInput();         // 입력 처리
    void HandleAnimations();    // 애니메이션 관리
    void HandleAttack();        // 공격 처리
    void HandleAirAttack();     // 공중 공격 상태 처리
    void HandleChargeAttack();  // 차지 공격 상태 처리
    void HandleDodge();         // 회피 처리
    void HandleJump();          // 점프 처리
    void HandleMovement();      // 이동 처리
    void HandleInteraction();   // 상호작용 처리
    void HandlePortal();        // 포탈 상호작용 처리
    void HandleHit();           // 히트 상태 처리


    // Attack
	void StartAttack();
	void ContinueAttack();
	void PlayAttackAnimation(int stage);
    void UpdateHitBox();
    void UpdateAirHitBox();
    void SetAttackReaource();
    void ActiveEffect(shared_ptr<GameObject> effect);
    void UpdateChargeHitBox();
    void CheckAtk(shared_ptr<BaseCollider> hitboxCollider, float damage);

    // Air Attack
    void StartAirAttack();
    void UpdateAirAttack();

    // Charge Attack
    void StartChargeAttack();
    void UpdateChargeAttack();

    // Shell
    bool GetIsBlocking() { return _isBlocking; }
    
    // Hit
    void StartHit();
    void UpdateHit();

    // Dodge
    bool GetIsInvincible() { return _isInvincible; }
    void StartDodge();
    void UpdateDodge();

    // Jump
    void Jump();

	void ResetToIdleState();

    // DustEffect
    void CreateDustEffect();
    // BubbleEffect     순서대로 한번에 생성되는 버블 개수, 플레이어 방향기준 x,y,z로 버블이 퍼져있는 정도, y축으로 +-, 플레이어가 보는 방향으로 +-
    //                  마지막 두 변수를 0, 0으로 하면 기본 위치는 _transform->GetPosition();
    void CreateBubbleEffect(int numBubbles, Vec3 bubbleSpread, float positionY, float positionLook);
    
    void HealPlayer();

    // SaveLoad -> 버튼클릭됐을 시 실행할 함수.
    void LoadPlayer(SaveData data);

public:
    void OnDeath() override;

private:
	float _speed = 5.f;
	shared_ptr<Model> _player;
	shared_ptr<ModelAnimator> _modelAnimator;
	shared_ptr<Transform> _transform;
    shared_ptr<GameObject> _camera;
	shared_ptr<GameObject> _hitbox;
	shared_ptr<GameObject> _airhitbox;
	shared_ptr<GameObject> _chargehitbox;
	shared_ptr<Rigidbody> _rigidbody;
    shared_ptr<GameObject> _effect;
    shared_ptr<GameObject> _hitEffect;

private:
	float _FPS;

    Vec3 _moveDir = Vec3(0.f);

    // Jump
    float _jumpSpeed = 15.f;
    bool _isJumping = false;
    float _jumpDuration = 0.0f; // 점프 애니메이션 지속시간 (초)
    float _jumpTimer = 0.0f;   // 점프 애니메이션 시간 추적

	// Attack
	int _attackStage = 0;           // 현재 공격 단계 (0: Idle, 1~4: 연속 공격 단계)
	bool _isAttacking = false;      // 공격 중인지 여부
	float _attackCooldown = 0.f;    // 공격 애니메이션 최소 실행 시간
	float _attackTimer = 0.0f;      // 현재 공격 단계의 경과 시간
	float _attackDurations[4];      // 각 공격 애니메이션 지속 시간 (초)
	float _currentDuration = 0.f;
    bool _isHit = false;            // 공격을 Hit 시켰는지 여부

    // AirAttack
    bool _isAirAttacking = false;       // 공중 공격 중인지 여부
    float _airAttackDuration = 0.0f;    // 공중 공격 동작 시간
    float _airAttackTimer = 0.0f;     

    // ChargeAttack
    bool _isCharging = false;              // 차지 중인지 여부
    bool _isChargeAttacking = false;       // 차지 공격 중인지 여부
    float _chargeAttackDuration = 0.0f;    // 차지 공격 동작 시간
    float _chargeThreshold = 0.2f;         // 차지 공격 발동 시간 (초)
    float _chargeTimer = 0.0f;             // 차지 발동 시간 (초)
    float _chargeAttackTimer = 0.0f;       // 차지 공격 발동 시간
    // AttackMove
    float _attackMoveDistance = 1.0f;  // 공격 시 이동할 거리
    float _attackMoveSpeed = 2.0f;     // 이동 속도
    float _attackMoveTimer = 0.0f;     // 이동 타이머 초기화

    // Dodge
    float _dodgeTimer = 0.0f;
    bool _isDodging = false;            // 회피 중인지 여부
    bool _isInvincible = false;         // 무적 상태 여부
    float _dodgeDuration = 0.0f;        // 회피 동작 시간
    float _dodgeDistance = 0.0f;        // 회피 이동 거리
    Vec3 _dodgeDirection = Vec3(0.f);   // 회피 방향 설정

    // Shell
    bool _isShellEquipped = false;      // 등껍질 장착 상태 여부
    bool _isBlocking = false;           // 막고 있는 상태인지 여부
    float _crawlSpeed = 2.f;            // 기어가는 속도  

    // Hit
    bool _hit = false;                // 히트 상태인지 여부
    float _hitDuration = 0.0f;        // 히트 동작 시간
    float _hitTimer = 0.0f;         

    // 애니메이션 진행 중인지 여부
	bool _isPlayeringJumpAnimation = false; // 점프 애니메이션 재생 중인지 여부 확인
	bool _isPlayeringAttackAnimation = false; // 공격 애니메이션 재생 중인지 여부 확인
	bool _isPlayeringAirAttackAnimation = false; // 공중 공격 애니메이션 재생 중인지 여부 확인
	bool _isPlayeringChargeAttackAnimation = false; // 차지 공격 애니메이션 재생 중인지 여부 확인
	bool _isPlayeringDodgeAnimation = false; // 회피 애니메이션 재생 중인지 여부 확인
	bool _isPlayeringHitAnimation = false; // 히트 애니메이션 재생 중인지 여부 확인

	AnimationState _currentAnimationState = AnimationState::Idle;

    // Dust
    shared_ptr<Material> _dustMaterial;
    float _dustInterval = 0.1f;
    float _dustTimer = 0.0f;

    // Bubble
    shared_ptr<Material> _bubbleMaterial;

    // Sound
    float _footstepTimer = 0.0f;
    float _runningInterval = 0.2f;
    float _walkingInterval = 0.3f;
public:
    // 스탯 접근자
    float GetShellMaxHP() const { return _shellMaxHp; }
    void SetShellMaxHP(float shellMaxHp) { _shellMaxHp = shellMaxHp; }
    float GetShellHP() const { return _shellHp; }
    void SetShellHP(float shellHp) { _shellHp = shellHp; }
private:
    // Stat
    float _shellMaxHp;
    float _shellHp;
    float _healHp = 50;

    // 전리품
    int _spoil = 0;

    bool _playerActive = true;
};

