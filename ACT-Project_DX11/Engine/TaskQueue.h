#pragma once

#include <functional>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>

class TaskQueue
{
public:
    static TaskQueue& GetInstance();

    TaskQueue();  // 생성자: TaskQueue 초기화 및 스레드 시작
    ~TaskQueue(); // 소멸자: TaskQueue 종료 및 자원 정리

    /*
      @brief 작업 큐에 작업 추가
      @param task 실행할 작업 (std::function<void()> 형식)
     */
    void AddTask(std::function<void()> task);

    /*
      @brief 작업 큐의 실행 루프
             내부에서 큐의 작업을 처리하는 메서드
     */
    void Run();

    /*
      @brief 작업 큐 실행 중지
             _running 플래그를 false로 설정하여 작업 종료
     */
    void Stop();

    void Clear();

private:
    std::queue<std::function<void()>> _taskQueue;  // 실행할 작업을 저장하는 큐
    std::mutex _queueMutex;                        // 작업 큐의 동기화를 위한 mutex
    std::condition_variable _condition;            // 작업 큐 상태 변화 알림을 위한 조건 변수
    std::atomic<bool> _running;                    // 작업 큐 실행 상태 (true: 실행 중, false: 중지)
    std::thread _workerThread;                     // 작업을 처리할 스레드
};