#pragma once


class Scene
{
public:
	virtual void Awake();		// 다른 오브젝트나 컴포넌트를 참조하기 전에 실행
	virtual void Start();		// 오브젝트 활성화된 이후 초기화 수행
	virtual void Update();		// 매 프레임마다 호출
	virtual void FixedUpdate(); // 고정된 시간 간격마다 호출 (물리 연산) 
	virtual void LateUpdate();	// 다른 오브젝트가 Update()에서 변경한 데이터를 기반으로 작업할 때 호출

	virtual void Render();

	virtual void Add(shared_ptr<GameObject> object);
	virtual void Remove(shared_ptr<GameObject> object);
    virtual void Clear();

	void SetPlayer(shared_ptr<GameObject> player) { _player = player; }
	void SetEnemy(shared_ptr<GameObject> enemy) { _enemy = enemy; }
	void SetTerrain(shared_ptr<GameObject> terrain) { _terrain = terrain; }

	unordered_set<shared_ptr<GameObject>>& GetObjects() { return _objects; }
	shared_ptr<GameObject> GetMainCamera();
	shared_ptr<GameObject> GetUICamera();
	shared_ptr<GameObject> GetLight() { return _lights.empty() ? nullptr : *_lights.begin(); }
	shared_ptr<GameObject> GetPlayer() { return _player; }
    shared_ptr<GameObject> GetEnemy() { return _enemy; }
	shared_ptr<GameObject> GetCurrentTerrain() { return _terrain; }

	void UpdateUI();
	shared_ptr<class GameObject> Pick(int32 screenX, int32 screenY);
	Vec3 Picking(int32 screenX, int32 screenY);

	void CheckPhysicCollision();

    vector<shared_ptr<GameObject>> FrustumCulling(const vector<shared_ptr<GameObject>>& allObject);

    bool GetMissionClear() { return _missionClear; };
    void SetMissionClear(bool mission) { _missionClear = mission; };
private:
	unordered_set<shared_ptr<GameObject>> _objects;
	// Cache Camera
	unordered_set<shared_ptr<GameObject>> _cameras;
	// Cache Light
	unordered_set<shared_ptr<GameObject>> _lights;

	shared_ptr<GameObject> _player;
    shared_ptr<GameObject> _enemy;
	shared_ptr<GameObject> _terrain;	// currentTerrain

    bool _missionClear = false;
};

