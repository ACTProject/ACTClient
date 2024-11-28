#include "pch.h"
#include "TimeManager.h"
#include <thread>

void TimeManager::Init()
{
    // CPU�� ���ػ� Ÿ�̸��� ���ļ�(�ʴ� ī��Ʈ ��)�� �����ɴϴ�.
    ::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&_frequency));
    // ���� CPU Ÿ�̸��� ���� �����ɴϴ�.
    ::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_prevCount));

    // ��ǥ ������ �ð� (60 FPS = 1 / 60��)
    _targetFrameTime = 1.0f / 60.f;
}

void TimeManager::Update()
{
    // ���� �������� CPU Ÿ�̸� ���� ������ currentCount�� �����մϴ�.
    uint64 currentCount;
    ::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentCount));

    // ���� �����Ӱ� ���� ������ ������ �ð�(�� ����)�� ����մϴ�.
    _deltaTime = (currentCount - _prevCount) / static_cast<float>(_frequency);

    // ��ǥ ������ �ð����� ���� ��� ���
    while (_deltaTime < _targetFrameTime)
    {
        // ���� �����ӱ��� ���� �ð� ���
        ::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentCount));
        _deltaTime = (currentCount - _prevCount) / static_cast<float>(_frequency);
    }

    // �������� �Ϸ�Ǿ����Ƿ� ���� Ÿ�̸� ���� ����
    _prevCount = currentCount;

    // FPS ���
    _frameCount++;
    _frameTime += _deltaTime;

    if (_frameTime >= 1.0f)
    {
        _fps = static_cast<uint32>(_frameCount);
        _frameCount = 0;
        _frameTime = 0.0f;
    }
}

