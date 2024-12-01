#pragma once


class Scene
{
public:
	virtual void Awake();		// �ٸ� ������Ʈ�� ������Ʈ�� �����ϱ� ���� ����
	virtual void Start();		// ������Ʈ Ȱ��ȭ�� ���� �ʱ�ȭ ����
	virtual void Update();		// �� �����Ӹ��� ȣ��
	virtual void FixedUpdate(); // ������ �ð� ���ݸ��� ȣ�� (���� ����) 
	virtual void LateUpdate();	// �ٸ� ������Ʈ�� Update()���� ������ �����͸� ������� �۾��� �� ȣ��

	virtual void Render();

	virtual void Add(shared_ptr<GameObject> object);
	virtual void Remove(shared_ptr<GameObject> object);

	void SetPlayer(shared_ptr<GameObject> player) { _player = player; }
	void SetTerrain(shared_ptr<GameObject> terrain) { _terrain = terrain; }

	unordered_set<shared_ptr<GameObject>>& GetObjects() { return _objects; }
	shared_ptr<GameObject> GetMainCamera();
	shared_ptr<GameObject> GetUICamera();
	shared_ptr<GameObject> GetLight() { return _lights.empty() ? nullptr : *_lights.begin(); }
	shared_ptr<GameObject> GetPlayer() { return _player; }
	shared_ptr<GameObject> GetCurrentTerrain() { return _terrain; }

	void UpdateUI();
	shared_ptr<class GameObject> Pick(int32 screenX, int32 screenY);
	Vec3 Picking(int32 screenX, int32 screenY);

	void CheckCollision();

private:
	unordered_set<shared_ptr<GameObject>> _objects;
	// Cache Camera
	unordered_set<shared_ptr<GameObject>> _cameras;
	// Cache Light
	unordered_set<shared_ptr<GameObject>> _lights;

	shared_ptr<GameObject> _player;
	shared_ptr<GameObject> _terrain;	// currentTerrain

};

