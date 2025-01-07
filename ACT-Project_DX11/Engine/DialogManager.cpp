#include "pch.h"
#include "DialogManager.h"

void DialogManager::StartDialogue(const vector<DialogueLine>& dialogueLines)
{
    _dialogueLines = dialogueLines;
    _currentLineIndex = 0;
    _isDialogueActive = true;

    ShowNextLine();
}

void DialogManager::ShowNextLine()
{
    if (_currentLineIndex >= _dialogueLines.size())
    {
        _isDialogueActive = false;
        return;
    }

    UpdateUI(_dialogueLines[_currentLineIndex]);
    _currentLineIndex++;
}

bool DialogManager::IsDialogueActive() const
{
    return _isDialogueActive;
}

void DialogManager::UpdateUI(const DialogueLine& currentLine)
{
    // 캐릭터 이미지 업데이트
    if (currentLine.isLeftCharacter)
    {
        _characterImageLeft->SetImage(currentLine.characterImagePath);
        _characterImageLeft->SetVisible(true);
        _characterImageRight->SetVisible(false);
    }
    else
    {
        _characterImageRight->SetImage(currentLine.characterImagePath);
        _characterImageRight->SetVisible(true);
        _characterImageLeft->SetVisible(false);
    }

    // 캐릭터 이름 및 대사 업데이트
    _characterName->SetText(currentLine.characterName);
    _textBox->SetText(currentLine.text);
}