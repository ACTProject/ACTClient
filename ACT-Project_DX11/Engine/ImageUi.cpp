#include "pch.h"
#include "ImageUi.h"
#include "MeshRenderer.h"
#include "Material.h"

ImageUi::ImageUi() : Super(UiType::NONE)
{
}

void ImageUi::Create(Vec3 screenPos, Vec2 size, shared_ptr<Material> material)
{
    _meshRenderer = make_shared<MeshRenderer>();
    _meshRenderer->SetMaterial(material);
}

void ImageUi::Update()
{
    if (!_isVisible)
        return;

    //_meshRenderer->Render();
}

void ImageUi::SetMaterial(shared_ptr<Material> material)
{
    _meshRenderer->SetMaterial(material);
}

shared_ptr<class Material> ImageUi::GetMaterial() const
{
    return _meshRenderer->GetMaterial();
}
