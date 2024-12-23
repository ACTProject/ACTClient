#include "pch.h"
#include "RawBuffer.h"
#include "TextureBuffer.h"
#include "Material.h"
#include "EffectTest.h"
#include "GeometryHelper.h"
#include "Camera.h"
#include "GameObject.h"
#include "CameraScript.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Material.h"
#include "Model.h"
#include "ModelRenderer.h"
#include "ModelAnimator.h"
#include "Mesh.h"
#include "Transform.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Light.h"
#include "Graphics.h"
#include "SphereCollider.h"
#include "Scene.h"
#include "AABBBoxCollider.h"
#include "OBBBoxCollider.h"
#include "Terrain.h"
#include "Camera.h"
#include "Button.h"
#include "Billboard.h"
#include "Particle.h"

void EffectTest::Init()
{
    shared_ptr<Shader> shader = make_shared<Shader>(L"Particle.fx");

    // Camera
    {
        auto camera = make_shared<GameObject>();
        camera->GetOrAddTransform()->SetPosition(Vec3{ 0.f, 0.f, -5.f });
        camera->AddComponent(make_shared<Camera>());
        camera->AddComponent(make_shared<CameraScript>());
        camera->GetCamera()->SetCullingMaskLayerOnOff(Layer_UI, true);
        CUR_SCENE->Add(camera);
    }

    // UI_Camera
    {
        auto camera = make_shared<GameObject>();
        camera->GetOrAddTransform()->SetPosition(Vec3{ 0.f, 0.f, -5.f });
        camera->AddComponent(make_shared<Camera>());
        camera->GetCamera()->SetProjectionType(ProjectionType::Orthographic);
        camera->GetCamera()->SetNear(1.f);
        camera->GetCamera()->SetFar(100.f);

        camera->GetCamera()->SetCullingMaskAll();
        camera->GetCamera()->SetCullingMaskLayerOnOff(Layer_UI, false);
        CUR_SCENE->Add(camera);
    }

    // Light
    {
        auto light = make_shared<GameObject>();
        light->AddComponent(make_shared<Light>());
        LightDesc lightDesc;
        lightDesc.ambient = Vec4(0.4f);
        lightDesc.diffuse = Vec4(1.f);
        lightDesc.specular = Vec4(0.1f);
        lightDesc.direction = Vec3(1.f, 0.f, 1.f);
        light->GetLight()->SetLightDesc(lightDesc);
        CUR_SCENE->Add(light);
    }

    // Material
    {
        shared_ptr<Material> material = make_shared<Material>();
        material->SetShader(shader);
        auto texture = RESOURCES->Load<Texture>(L"Veigar", L"..\\Resources\\Textures\\veigar.jpg");
        material->SetDiffuseMap(texture);
        MaterialDesc& desc = material->GetMaterialDesc();
        desc.ambient = Vec4(1.f);
        desc.diffuse = Vec4(1.f);
        desc.specular = Vec4(1.f);
        RESOURCES->Add(L"Veigar", material);
    }
    // Material
    {
        shared_ptr<Material> material = make_shared<Material>();
        material->SetShader(shader);
        auto texture = RESOURCES->Load<Texture>(L"Dust", L"..\\Resources\\Textures\\Effect\\dust+.dds");
        material->SetDiffuseMap(texture);
        MaterialDesc& desc = material->GetMaterialDesc();
        desc.ambient = Vec4(1.f);
        desc.diffuse = Vec4(1.f);
        desc.specular = Vec4(1.f);
        RESOURCES->Add(L"Dust", material);
    }


    // Mesh
    {
        auto obj = make_shared<GameObject>();
        obj->AddComponent(make_shared<Particle>());
        
        obj->GetParticle()->Create(Vec3(0, 0, -2), Vec2(2, 2), RESOURCES->Get<Material>(L"Dust"));

        CUR_SCENE->Add(obj);
    }
    // Mesh
    {
        auto obj = make_shared<GameObject>();
        obj->GetOrAddTransform()->SetLocalPosition(Vec3(0.f));
        obj->GetOrAddTransform()->SetScale(Vec3(2.f));
        obj->AddComponent(make_shared<MeshRenderer>());
        {
            obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"Veigar"));
        }
        {
            auto mesh = RESOURCES->Get<Mesh>(L"Cube");
            obj->GetMeshRenderer()->SetMesh(mesh);
            obj->GetMeshRenderer()->SetPass(0);
        }

        CUR_SCENE->Add(obj);
    }

    //// Mesh
    //{
    //    auto obj = make_shared<GameObject>();
    //    obj->AddComponent(make_shared<Button>());

    //    obj->GetButton()->Create(Vec3(100, 100, 1), Vec2(100, 100), RESOURCES->Get<Material>(L"Veigar"));

    //    obj->GetButton()->AddOnClickedEvent([obj]() { CUR_SCENE->Remove(obj); });

    //    CUR_SCENE->Add(obj);
    //}
	
}

void EffectTest::Update()
{
	
}

void EffectTest::Render()
{

}
