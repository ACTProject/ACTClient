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


void Title::Init()
{
    shared_ptr<Shader> renderShader = make_shared<Shader>(L"23. RenderDemo.fx");
    shared_ptr<Shader> renderUIShader = make_shared<Shader>(L"23. RenderDemoUI.fx");

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

        obj->GetButton()->Create(Vec3(250, 300, 0.3), Vec2(166, 44), RESOURCES->Get<Material>(L"StartBtn"));
        obj->GetMeshRenderer()->SetAlphaBlend(true);
        obj->GetButton()->AddOnHoverEvent([obj]() { obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"StartBtn_Hover")); });
        obj->GetButton()->AddOnHoverEndEvent([obj]() { obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"StartBtn")); });

        obj->GetButton()->AddOnClickedEvent([]() { GAME->ChangeScene(1); });

        CUR_SCENE->Add(obj);
    }
    // Mesh
    {
        auto obj = make_shared<GameObject>();
        obj->AddComponent(make_shared<Button>());

        obj->GetButton()->Create(Vec3(250, 360, 0.3), Vec2(166, 44), RESOURCES->Get<Material>(L"EndBtn"));
        obj->GetMeshRenderer()->SetAlphaBlend(true);
        obj->GetButton()->AddOnHoverEvent([obj]() { obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"EndBtn_Hover")); });
        obj->GetButton()->AddOnHoverEndEvent([obj]() { obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"EndBtn")); });
        obj->GetButton()->AddOnClickedEvent([]() { GAME->GameEnd(); });

        CUR_SCENE->Add(obj);
    }

    // Terrain
    {
        // Material

        shared_ptr<Material> material = make_shared<Material>();
        material->SetShader(renderShader);

        //material->SetShader(tessellationShader);
        auto heightMap = RESOURCES->Load<Texture>(L"Height", L"../Resources/Textures/Terrain/height4.png");
        auto texture = RESOURCES->Load<Texture>(L"Sand", L"..\\Resources\\Textures\\Terrain\\SandMap.png");
        auto textureNormal = RESOURCES->Load<Texture>(L"SandNormal", L"..\\Resources\\Textures\\Terrain\\SandNormalMap.png");
        //auto texture = RESOURCES->Load<Texture>(L"Sand", L"..\\Resources\\Textures\\Terrain\\testTile.png");

        const int32 width = heightMap->GetSize().x;
        const int32 height = heightMap->GetSize().y;

        const DirectX::ScratchImage& info = heightMap->GetInfo();

        // size_t pixelDataSize = info.GetPixelsSize();
        // Replace the old heightmap with the filtered one.
        uint8* pixelBuffer = info.GetPixels();
        std::vector<uint8> expandedPixelBuffer((width + 1) * (height + 1));

        for (int z = 0; z <= height; ++z) {
            for (int x = 0; x <= width; ++x) {
                int idxExpanded = z * (width + 1) + x;

                if (x < width && z < height) {
                    int idxOriginal = z * width + x;
                    expandedPixelBuffer[idxExpanded] = pixelBuffer[idxOriginal];
                }
                else {
                    if (x == width && z != height) {
                        expandedPixelBuffer[idxExpanded] = expandedPixelBuffer[z * (width + 1) + x - 1];
                    }
                    else if (z == height && x != width) {
                        expandedPixelBuffer[idxExpanded] = expandedPixelBuffer[(z - 1) * (width + 1) + x];
                    }
                    else if (x == width && z == height) {
                        expandedPixelBuffer[idxExpanded] = expandedPixelBuffer[(z - 1) * (width + 1) + x - 1];
                    }
                }
            }
        }


        material->SetDiffuseMap(texture);
        material->SetNormalMap(textureNormal);
        MaterialDesc& desc = material->GetMaterialDesc();
        desc.ambient = Vec4(1.f);
        desc.diffuse = Vec4(1.f);
        desc.specular = Vec4(1.f);
        RESOURCES->Add(L"Sand", material);


        auto obj = make_shared<GameObject>();
        obj->AddComponent(make_shared<Terrain>());
        obj->GetTerrain()->Create(width, height, RESOURCES->Get<Material>(L"Sand"));
        {


            vector<VertexTextureNormalTangentData>& v = const_cast<vector<VertexTextureNormalTangentData>&>(obj->GetTerrain()->GetMesh()->GetGeometry()->GetVertices());
            assert(v.size() == (width + 1) * (height + 1));
            for (int32 z = 0; z <= height; z++)
            {
                for (int32 x = 0; x <= width; x++)
                {
                    int32 idx = (width + 1) * z + x;
                    uint8 height = expandedPixelBuffer[idx] / 255.f * 25.f;
                    v[idx].position.y = height - 8.f;
                }
            }

            // Smooth
            float avg = 0.0f;
            float num = 0.0f;

            for (int32 z = 0; z < height; z++)
            {
                for (int32 x = 0; x < width; x++)
                {
                    avg = 0.0f;
                    num = 0.0f;
                    for (int32 m = z - 1; m <= z + 1; ++m) // -1 ~ 1, 0 ~ 2
                    {

                        for (int32 n = x - 1; n <= x + 1; ++n)
                        {

                            if (m >= 0 && m < (int32)height && n >= 0 && n < (int32)width)
                            {
                                avg += v[m * width + n].position.y;
                                num += 1.0f;
                            }
                        }
                    }
                    v[z * height + x].position.y = avg / num;
                }
            }

        }

        obj->GetTerrain()->GetMesh()->GetVertexBuffer()->Create(obj->GetTerrain()->GetMesh()->GetGeometry()->GetVertices());
        obj->GetTerrain()->GetMesh()->GetIndexBuffer()->Create(obj->GetTerrain()->GetMesh()->GetGeometry()->GetIndices());

        MAP->SetTerrain(obj->GetTerrain());
        CUR_SCENE->SetTerrain(obj);
        CUR_SCENE->Add(obj);
    }

}

void Title::Update()
{
}

void Title::Render()
{
}
