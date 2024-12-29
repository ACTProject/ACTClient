#include "pch.h"
#include "SaveObj.h"

SaveObj::SaveObj(DynamicType _type) : DynamicObj(_type)
{

}

SaveObj::~SaveObj()
{
}

void SaveObj::Start()
{
   // 여기서 플레이어 얻어오고.
}

void SaveObj::Update()
{
    // 여기서 SaveManager에서 ui버튼 클릭 한 뒤 인덱스만 받아와서
    // 플레이어 상태 on하고 setLoclaPosition 할까?

}
