#include "pch.h"
#include "Particle.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "material.h"

Particle::Particle() : Super(ComponentType::Particle)
{
}

Particle::~Particle()
{

}

void Particle::Create(Vec3 screenPos, Vec2 size, shared_ptr<Material> material)
{
    auto go = _gameObject.lock();


    go->GetOrAddTransform()->SetPosition(screenPos);
    go->GetOrAddTransform()->SetScale(Vec3(size.x, size.y, 1));

    if (go->GetMeshRenderer() == nullptr)
        go->AddComponent(make_shared<MeshRenderer>());

    go->GetMeshRenderer()->SetMaterial(material);
    go->GetMeshRenderer()->SetAlphaBlend(true);
    auto mesh = RESOURCES->Get<Mesh>(L"Quad");
    go->GetMeshRenderer()->SetMesh(mesh);
    go->GetMeshRenderer()->SetPass(0);

    _material = material;
}

void Particle::Update()
{
    _elapsedTime += DT;

    ParticleDesc desc;
    desc.time = _elapsedTime;
    desc.fadeStart = _fadeStart;
    desc.lifetime = _lifetime;

    auto shader = _material->GetShader();
    shader->PushParticleData(desc);

    if (_elapsedTime >= _lifetime)
    {
        auto obj = _gameObject.lock();
        if (obj)
        {
            CUR_SCENE->Remove(obj);
        }
        return;
    }

    float scaleFactor = 1.0f + _elapsedTime * 0.001f;
    auto transform = _gameObject.lock()->GetTransform();
    transform->SetScale(transform->GetScale() * scaleFactor);
}
