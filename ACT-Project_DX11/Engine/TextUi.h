#pragma once
#include "Ui.h"

class TextUi : public Ui
{
    using Super = Ui;

public:
    TextUi() : Super(UiType::TEXT) {}
    virtual ~TextUi() = default;

    void Create(Vec3 screenPos, Vec2 size, shared_ptr<class Material> material) override;
    void Update() override;

    void SetText(const wstring& text) { _text = text; }
    const wstring& GetText() const { return _text; }

private:
    wstring _text;
};