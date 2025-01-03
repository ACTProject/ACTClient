#include "pch.h"
#include "Title.h"
#include "Camera.h"
#include "Button.h"
#include "RawBuffer.h"
#include "TextureBuffer.h"
#include "Material.h"
#include "GeometryHelper.h"
#include "Camera.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Material.h"
#include "Mesh.h"
#include "Transform.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Light.h"
#include "Graphics.h"
#include "Scene.h"
#include "Camera.h"
#include "Billboard.h"
#include "CameraController.h"
#include "Client.h"
#include "SoundManager.h"


void Title::Init()
{
    shared_ptr<Shader> renderShader = make_shared<Shader>(L"23. RenderDemo.fx");
    shared_ptr<Shader> renderUIShader = make_shared<Shader>(L"23. RenderDemoUI.fx");

    // BGM
    {
        SOUND->Release();
        if (!SOUND->Initialize())
        {
            std::cout << "Failed to init SoundManager" << std::endl;
        }
        SOUND->Load(L"bgm", L"title/Lobby_Sound");
        SOUND->Load(L"hover", L"title/ClickHigh4");
        SOUND->Load(L"click", L"title/UI_Confirm_Soft");

        SOUND->Play(L"bgm", true);
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
        camera->GetCamera()->SetIsTitle(true);
        CUR_SCENE->Add(camera);
    }

    // Material
    {
        shared_ptr<Material> material = make_shared<Material>();
        material->SetShader(renderUIShader);
        auto texture = RESOURCES->Load<Texture>(L"RedBar", L"..\\Resources\\Textures\\UI\\HP.png");
        material->SetDiffuseMap(texture);
        MaterialDesc& desc = material->GetMaterialDesc();
        desc.ambient = Vec4(1.f);
        desc.diffuse = Vec4(1.f);
        desc.specular = Vec4(1.f);
        RESOURCES->Add(L"RedBar", material);
    }

    // BackGround Material
    {
        shared_ptr<Material> material = make_shared<Material>();
        material->SetShader(renderUIShader);
        auto texture = RESOURCES->Load<Texture>(L"Background", L"..\\Resources\\Textures\\UI\\title_background_8x6.png");
        material->SetDiffuseMap(texture);
        MaterialDesc& desc = material->GetMaterialDesc();
        desc.ambient = Vec4(1.f);
        desc.diffuse = Vec4(1.f);
        desc.specular = Vec4(1.f);
        RESOURCES->Add(L"Background", material);
    }

    // Logo Material
    {
        shared_ptr<Material> material = make_shared<Material>();
        material->SetShader(renderUIShader);
        auto texture = RESOURCES->Load<Texture>(L"Logo", L"..\\Resources\\Textures\\UI\\title_logo.png");
        material->SetDiffuseMap(texture);
        MaterialDesc& desc = material->GetMaterialDesc();
        desc.ambient = Vec4(1.f);
        desc.diffuse = Vec4(1.f);
        desc.specular = Vec4(1.f);
        RESOURCES->Add(L"Logo", material);
    }

    // Button Material
    {
        {
            shared_ptr<Material> material = make_shared<Material>();
            material->SetShader(renderUIShader);
            auto texture = RESOURCES->Load<Texture>(L"StartBtn", L"..\\Resources\\Textures\\UI\\Start_0.png");
            material->SetDiffuseMap(texture);
            MaterialDesc& desc = material->GetMaterialDesc();
            desc.ambient = Vec4(1.f);
            desc.diffuse = Vec4(1.f);
            desc.specular = Vec4(1.f);
            RESOURCES->Add(L"StartBtn", material);
        }
        {
            shared_ptr<Material> material = make_shared<Material>();
            material->SetShader(renderUIShader);
            auto texture = RESOURCES->Load<Texture>(L"StartBtn_Hover", L"..\\Resources\\Textures\\UI\\Start_1.png");
            material->SetDiffuseMap(texture);
            MaterialDesc& desc = material->GetMaterialDesc();
            desc.ambient = Vec4(1.f);
            desc.diffuse = Vec4(1.f);
            desc.specular = Vec4(1.f);
            RESOURCES->Add(L"StartBtn_Hover", material);
        }
        {
            shared_ptr<Material> material = make_shared<Material>();
            material->SetShader(renderUIShader);
            auto texture = RESOURCES->Load<Texture>(L"EndBtn", L"..\\Resources\\Textures\\UI\\End_0.png");
            material->SetDiffuseMap(texture);
            MaterialDesc& desc = material->GetMaterialDesc();
            desc.ambient = Vec4(1.f);
            desc.diffuse = Vec4(1.f);
            desc.specular = Vec4(1.f);
            RESOURCES->Add(L"EndBtn", material);
        }
        {
            shared_ptr<Material> material = make_shared<Material>();
            material->SetShader(renderUIShader);
            auto texture = RESOURCES->Load<Texture>(L"EndBtn_Hover", L"..\\Resources\\Textures\\UI\\End_1.png");
            material->SetDiffuseMap(texture);
            MaterialDesc& desc = material->GetMaterialDesc();
            desc.ambient = Vec4(1.f);
            desc.diffuse = Vec4(1.f);
            desc.specular = Vec4(1.f);
            RESOURCES->Add(L"EndBtn_Hover", material);
        }
    }
    // Mesh
    {
        auto obj = make_shared<GameObject>();
        obj->GetOrAddTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.5f));
        obj->GetOrAddTransform()->SetScale(Vec3(800.f, 600.f, 0.f));
        obj->AddComponent(make_shared<MeshRenderer>());

        obj->SetLayerIndex(Layer_UI);
        {
            obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"Background"));
        }
        {
            auto mesh = RESOURCES->Get<Mesh>(L"Quad");
            obj->GetMeshRenderer()->SetMesh(mesh);
            obj->GetMeshRenderer()->SetPass(0);
        }

        CUR_SCENE->Add(obj);
    }

    // Mesh
    {
        auto obj = make_shared<GameObject>();
        obj->GetOrAddTransform()->SetLocalPosition(Vec3(-180.f, 170.f, 0.4f));
        obj->GetOrAddTransform()->SetScale(Vec3(400.f, 225.f, 0.f));
        obj->AddComponent(make_shared<MeshRenderer>());

        obj->SetLayerIndex(Layer_UI);
        {
            obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"Logo"));
        }
        {
            auto mesh = RESOURCES->Get<Mesh>(L"Quad");
            obj->GetMeshRenderer()->SetMesh(mesh);
            obj->GetMeshRenderer()->SetPass(0);
            obj->GetMeshRenderer()->SetAlphaBlend(true);
        }

        CUR_SCENE->Add(obj);
    }


    // Mesh
    {
        auto obj = make_shared<GameObject>();
        obj->AddComponent(make_shared<Button>());
        UIMANAGER->AddButton(obj->GetButton());
        obj->GetButton()->Create(Vec3(250, 300, 0.3), Vec2(166, 44), RESOURCES->Get<Material>(L"StartBtn"));
        obj->GetMeshRenderer()->SetAlphaBlend(true);
        obj->GetButton()->AddOnHoverEvent([obj]() { 
            obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"StartBtn_Hover"));
            SOUND->PlayEffect(L"hover");
            });
        obj->GetButton()->AddOnHoverEndEvent([obj]() { 
            obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"StartBtn")); 
            });

        obj->GetButton()->AddOnClickedEvent([]() { 
            GAME->ChangeScene(1);
            SOUND->PlayEffect(L"click");
            });

        CUR_SCENE->Add(obj);
    }
    // Mesh
    {
        auto obj = make_shared<GameObject>();
        obj->AddComponent(make_shared<Button>());
        UIMANAGER->AddButton(obj->GetButton());
        obj->GetButton()->Create(Vec3(250, 360, 0.3), Vec2(166, 44), RESOURCES->Get<Material>(L"EndBtn"));
        obj->GetMeshRenderer()->SetAlphaBlend(true);
        obj->GetButton()->AddOnHoverEvent([obj]() { 
            obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"EndBtn_Hover"));
            SOUND->PlayEffect(L"hover");
            });
        obj->GetButton()->AddOnHoverEndEvent([obj]() {
            obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"EndBtn"));
            });
        obj->GetButton()->AddOnClickedEvent([]() { 
            GAME->GameEnd();
            SOUND->PlayEffect(L"click"); 
            });

        CUR_SCENE->Add(obj);
    }

    {
      auto obj = make_shared<GameObject>();
      obj->AddComponent(make_shared<Terrain>());
      obj->GetTerrain()->Create(1, 1, NULL);
      CUR_SCENE->SetTerrain(obj);
    }

}

void Title::Update()
{
}

void Title::Render()
{
}
