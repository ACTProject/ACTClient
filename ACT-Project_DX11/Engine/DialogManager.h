#pragma once
#include "Ui.h"

struct DialogueLine
{
    wstring characterName;        // 캐릭터 이름
    wstring text;                 // 대사 내용
    bool isLeftCharacter;         // 캐릭터 위치 (true: 왼쪽, false: 오른쪽)
    wstring characterImagePath;   // 캐릭터 이미지 경로
};

class DialogManager
{
public:
    void StartDialogue(const vector<DialogueLine>& dialogueLines);
    void ShowNextLine();
    bool IsDialogueActive() const;

private:
    void UpdateUI(const DialogueLine& currentLine);

    vector<DialogueLine> _dialogueLines;
    int _currentLineIndex = 0;
    bool _isDialogueActive = false;

    // UI Elements
    shared_ptr<Ui> _characterImageLeft;
    shared_ptr<Ui> _characterImageRight;
    shared_ptr<Ui> _textBox;
    shared_ptr<Ui> _characterName;
};

