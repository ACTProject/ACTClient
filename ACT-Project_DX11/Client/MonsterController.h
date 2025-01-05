#pragma once
#include "CreatureController.h"
/*
    MonsterController는 모든 몬스터의 공통 동작을 관리.
*/
class MonsterController : public CreatureController
{
    using Super = CreatureController;

public:
    void Start() override;
    void Update() override;


public:
    float currentTime;            //현재 게임 시간
    float _FPS;                         //게임 FPS = 60
    Vec3 playerPos;                     //플레이어 위치
    float animPlayingTime = 0.0f;       //애니메이션 플레이 타임

    shared_ptr<GameObject> _player;
};

