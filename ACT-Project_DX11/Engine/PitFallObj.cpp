#include "pch.h"
#include "PitFallObj.h"

PitFallObj::PitFallObj(DynamicType _type) : DynamicObj(_type)
{

}

PitFallObj::~PitFallObj()
{
}

void PitFallObj::Start()
{
    //auto obj = _gameObject.lock();
    //{
    //    shared_ptr<Rigidbody> rigidBody = make_shared<Rigidbody>();
    //    rigidBody->SetUseGravity(false);
    //    rigidBody->SetMass(50.0f);
    //    obj->AddComponent(rigidBody);

    //    COLLISION->AddRigidbody(rigidBody);
    //}
}

void PitFallObj::Update()
{
}
