#include "pch.h"
#include "Button.h"
#include "MeshRenderer.h"
#include "Material.h"


Button::Button() : Super(ComponentType::Button)
{

}

Button::~Button()
{

}

bool Button::Picked(POINT screenPos)
{
	return ::PtInRect(&_rect, screenPos);
}



void Button::Create(Vec3 screenPos, Vec2 size, shared_ptr<class Material> material)
{
	auto go = _gameObject.lock();

	float height = GRAPHICS->GetViewport().GetHeight();
	float width = GRAPHICS->GetViewport().GetWidth();

    _screenPos = screenPos;
    _size = size;

	float x = _screenPos.x - width / 2;
	float y = height / 2 - _screenPos.y;
	Vec3 position = Vec3(x, y, screenPos.z);

	go->GetOrAddTransform()->SetPosition(position);
	go->GetOrAddTransform()->SetScale(Vec3(size.x, size.y, 1));

	go->SetLayerIndex(Layer_UI);

	if (go->GetMeshRenderer() == nullptr)
		go->AddComponent(make_shared<MeshRenderer>());

	go->GetMeshRenderer()->SetMaterial(material);

    if (RESOURCES->Get<Mesh>(L"Quad") == nullptr)
    {
        shared_ptr<Mesh> mes = make_shared<Mesh>();
        mes->CreateQuad();
        RESOURCES->Add(L"Quad", mes);
    }
    auto mesh = RESOURCES->Get<Mesh>(L"Quad");
	go->GetMeshRenderer()->SetMesh(mesh);
	go->GetMeshRenderer()->SetPass(0);

	// Picking
	_rect.left = _screenPos.x - _size.x / 2;
	_rect.right = _screenPos.x + _size.x / 2;
	_rect.top = _screenPos.y - _size.y / 2;
	_rect.bottom = _screenPos.y + _size.y / 2;

    if (width > 1800)
    {
        float WidthRatio = 1920.f / 800.f;
        float HeightRatio = 1017.f / 600.f;
        _screenPos = Vec3(300.f * WidthRatio, (screenPos.y -200.0f) * HeightRatio, _screenPos.z);
        _size = Vec2(_size.x * WidthRatio, _size.y * HeightRatio);

        // Picking
        _rect.left = _screenPos.x - _size.x / 2;
        _rect.right = _screenPos.x + _size.x / 2;
        _rect.top = _screenPos.y - _size.y / 2;
        _rect.bottom = _screenPos.y + _size.y / 2;
    }
}

void Button::AddOnClickedEvent(std::function<void(void)> func)
{
	_onClicked = func;
}

void Button::InvokeOnClicked()
{
    bool isActive = GetGameObject()->IsActive();
	if (_onClicked && isActive)
		_onClicked();
}



void Button::AddOnHoverEvent(std::function<void(void)> func)
{
    _onHover = func;
}

void Button::AddOnHoverEndEvent(std::function<void(void)> func)
{
    _onHoverEnd = func;
}

void Button::CheckHover(POINT screenPos)
{
    if (Picked(screenPos))
    {
        if (!_isHoverd)
        {
            _isHoverd = true;
            if (_onHover)
                _onHover();
        }
    }
    else if (_isHoverd)
    {
        _isHoverd = false;
        if (_onHoverEnd)
            _onHoverEnd();
    }
}

void Button::AddOnKeyPressEvent(KEY_TYPE key, std::function<void(void)> func)
{
    _onKeyPress[key] = func;
}

void Button::InvokeOnKeyPress(KEY_TYPE key)
{
    if (_onKeyPress.find(key) != _onKeyPress.end())
    {
        if (_onKeyPress[key])
            _onKeyPress[key]();
    }
}

void Button::CheckKeyInput()
{
    for (const auto& [key, callback] : _onKeyPress) {
        if (INPUT->GetButtonDown(key)) {
            InvokeOnKeyPress(key);
        }
    }
}

void Button::ResizeRect(float width, float height)
{
    float initWidth = GRAPHICS->GetViewport().GetWidth();
    float initHeight = GRAPHICS->GetViewport().GetHeight();

    float WidthRatio = width / initWidth;
    float HeightRatio = height / initHeight;
    _screenPos = Vec3(_screenPos.x * WidthRatio, _screenPos.y * HeightRatio,_screenPos.z);
    _size = Vec2(_size.x * WidthRatio,_size.y * HeightRatio);


    // Picking
    _rect.left = _screenPos.x - _size.x / 2;
    _rect.right = _screenPos.x + _size.x / 2;
    _rect.top = _screenPos.y - _size.y / 2;
    _rect.bottom = _screenPos.y + _size.y / 2;
}
