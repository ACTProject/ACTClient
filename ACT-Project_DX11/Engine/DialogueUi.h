#pragma once
#include "Ui.h"
#include "TextUi.h"
#include "ImageUi.h"

class DialogueUi : public Ui
{
    using Super = Ui;

public:
    DialogueUi();
    virtual ~DialogueUi() = default;

    void Create(const Vec3& screenPos, const Vec2& size, shared_ptr<class Material> backgroundMaterial);
    void Update() override;

    void SetDialogue(const wstring& characterName, const wstring& dialogueText, shared_ptr<class Material> characterImage);

    void Show();
    void Hide();

private:
    shared_ptr<TextUi> _nameText;        // 캐릭터 이름 UI
    shared_ptr<TextUi> _dialogueText;    // 대화 내용 UI
    shared_ptr<ImageUi> _characterImage; // 캐릭터 이미지 UI
    shared_ptr<ImageUi> _background;     // 대화창 배경 UI
};
