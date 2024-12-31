#pragma once
#include "DynamicObj.h"

class Potal :
    public DynamicObj
{
    using Super = DynamicObj;
public:
    Potal(DynamicType _type);
    virtual ~Potal();

    virtual void Start() override;
    virtual void Update() override;
};

