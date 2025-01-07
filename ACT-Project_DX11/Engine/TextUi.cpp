#include "pch.h"
#include "TextUi.h"
#include "Material.h"

TextUi::TextUi() : Super(UiType::NONE)
{
}

void TextUi::Create(Vec3 screenPos, Vec2 size, shared_ptr<Material> material)
{
}

void TextUi::Update()
{
    if (!_isVisible)
        return;
}

void TextUi::SetText(const wstring& text)
{
    _text = text;
}

const wstring& TextUi::GetText() const
{
    return _text;
}
