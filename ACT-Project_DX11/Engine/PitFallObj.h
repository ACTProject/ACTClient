#pragma once
#include "DynamicObj.h"
class PitFallObj :
    public DynamicObj
{
    using Super = DynamicObj;
public:
    PitFallObj(DynamicType _type);
    virtual ~PitFallObj();

    virtual void Start() override;
    virtual void Update() override;
};

