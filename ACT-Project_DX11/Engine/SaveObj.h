#pragma once
#include "DynamicObj.h"
class SaveObj :
    public DynamicObj
{
    using Super = DynamicObj;
public:
    SaveObj(DynamicType _type);
    virtual ~SaveObj();

    virtual void Start() override;
    virtual void Update() override;
};

