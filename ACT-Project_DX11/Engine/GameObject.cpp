#include "pch.h"
#include "GameObject.h"
#include "MonoBehaviour.h"
#include "Transform.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "ModelRenderer.h"
#include "ModelAnimator.h"
#include "Light.h"
#include "BaseCollider.h"
#include "Terrain.h"
#include "Button.h"
#include "Billboard.h"
#include "SnowBillboard.h"
#include "Rigidbody.h"
#include "Slider.h"
#include "Skybox.h"
#include "HitBox.h"
#include "Bullet.h"
#include "Raycast.h"
#include "Ui.h"
#include "Particle.h"



GameObject::GameObject()
{
	
}

GameObject::~GameObject()
{

}

void GameObject::Awake()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Awake();
	}

	for (shared_ptr<MonoBehaviour>& script : _scripts)
	{
		script->Awake();
	}
}

void GameObject::Start()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Start();
	}

	for (shared_ptr<MonoBehaviour>& script : _scripts)
	{
		script->Start();
	}
}

void GameObject::Update()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Update();
	}

	for (shared_ptr<MonoBehaviour>& script : _scripts)
	{
		script->Update();
	}
}

void GameObject::LateUpdate()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->LateUpdate();
	}

	for (shared_ptr<MonoBehaviour>& script : _scripts)
	{
		script->LateUpdate();
	}
}

void GameObject::FixedUpdate()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->FixedUpdate();
	}

	for (shared_ptr<MonoBehaviour>& script : _scripts)
	{
		script->FixedUpdate();
	}
}

std::shared_ptr<Component> GameObject::GetFixedComponent(ComponentType type)
{
	uint8 index = static_cast<uint8>(type);
	assert(index < FIXED_COMPONENT_COUNT);
	return _components[index];
}

std::shared_ptr<Transform> GameObject::GetTransform()
{
	shared_ptr<Component> component = GetFixedComponent(ComponentType::Transform);
	return static_pointer_cast<Transform>(component);
}

std::shared_ptr<Camera> GameObject::GetCamera()
{
	shared_ptr<Component> component = GetFixedComponent(ComponentType::Camera);
	return static_pointer_cast<Camera>(component);
}

std::shared_ptr<MeshRenderer> GameObject::GetMeshRenderer()
{
	shared_ptr<Component> component = GetFixedComponent(ComponentType::MeshRenderer);
	return static_pointer_cast<MeshRenderer>(component);
}

std::shared_ptr<ModelRenderer> GameObject::GetModelRenderer()
{
	shared_ptr<Component> component = GetFixedComponent(ComponentType::ModelRenderer);
	return static_pointer_cast<ModelRenderer>(component);
}

std::shared_ptr<ModelAnimator> GameObject::GetModelAnimator()
{
	shared_ptr<Component> component = GetFixedComponent(ComponentType::Animator);
	return static_pointer_cast<ModelAnimator>(component);
}

std::shared_ptr<Light> GameObject::GetLight()
{
	shared_ptr<Component> component = GetFixedComponent(ComponentType::Light);
	return static_pointer_cast<Light>(component);
}

std::shared_ptr<BaseCollider> GameObject::GetCollider()
{
	shared_ptr<Component> component = GetFixedComponent(ComponentType::Collider);
	return static_pointer_cast<BaseCollider>(component);
}

std::shared_ptr<Terrain> GameObject::GetTerrain()
{
	shared_ptr<Component> component = GetFixedComponent(ComponentType::Terrain);
	return static_pointer_cast<Terrain>(component);
}

std::shared_ptr<Button> GameObject::GetButton()
{
	shared_ptr<Component> component = GetFixedComponent(ComponentType::Button);
	return static_pointer_cast<Button>(component);
}

std::shared_ptr<Billboard> GameObject::GetBillboard()
{
	shared_ptr<Component> component = GetFixedComponent(ComponentType::BillBoard);
	return static_pointer_cast<Billboard>(component);
}

std::shared_ptr<SnowBillboard> GameObject::GetSnowBillboard()
{
	shared_ptr<Component> component = GetFixedComponent(ComponentType::SnowBillBoard);
	return static_pointer_cast<SnowBillboard>(component);
}

std::shared_ptr<Rigidbody> GameObject::GetRigidbody()
{
	shared_ptr<Component> component = GetFixedComponent(ComponentType::Rigidbody);
	return static_pointer_cast<Rigidbody>(component);
}

std::shared_ptr<Ui> GameObject::GetUI()
{
	shared_ptr<Component> component = GetFixedComponent(ComponentType::UI);
	return static_pointer_cast<Ui>(component);
}

std::shared_ptr<Skybox> GameObject::GetSkybox()
{
	shared_ptr<Component> component = GetFixedComponent(ComponentType::Skybox);
	return static_pointer_cast<Skybox>(component);
}

std::shared_ptr<HitBox> GameObject::GetHitBox()
{
	shared_ptr<Component> component = GetFixedComponent(ComponentType::HitBox);
	return static_pointer_cast<HitBox>(component);
}

std::shared_ptr<Bullet> GameObject::GetBullet()
{
    shared_ptr<Component> component = GetFixedComponent(ComponentType::Bullet);
    return static_pointer_cast<Bullet>(component);
}

std::shared_ptr<Raycast> GameObject::GetRaycast()
{
    shared_ptr<Component> component = GetFixedComponent(ComponentType::Raycast);
    return static_pointer_cast<Raycast>(component);
}

shared_ptr<Particle> GameObject::GetParticle()
{
    shared_ptr<Component> component = GetFixedComponent(ComponentType::Particle);
    return static_pointer_cast<Particle>(component);
}

std::shared_ptr<Transform> GameObject::GetOrAddTransform()
{
	if (GetTransform() == nullptr)
	{
		shared_ptr<Transform> transform = make_shared<Transform>();
		AddComponent(transform);
	}

	return GetTransform();
}

void GameObject::AddComponent(shared_ptr<Component> component)
{
	component->SetGameObject(shared_from_this());

	uint8 index = static_cast<uint8>(component->GetType());
	if (index < FIXED_COMPONENT_COUNT)
	{
		_components[index] = component;
	}
	else
	{
		_scripts.push_back(dynamic_pointer_cast<MonoBehaviour>(component));
	}
}

void GameObject::Destroy()
{
    // 활성화 상태를 비활성화
    _isActive = false;

    OCTREE->RemoveCollider(GetCollider());

    // 모든 컴포넌트 정리
    for (auto& component : _components)
    {
        if (component)
        {
            component.reset(); // 컴포넌트의 shared_ptr 해제
        }
    }

    // MonoBehaviour 스크립트 정리
    _scripts.clear();

    // Controller 해제
    if (_controller)
    {
        _controller.reset();
    }
}