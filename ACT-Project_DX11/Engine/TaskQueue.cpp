#include "pch.h"
#include "TaskQueue.h"

TaskQueue& TaskQueue::GetInstance()
{
    static TaskQueue instance;
    return instance;
}

// 생성자: 작업 큐 초기화 및 스레드 실행
TaskQueue::TaskQueue()
    : _running(true)  // 작업 큐 실행 상태를 true로 설정
{
    // 작업을 처리할 스레드 시작
    _workerThread = std::thread(&TaskQueue::Run, this);
}

// 소멸자: 작업 큐 종료 및 자원 정리
TaskQueue::~TaskQueue()
{
    Stop(); // 작업 큐 종료
    if (_workerThread.joinable()) // 스레드가 실행 중이면
        _workerThread.join();     // 스레드 종료 대기
}

// 작업 추가: 큐에 작업을 추가하고 조건 변수를 통해 작업 알림
void TaskQueue::AddTask(std::function<void()> task)
{
    {
        std::lock_guard<std::mutex> lock(_queueMutex); // 작업 큐에 접근하는 동안 잠금
        _taskQueue.push(task); // 큐에 작업 추가
    }
    _condition.notify_one(); // 작업이 추가되었음을 스레드에 알림
}

// 작업 큐 실행: 큐에서 작업을 가져와 실행하는 메서드
void TaskQueue::Run()
{
    while (_running) // _running이 true인 동안 실행
    {
        std::function<void()> task;

        {
            // 큐가 비어 있으면 대기
            std::unique_lock<std::mutex> lock(_queueMutex);
            _condition.wait(lock, [this]() { return !_taskQueue.empty() || !_running; });

            // _running이 false이고 큐가 비어 있으면 종료
            if (!_running && _taskQueue.empty())
                return;

            // 작업 큐에서 작업 가져오기
            task = std::move(_taskQueue.front());
            _taskQueue.pop();
        }

        // 가져온 작업 실행
        task();
    }
}

// 작업 큐 중지: 실행 상태를 false로 설정하고 모든 스레드 깨우기
void TaskQueue::Stop()
{
    {
        std::lock_guard<std::mutex> lock(_queueMutex); // 작업 큐 접근 동기화
        _running = false; // 실행 상태 중지
    }
    _condition.notify_all(); // 모든 대기 중인 스레드에 알림
}

void TaskQueue::Clear()
{
    std::lock_guard<std::mutex> lock(_queueMutex);
    while (!_taskQueue.empty())
    {
        _taskQueue.pop(); // 모든 작업 제거
    }
}

