#pragma once
#include "DynamicObj.h"
class HealObj :
    public DynamicObj
{
    using Super = DynamicObj;
public:
    HealObj(DynamicType _type);
    virtual ~HealObj();

    virtual void Start() override;
    virtual void Update() override;
};

