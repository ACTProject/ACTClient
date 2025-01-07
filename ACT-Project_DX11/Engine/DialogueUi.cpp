#include "pch.h"
#include "DialogueUi.h"
#include "Material.h"
#include <iostream>

DialogueUi::DialogueUi() : Super(UiType::NONE)
{
}

void DialogueUi::Create(const Vec3& screenPos, const Vec2& size, shared_ptr<Material> backgroundMaterial)
{
    // 대화창 배경 생성
    _background = make_shared<ImageUi>();
    _background->Create(screenPos, size, backgroundMaterial);

    // 캐릭터 이미지 생성 (기본 위치 설정)
    Vec3 imagePos = screenPos + Vec3(-size.x * 0.4f, 0.0f, 0.0f);
    _characterImage = make_shared<ImageUi>();
    _characterImage->Create(imagePos, Vec2(size.y * 0.8f, size.y * 0.8f), nullptr);

    // 캐릭터 이름 텍스트 생성
    Vec3 namePos = screenPos + Vec3(-size.x * 0.2f, size.y * 0.35f, 0.0f);
    _nameText = make_shared<TextUi>();
    _nameText->Create(namePos, Vec2(size.x * 0.5f, size.y * 0.2f), nullptr);

    // 대화 텍스트 생성
    Vec3 dialoguePos = screenPos + Vec3(0.0f, -size.y * 0.2f, 0.0f);
    _dialogueText = make_shared<TextUi>();
    _dialogueText->Create(dialoguePos, Vec2(size.x * 0.9f, size.y * 0.6f), nullptr);

    Hide();
}

void DialogueUi::Update()
{
    if (!_isVisible)
        return;

    // 각 UI 요소 업데이트
    _background->Update();
    _characterImage->Update();
    _nameText->Update();
    _dialogueText->Update();
}

void DialogueUi::SetDialogue(const wstring& characterName, const wstring& dialogueText, shared_ptr<Material> characterImage)
{
    // 캐릭터 이름 설정
    _nameText->SetText(characterName);

    // 대화 내용 설정
    _dialogueText->SetText(dialogueText);

    // 캐릭터 이미지 설정
    _characterImage->SetMaterial(characterImage);

    // 대화창 표시
    Show();
}

void DialogueUi::Show()
{
    _isVisible = true;
    _background->SetActive(true);
    _characterImage->SetActive(true);
    _nameText->SetActive(true);
    _dialogueText->SetActive(true);
}

void DialogueUi::Hide()
{
    _isVisible = false;
    _background->SetActive(false);
    _characterImage->SetActive(false);
    _nameText->SetActive(false);
    _dialogueText->SetActive(false);
}
