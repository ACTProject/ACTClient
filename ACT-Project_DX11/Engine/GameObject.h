#pragma once
#include "Component.h"
class MonoBehaviour;
class Transform;
class Camera;
class MeshRenderer;
class ModelRenderer;
class ModelAnimator;
class Light;
class BaseCollider;
class Terrain;
class Button;
class Billboard;
class SnowBillboard;
class Rigidbody;
class Slider;
class Skybox;
class HitBox;
class Ui;
class Particle;

enum class GameObjectType : uint8
{
    NONE,
    PLAYER,
    ENEMY,
    TERRAIN,
    ENVIRONMENT,
    UI,
    End
};


class GameObject : public enable_shared_from_this<GameObject>
{
public:
    GameObject(GameObjectType type = GameObjectType::NONE);
	~GameObject();

	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FixedUpdate();

	shared_ptr<Component> GetFixedComponent(ComponentType type);
	shared_ptr<Transform> GetTransform();
	shared_ptr<Camera> GetCamera();
	shared_ptr<MeshRenderer> GetMeshRenderer();
	shared_ptr<ModelRenderer> GetModelRenderer();
	shared_ptr<ModelAnimator> GetModelAnimator();
	shared_ptr<Light> GetLight();
	shared_ptr<BaseCollider> GetCollider();
	shared_ptr<Terrain> GetTerrain();
	shared_ptr<Button> GetButton();
	shared_ptr<Billboard> GetBillboard();
	shared_ptr<SnowBillboard> GetSnowBillboard();
	shared_ptr<Rigidbody> GetRigidbody();
	shared_ptr<Ui> GetUI();
	shared_ptr<Skybox> GetSkybox();
	shared_ptr<HitBox> GetHitBox();
    shared_ptr<Particle> GetParticle();

	shared_ptr<Transform> GetOrAddTransform();
	void AddComponent(shared_ptr<Component> component);

	void SetLayerIndex(uint8 layer) { _layerIndex = layer; }
	uint8 GetLayerIndex() { return _layerIndex; }

    GameObjectType GetGameObjectType() { return _type; };

    void SetVisible() { _isVisible = !_isVisible; }
    bool GetVisible() { return _isVisible; }
protected:
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;
	vector<shared_ptr<MonoBehaviour>> _scripts;

    GameObjectType _type;
	uint8 _layerIndex = 0;

    bool _isVisible = true;
};

