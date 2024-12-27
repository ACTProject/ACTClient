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
class Bullet;
class Raycast;
class Particle;

enum class ObjectType
{
    Monster,
    Player,
    Terrain,
    Shell,
    Environment,
    UI,
    Boss2,
    Portal,
    MapMesh,
    Map,
    // ...

    Unknown,
};

class GameObject : public enable_shared_from_this<GameObject>
{
public:
    GameObject();
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
	shared_ptr<Bullet> GetBullet();
	shared_ptr<Raycast> GetRaycast();
    shared_ptr<Particle> GetParticle();

	shared_ptr<Transform> GetOrAddTransform();
	void AddComponent(shared_ptr<Component> component);
    void Destroy();

	void SetLayerIndex(uint8 layer) { _layerIndex = layer; }
	uint8 GetLayerIndex() { return _layerIndex; }

    bool IsActive() { return _isActive; }
    void SetActive(bool isActive) { _isActive = isActive; }

    shared_ptr<MonoBehaviour> GetController() { return _controller; }
    void SetController(shared_ptr<MonoBehaviour> controller) { _controller = controller; }

    ObjectType GetObjectType() const { return _type; }
    void SetObjectType(ObjectType type) { _type = type; }


protected:
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;
	vector<shared_ptr<MonoBehaviour>> _scripts;

	uint8 _layerIndex = 0;
    bool _isActive = true;

private:
    shared_ptr<MonoBehaviour> _controller = nullptr;
    ObjectType _type = ObjectType::Unknown;

};

