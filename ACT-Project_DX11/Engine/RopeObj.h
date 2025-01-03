#pragma once
#include "DynamicObj.h"
class RopeObj :
    public DynamicObj
{
    using Super = DynamicObj;
public:
    RopeObj(DynamicType _type);
    virtual ~RopeObj();

    virtual void Start() override;
    virtual void Update() override;
};

