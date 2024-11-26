#include "pch.h"
#include "SceneManager.h"

void SceneManager::Awake()
{
	if (_currentScene != nullptr)
	{
		_currentScene->Awake();
	}
}

void SceneManager::Start()
{
	if (_currentScene != nullptr)
	{
		_currentScene->Start();
	}
}

void SceneManager::FixedUpdate()
{
	// 1. ���� �����ӿ��� ����� �ð� ���
	_deltaTime = TIME->GetDeltaTime();

	// 2. ��� �ð��� ����
	_accumulatedTime += _deltaTime;

	// 3. ������ �������� FixedUpdate ȣ��
	while (_accumulatedTime >= _fixedDeltaTime)
	{
		_currentScene->FixedUpdate();
		_accumulatedTime -= _fixedDeltaTime;
	}
}

void SceneManager::Update()
{
	if (_currentScene == nullptr)
		return;

	_currentScene->Update();
	_currentScene->LateUpdate();

	_currentScene->Render();
}

void SceneManager::AddScene(const std::string& name, std::shared_ptr<Scene> scene)
{
	if (_scenes.find(name) != _scenes.end())
	{
		// �̹� ���� �̸��� ���� �ִٸ� ��� ���
		std::cerr << "Scene \"" << name << "\" already exists!" << std::endl;
		return;
	}

	_scenes[name] = scene;
}

void SceneManager::ChangeScene(const string& name)
{
	auto it = _scenes.find(name);
    if (it == _scenes.end())
    {
        // �ش� �̸��� ���� ���� ��� ��� ���
        std::cerr << "Scene \"" << name << "\" does not exist!" << std::endl;
        return;
    }

    // ���� �� ���� �� �����ֱ� �ʱ�ȭ
    _currentScene = it->second;
    _currentScene->Awake();
    _currentScene->Start();
}
