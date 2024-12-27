#include "pch.h"
#include "Potal.h"

Potal::Potal(DynamicType _type) : DynamicObj(_type)
{
}

Potal::~Potal()
{
}

void Potal::Start()
{
    auto obj = _gameObject.lock();
    obj->SetObjectType(ObjectType::Portal);
}

void Potal::Update()
{

}
