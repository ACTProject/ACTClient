#pragma once
#include "Scene.h"

class SceneManager
{
	DECLARE_SINGLE(SceneManager);

public:
	void Awake();		// �ٸ� ������Ʈ�� ������Ʈ�� �����ϱ� ���� ����
	void Start();		// ������Ʈ Ȱ��ȭ�� ���� �ʱ�ȭ ����
	void FixedUpdate();		// �� �����Ӹ��� ȣ��
	void Update();		// �� �����Ӹ��� ȣ��

	void AddScene(const std::string& name, std::shared_ptr<Scene> scene);
	void ChangeScene(const string& name);

	shared_ptr<Scene> GetCurrentScene() { return _currentScene; }
	float GetFixedDeltaTime() { return _fixedDeltaTime; }

private:
	shared_ptr<Scene> _currentScene = make_shared<Scene>();
	unordered_map<string, shared_ptr<Scene>> _scenes;

	const float _fixedDeltaTime = 0.02f; // Fixed timestep (20ms)
	float _accumulatedTime = 0.0f;			// ������ �ð�
	float _deltaTime = 0.0f;				// ���� �������� ��� �ð�
};

