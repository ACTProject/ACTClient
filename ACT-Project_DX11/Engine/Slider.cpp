#include "pch.h"
#include "Slider.h"
#include "pch.h"
#include "Slider.h"
#include "MeshRenderer.h"

Slider::Slider() : Super(UiType::SLIDER)
{

}

Slider::~Slider()
{
}

void Slider::Update()
{
    Super::PreUpdate();
    UpdateScale();
}

void Slider::Create(Vec3 screenPos, Vec2 size, shared_ptr<class Material> material)
{
	auto go = _gameObject.lock();
    //_screenPos = screenPos;
    _size = size;
    float height = GRAPHICS->GetViewport().GetHeight();
    float width = GRAPHICS->GetViewport().GetWidth();

    // 부모
    float px = _screenPos.x - width / 2;
    float py = height / 2 - _screenPos.y;
    // 자기자신
    float _x = screenPos.x - width / 2;
    float _y = height / 2 - screenPos.y;
    _thisPos = screenPos;
    _sub = Vec3(px - _x, py - _y, screenPos.z);



    float x = screenPos.x - width / 2;
    float y = height / 2 - screenPos.y;
    Vec3 position = Vec3(x, y, 0);

    UIMANAGER->AddObjUi(go->GetUI());


	go->GetOrAddTransform()->SetLocalPosition(Vec3(position.x, position.y, screenPos.z));
	go->GetOrAddTransform()->SetScale(Vec3(size.x, size.y, 1.0f));


	go->GetTransform()->SetLocalCenter(Vec3{ 0.5f,0.0f,0.0f });

	_maxRatio = size.x;

	go->SetLayerIndex(Layer_UI);

	if (go->GetMeshRenderer() == nullptr)
		go->AddComponent(make_shared<MeshRenderer>());

	go->GetMeshRenderer()->SetMaterial(material);


	auto mesh = RESOURCES->Get<Mesh>(L"Quad");
	go->GetMeshRenderer()->SetMesh(mesh);
	go->GetMeshRenderer()->SetPass(0);
	go->GetMeshRenderer()->SetAlphaBlend(true);
}

void Slider::UpdateScale()
{
    auto go = _gameObject.lock();
    Vec3 vecScale = go->GetTransform()->GetLocalScale();
    go->GetTransform()->SetLocalScale(Vec3(_maxRatio * _uvRatio, vecScale.y, vecScale.z));
}
