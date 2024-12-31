#include "pch.h"
#include "HealObj.h"

HealObj::HealObj(DynamicType _type) : DynamicObj(_type)
{
}

HealObj::~HealObj()
{
}

void HealObj::Start()
{
    GetGameObject()->GetModelRenderer()->SetPass(10);
}

void HealObj::Update()
{

}
