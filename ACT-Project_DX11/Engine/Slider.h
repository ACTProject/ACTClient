#pragma once
#include "Component.h"
class Slider :
	public Component
{
	using Super = Component;

public:
	Slider();
	virtual ~Slider();

	bool Picked(POINT screenPos);

	void Create(Vec2 screenPos, Vec2 size, shared_ptr<class Material> material);
	void AddOnClickedEvent(std::function<void(void)> func);
	void InvokeOnClicked();

private:
	std::function<void(void)> _onClicked;
	RECT _rect;
};

//class Component
//{
//public:
//	Component(ComponentType type);
//	virtual ~Component();
//
//	virtual void Awake() { }
//	virtual void Start() { }
//	virtual void Update() { }
//	virtual void LateUpdate() { }
//	virtual void FixedUpdate() { }
//
//public:
//	ComponentType GetType() { return _type; }
//
//	shared_ptr<GameObject> GetGameObject();
//	shared_ptr<Transform> GetTransform();
//
//private:
//	friend class GameObject;
//	void SetGameObject(shared_ptr<GameObject> gameObject) { _gameObject = gameObject; }
//
//protected:
//	ComponentType _type;
//	weak_ptr<GameObject> _gameObject;
//};

// scene�� ������Ʈ���� if������ false�� �ϸ鼭 ���ӿ����� ���鼭 �װ� button�̸� invokeonclicked �Լ� ���� ����.
// MeshRenderer::RenderSingle() üũ
