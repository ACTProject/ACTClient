#include "pch.h"
#include "Ui.h"
#include "Camera.h"

void Ui::SetOwner(weak_ptr<GameObject> obj)
{
    _owner = obj;
}

void Ui::PreUpdate()
{
    ImGui::Checkbox("visible", &_isVisible);

    shared_ptr<GameObject> owner = _owner.lock();
    if (owner != nullptr)
    {
        Vec3 pos = owner->GetTransform()->GetLocalPosition();
        pos += _addPos;
        Matrix VP = Camera::S_MatView * Camera::S_MatProjection;
        Vec3 uiClipPos = XMVector3TransformCoord(pos, VP);
        float d = sqrt(uiClipPos.x * uiClipPos.x + uiClipPos.y * uiClipPos.y + uiClipPos.z * uiClipPos.z);
        float xNDC = uiClipPos.x / d;
        float yNDC = uiClipPos.y / d;
        float screenX = (xNDC + 1.0f) * 400.f - 400;
        float screenY = (yNDC + 1.0f) * 300.f - 300;
        Vec3 screen = {screenX,screenY,1.0f};
        GetGameObject()->GetTransform()->SetLocalPosition(screen);
    }
}
