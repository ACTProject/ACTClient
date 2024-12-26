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

};

