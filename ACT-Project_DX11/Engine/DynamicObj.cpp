#include "pch.h"
#include "DynamicObj.h"

DynamicObj::DynamicObj(DynamicType dType) : Super(ComponentType::DynamicObject)
{
    _dtype = dType;
}

DynamicObj::~DynamicObj()
{
}
