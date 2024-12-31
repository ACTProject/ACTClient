#pragma once
#include "DynamicObj.h"
class JumpObj :
    public DynamicObj
{
    using Super = DynamicObj;
public:
    JumpObj(DynamicType _type);
    virtual ~JumpObj();

    virtual void Start() override;
    virtual void Update() override;
};

