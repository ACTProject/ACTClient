#include "pch.h"
#include "SaveObj.h"

SaveObj::SaveObj(DynamicType _type) : DynamicObj(_type)
{

}

SaveObj::~SaveObj()
{
}

void SaveObj::Start()
{
    // Start하면 모델렌더러로 되어 있는 얘를 모델애니메이션으로 바꿔주고,
    // 만약 e를 눌렀다면? ui를 띄워서 paddig 간격대로 만들게끔 하고,
    auto obj = GetGameObject();
    // 모델렌더러를 모델애니메이션으로 교체작업. 모델안에 쉐이더랑 다 있음.
    auto model = obj->GetModelRenderer()->GetModel();
    {
        // Model
        {
            //model->ReadAnimation(L"Enemy/Rangoon_idle", AnimationState::Idle);
        }
     /*   shared_ptr<ModelAnimator> ma2 = make_shared<ModelAnimator>();
        obj->AddComponent(ma2);
        {
            obj->GetModelAnimator()->SetModel(model);
            obj->GetModelAnimator()->SetPass(2);
        }*/
    }



    //obj->EraseComponent(obj->GetModelRenderer());
}

void SaveObj::Update()
{

}
