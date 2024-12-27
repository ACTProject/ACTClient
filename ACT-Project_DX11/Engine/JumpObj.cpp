#include "pch.h"
#include "JumpObj.h"

JumpObj::JumpObj(DynamicType _type) : DynamicObj(_type)
{
}

JumpObj::~JumpObj()
{
}

void JumpObj::Start()
{
    auto obj = _gameObject.lock();
    {
        shared_ptr<Rigidbody> rigidBody = make_shared<Rigidbody>();
        rigidBody->SetUseGravity(false);
        rigidBody->SetMass(1000.0f);
        obj->AddComponent(rigidBody);

        COLLISION->AddRigidbody(rigidBody);
    }
}

void JumpObj::Update()
{

}
