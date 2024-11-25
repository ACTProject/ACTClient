#include "pch.h"
#include "TimeManager.h"

void TimeManager::Init()
{
	// CPU�� ���ػ� Ÿ�̸��� ���ļ�(�ʴ� ī��Ʈ ��)�� �����ɴϴ�.
	::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&_frequency));
	// ���� CPU Ÿ�̸��� ���� �����ɴϴ�.
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_prevCount));
}

void TimeManager::Update()
{
	// ���� �������� CPU Ÿ�̸� ���� ������ currentCount�� �����մϴ�.
	uint64 currentCount;
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentCount));

	// ���� �����Ӱ� ���� ������ ������ �ð�(�� ����)�� ����մϴ�.
	_deltaTime = (currentCount - _prevCount) / static_cast<float>(_frequency);
	_prevCount = currentCount;

	// 1�� ���� ó���� ������ �� ī��Ʈ
	_frameCount++;
	// ���� �ð�
	_frameTime += _deltaTime;

	// ���� �ð��� 1�ʸ� �ʰ��ϸ� FPS�� ���
	if (_frameTime > 1.f)
	{
		// �ʴ� ������ ���� ����Ͽ� �����մϴ�
		_fps = static_cast<uint32>(_frameCount / _frameTime);

		_frameTime = 0.f;
		_frameCount = 0;
	}
}