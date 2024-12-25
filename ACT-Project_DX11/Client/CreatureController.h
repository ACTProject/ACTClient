#pragma once
#include "MonoBehaviour.h"
#include "TaskQueue.h"

/*
    CreatureController는 생명체의 공통 동작과 스탯을 정의.
*/

class CreatureController : public MonoBehaviour
{

public:
    virtual void Start() override;
    virtual void Update() override;

    // 스탯 접근자
    float GetMaxHP() const { return _maxHp; }
    void SetMaxHP(float maxHp) { _maxHp = maxHp; }
    float GetHP() const { return _hp; }
    void SetHP(float hp) { _hp = hp; }
    float GetAtk() const { return _atk; }
    void SetAtk(float atk) { _atk = atk; }

public:
    void OnDamage(float damage);
    virtual void OnDeath();

protected:
    float _maxHp = 100.0f;      // 최대 체력
    float _hp = 100.0f;         // 체력
    float _atk = 10.0f;         // 공격력
};

