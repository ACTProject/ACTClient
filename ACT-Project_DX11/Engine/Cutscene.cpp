#include "pch.h"
#include "Cutscene.h"
#include <iostream>

// 이벤트를 추가
void Cutscene::AddEvent(EventFunction event)
{
    _events.emplace_back(event);
}

// 컷신 시작
void Cutscene::Play()
{
    if (_events.empty())
    {
        std::cerr << "No events to play!" << std::endl;
        return;
    }

    _isPlaying = true;
    _currentEventIndex = 0;

    // 첫 번째 이벤트 실행
    _events[_currentEventIndex].function();
}

// 컷신 업데이트
void Cutscene::Update()
{
    if (!_isPlaying) return;

    // Enter바 입력 감지
    if (IsEnterPressed())
    {
        _currentEventIndex++;

        if (_currentEventIndex < _events.size())
        {
            // 다음 이벤트 실행
            _events[_currentEventIndex].function();
        }
        else
        {
            // 모든 이벤트가 끝났으면 컷신 종료
            _isPlaying = false;
        }
    }
}

// 컷신 실행 여부 확인
bool Cutscene::IsPlaying() const
{
    return _isPlaying;
}

// Enter 입력 여부를 확인 
bool Cutscene::IsEnterPressed()
{
    if (INPUT->GetButtonDown(KEY_TYPE::ENTER))
        return true;

    return false;
}
