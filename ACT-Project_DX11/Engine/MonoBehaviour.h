#pragma once
#include "Component.h"
#include "MyCoroutine.h"
#include <coroutine>

enum class MonoBehaviourType : uint8
{
    None,
    Player,
    Camera,
    MelleMonster,
    ShootingMonster,
    FinalBossMonster_1,
    FinalBossMonster_2,
};

constexpr float EPSILON = 0.01f;
class MonoBehaviour : public Component
{
    using Super = Component;

public:
    MonoBehaviour();
    ~MonoBehaviour();

    virtual void Awake() override;
    virtual void Update() override;
    virtual MonoBehaviourType GetMonoBehaviourType() const { return MonoBehaviourType::None; }

    // Coroutine
    std::coroutine_handle<MyCoroutine::promise_type> currentEnemyCoroutine;

    void EnemyEndCoroutine() {
        if (currentEnemyCoroutine) {
            currentEnemyCoroutine.destroy();
            currentEnemyCoroutine = nullptr;
        }
    }

    // 플레이어공격 코루틴 함수 정의
    template<class T>
    MyCoroutine EnemyCoroutine(T t, float animationDuration)
    {
        // 애니메이션 재생 시간 대기
        co_await AwaitableSleep(chrono::milliseconds(static_cast<int>(animationDuration * 1000)));		// 공격 전 `atkType`을 랜덤으로 설정
        EnemyEndCoroutine();
    }

    void Remove(shared_ptr<GameObject> obj)
    {
        CUR_SCENE->Remove(obj);
    }
};

