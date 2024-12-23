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

    if (_uiID == "HP")
    {
        ImGui::Text("HP");
        if (ImGui::Button("Decrease HP"))
        {
            _uvRatio -= 0.1f;
            if (_uvRatio < 0.0f) _uvRatio = 0.0f;
        }
        ImGui::SliderFloat("HP Control", &_uvRatio, 0.0f, 1.0f);
    }
    else if (_uiID == "Armor")
    {
        ImGui::Text("Armor Slider");
        if (ImGui::Button("Decrease Armor"))
        {
            _uvRatio -= 0.1f;
            if (_uvRatio < 0.0f) _uvRatio = 0.0f;
        }
        ImGui::SliderFloat("Armor Control", &_uvRatio, 0.0f, 1.0f);
    }

    else if (_uiID == "Enemy")
    {
        ImGui::Text("Enemy Slider");
        if (ImGui::Button("Decrease Enemy"))
        {
            _uvRatio -= 0.1f;
            if (_uvRatio < 0.0f) _uvRatio = 0.0f;
        }
        ImGui::SliderFloat("Enemy Control", &_uvRatio, 0.0f, 1.0f);
    }
    
    UpdateScale();
}

void Slider::Create(Vec3 screenPos, Vec2 size, shared_ptr<class Material> material)
{
	auto go = _gameObject.lock();


	go->GetOrAddTransform()->SetLocalPosition(Vec3(screenPos.x,screenPos.y, screenPos.z));
	go->GetOrAddTransform()->SetScale(Vec3(size.x, size.y, 1.0f));

	float height = GRAPHICS->GetViewport().GetHeight();
	float width = GRAPHICS->GetViewport().GetWidth();

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
