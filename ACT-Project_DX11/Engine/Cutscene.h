#pragma once
#include <functional>

class Cutscene
{
public:
    using EventFunction = std::function<void()>;

    // 이벤트를 추가
    void AddEvent(EventFunction event);

    // 컷신 시작
    void Play();

    // 컷신 업데이트 (게임 루프에서 호출)
    void Update();

    // 컷신이 실행 중인지 확인
    bool IsPlaying() const;

private:
    struct Event
    {
        EventFunction function;    // 실행할 함수

        Event(EventFunction f) : function(move(f)) {}
    };

    vector<Event> _events;  // 이벤트 큐
    size_t _currentEventIndex = 0; // 현재 이벤트 인덱스
    bool _isPlaying = false;     // 컷신 실행 여부

    // Enter 입력 여부를 확인
    bool IsEnterPressed();
};
