#include "pch.h"
#include "Ui.h"
#include "Camera.h"

void Ui::SetOwner(weak_ptr<GameObject> obj)
{
    _owner = obj;
}

// UI 생성 (상속받은 클래스에서 구현)
void Ui::Create(Vec3 screenPos, Vec2 size, shared_ptr<class Material> material)
{
    // 기본적인 UI 생성 로직 작성
    std::cout << "Creating UI of type: " << static_cast<int>(_type) << std::endl;
}

// UI 업데이트 (상속받은 클래스에서 구현)
void Ui::Update()
{
    // 기본적인 업데이트 로직
    std::cout << "Updating UI: " << _uiID << std::endl;

    // 상태에 따라 이벤트 실행
    if (_state == UiState::HOVERED && _onHover)
        _onHover();
    else if (_state == UiState::PRESSED && _onPress)
        _onPress();
}

void Ui::PreUpdate()
{
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

// 이벤트 핸들러 설정
void Ui::SetOnClick(const function<void()>& callback)
{
    _onClick = callback;
}

void Ui::SetOnHover(const function<void()>& callback)
{
    _onHover = callback;
}

void Ui::SetOnPress(const function<void()>& callback)
{
    _onPress = callback;
}