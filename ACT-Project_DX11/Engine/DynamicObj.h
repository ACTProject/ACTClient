#pragma once
#include "Component.h"


enum class DynamicType
{
    None = 0,
    Potal,


    End,
};


class DynamicObj :
    public Component
{
    using Super = Component;
public:
    DynamicObj(DynamicType dType);
    virtual ~DynamicObj();

    virtual void Start() { int a = 0; };
    virtual void Update() { };
public:
    DynamicType GetDynamicType() { return _dtype; };
private:
    DynamicType _dtype = DynamicType::None;
};

