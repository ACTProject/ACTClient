#pragma once
#include "Component.h"


enum class DynamicType
{
    None = 0,
    Potal,
    Jump,
    Save,
    Heal,
    PitFall,
    Rope,

    End,
};


class DynamicObj :
    public Component
{
    using Super = Component;
public:
    DynamicObj(DynamicType dType);
    virtual ~DynamicObj();

    virtual void Start() { };
    virtual void Update() { };
public:
    DynamicType GetDynamicType() { return _dtype; };
private:
    DynamicType _dtype = DynamicType::None;
};

