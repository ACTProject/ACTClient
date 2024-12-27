#include "pch.h"
#include "RawBuffer.h"
#include "TextureBuffer.h"
#include "Material.h"
#include "Client2.h"
#include "GeometryHelper.h"
#include "Camera.h"
#include "GameObject.h"
#include "CameraController.h"
#include "PlayerController.h"
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
#include "MelleMonsterController.h"
#include "ShootingMonsterController.h"
#include "EnemyManager.h"
#include "Rigidbody.h"
#include "Slider.h"
#include "Skybox.h"
#include "HitBox.h"
#include "Frustum.h"
#include "Shadow.h"
#include "Particle.h"

void Client2::Init()
{
	shared_ptr<Shader> renderShader = make_shared<Shader>(L"23. RenderDemo.fx");
	shared_ptr<Shader> renderBoxShader = make_shared<Shader>(L"23. RenderDemoBox.fx");
	shared_ptr<Shader> renderUIShader = make_shared<Shader>(L"23. RenderDemoUI.fx");
    shared_ptr<Shader> particleShader = make_shared<Shader>(L"Particle.fx");

	// Camera
	{
		auto camera = make_shared<GameObject>();
		camera->GetOrAddTransform()->SetPosition(Vec3(0.f));
		camera->GetOrAddTransform()->SetLocalRotation(Vec3{ XMConvertToRadians(15), 0.f, 0.f });
		camera->AddComponent(make_shared<Camera>());
		{
			camera->GetCamera()->SetCameraOffset(Vec3(0.f, 7.f, -14.f));
		}
		camera->AddComponent(make_shared<CameraController>());
		camera->GetCamera()->SetCullingMaskLayerOnOff(Layer_UI, true);
        camera->GetCamera()->SetMainCamera(true);

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

    //UI_HPBar
    {
        // Material
        {
            shared_ptr<Material> material = make_shared<Material>();
            material->SetShader(renderUIShader);
            auto texture = RESOURCES->Load<Texture>(L"HealBar_Shadow", L"..\\Resources\\Textures\\UI\\BarBorder_Health_Shadow.png");
            material->SetDiffuseMap(texture);
            MaterialDesc& desc = material->GetMaterialDesc();
            desc.ambient = Vec4(1.f);
            desc.diffuse = Vec4(1.f);
            desc.specular = Vec4(1.f);
            RESOURCES->Add(L"HealBar_Shadow", material);
        }
        // Material
        {
            shared_ptr<Material> material = make_shared<Material>();
            material->SetShader(renderUIShader);
            auto texture = RESOURCES->Load<Texture>(L"ArmorBar_Shadow", L"..\\Resources\\Textures\\UI\\BarBorder_Armor_Shadow.png");
            material->SetDiffuseMap(texture);
            MaterialDesc& desc = material->GetMaterialDesc();
            desc.ambient = Vec4(1.f);
            desc.diffuse = Vec4(1.f);
            desc.specular = Vec4(1.f);
            RESOURCES->Add(L"ArmorBar_Shadow", material);
        }
        // Material
        {
            shared_ptr<Material> material = make_shared<Material>();
            material->SetShader(renderUIShader);
            auto texture = RESOURCES->Load<Texture>(L"HealBar", L"..\\Resources\\Textures\\UI\\BarBorder_Health.png");
            material->SetDiffuseMap(texture);
            MaterialDesc& desc = material->GetMaterialDesc();
            desc.ambient = Vec4(1.f);
            desc.diffuse = Vec4(1.f);
            desc.specular = Vec4(1.f);
            RESOURCES->Add(L"HealBar", material);
        }
        // Material
        {
            shared_ptr<Material> material = make_shared<Material>();
            material->SetShader(renderUIShader);
            auto texture = RESOURCES->Load<Texture>(L"ArmorBar", L"..\\Resources\\Textures\\UI\\BarBorder_Armor.png");
            material->SetDiffuseMap(texture);
            MaterialDesc& desc = material->GetMaterialDesc();
            desc.ambient = Vec4(1.f);
            desc.diffuse = Vec4(1.f);
            desc.specular = Vec4(1.f);
            RESOURCES->Add(L"ArmorBar", material);
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
        // Material
        {
            shared_ptr<Material> material = make_shared<Material>();
            material->SetShader(renderUIShader);
            auto texture = RESOURCES->Load<Texture>(L"hpBar", L"..\\Resources\\Textures\\UI\\HP.png");
            material->SetDiffuseMap(texture);
            MaterialDesc& desc = material->GetMaterialDesc();
            desc.ambient = Vec4(1.f);
            desc.diffuse = Vec4(1.f);
            desc.specular = Vec4(1.f);
            RESOURCES->Add(L"hpBar", material);
        }

        // Material
        {
            shared_ptr<Material> material = make_shared<Material>();
            material->SetShader(renderUIShader);
            auto texture = RESOURCES->Load<Texture>(L"BlueBar", L"..\\Resources\\Textures\\UI\\Armor.png");
            material->SetDiffuseMap(texture);
            MaterialDesc& desc = material->GetMaterialDesc();
            desc.ambient = Vec4(1.f);
            desc.diffuse = Vec4(1.f);
            desc.specular = Vec4(1.f);
            RESOURCES->Add(L"BlueBar", material);
        }

        Vec3 healPosition;
        Vec3 armorPosition;;
        healPosition.x = -330.f;
        healPosition.y = -260.f;
        armorPosition.x = healPosition.x + 2.f;
        armorPosition.y = healPosition.y + 40.f;
        // MeshHealBar_Shadow
        {
            auto obj = make_shared<GameObject>();
            obj->SetObjectType(ObjectType::UI);
            obj->GetOrAddTransform()->SetLocalPosition(Vec3(healPosition.x, healPosition.y, 0.2f));
            obj->GetOrAddTransform()->SetScale(Vec3(100, 29.75, 100));
            obj->AddComponent(make_shared<MeshRenderer>());

            obj->SetLayerIndex(Layer_UI);
            {
                obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"HealBar_Shadow"));

            }
            {
                auto mesh = RESOURCES->Get<Mesh>(L"Quad");
                obj->GetMeshRenderer()->SetMesh(mesh);
                obj->GetMeshRenderer()->SetAlphaBlend(true);
                obj->GetMeshRenderer()->SetPass(0);

            }

            CUR_SCENE->Add(obj);
        }
        // MeshArmorBar_Shadow
        {
            auto obj = make_shared<GameObject>();
            obj->SetObjectType(ObjectType::UI);
            obj->GetOrAddTransform()->SetLocalPosition(Vec3(armorPosition.x, armorPosition.y, 0.2f));
            obj->GetOrAddTransform()->SetScale(Vec3(100, 46, 100));
            obj->AddComponent(make_shared<MeshRenderer>());

            obj->SetLayerIndex(Layer_UI);
            {
                obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"ArmorBar_Shadow"));

            }
            {
                auto mesh = RESOURCES->Get<Mesh>(L"Quad");
                obj->GetMeshRenderer()->SetMesh(mesh);
                obj->GetMeshRenderer()->SetAlphaBlend(true);
                obj->GetMeshRenderer()->SetPass(0);
            }

            CUR_SCENE->Add(obj);
        }

        // MeshHealBar
        {
            auto obj = make_shared<GameObject>();
            obj->SetObjectType(ObjectType::UI);
            obj->GetOrAddTransform()->SetLocalPosition(Vec3(healPosition.x, healPosition.y, 0.f));
            obj->GetOrAddTransform()->SetScale(Vec3(100, 29.75, 100));
            obj->AddComponent(make_shared<MeshRenderer>());

            obj->SetLayerIndex(Layer_UI);
            {
                obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"HealBar"));

            }
            {
                auto mesh = RESOURCES->Get<Mesh>(L"Quad");
                obj->GetMeshRenderer()->SetMesh(mesh);
                obj->GetMeshRenderer()->SetAlphaBlend(true);
                obj->GetMeshRenderer()->SetPass(0);

            }

            CUR_SCENE->Add(obj);
        }
        // MeshArmorBar
        {
            auto obj = make_shared<GameObject>();
            obj->SetObjectType(ObjectType::UI);
            obj->GetOrAddTransform()->SetLocalPosition(Vec3(armorPosition.x, armorPosition.y, 0.f));
            obj->GetOrAddTransform()->SetScale(Vec3(100, 46, 100));
            obj->AddComponent(make_shared<MeshRenderer>());

            obj->SetLayerIndex(Layer_UI);
            {
                obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"ArmorBar"));

            }
            {
                auto mesh = RESOURCES->Get<Mesh>(L"Quad");
                obj->GetMeshRenderer()->SetMesh(mesh);
                obj->GetMeshRenderer()->SetAlphaBlend(true);
                obj->GetMeshRenderer()->SetPass(0);
            }

            CUR_SCENE->Add(obj);
        }
        // RedBar HPMesh
        {
            // 슬라이더 컴포넌트 추가.
            auto obj = make_shared<GameObject>();
            obj->SetObjectType(ObjectType::UI);
            obj->AddComponent(make_shared<Slider>());
            obj->GetUI()->Create(Vec3(healPosition.x - 27.f, healPosition.y - 1.f, 0.1f), Vec2(65, 10), RESOURCES->Get<Material>(L"hpBar"));
            obj->GetUI()->SetUIID("HP");
            CUR_SCENE->Add(obj);
        }

        // RedBar ARmor Mesh
        {
            // 슬라이더 컴포넌트 추가.
            auto obj = make_shared<GameObject>();
            obj->SetObjectType(ObjectType::UI);
            obj->AddComponent(make_shared<Slider>());
            obj->GetUI()->Create(Vec3(armorPosition.x - 27.f, armorPosition.y - 9.f, 0.1f), Vec2(65, 10), RESOURCES->Get<Material>(L"BlueBar"));
            obj->GetUI()->SetUIID("Armor");
            CUR_SCENE->Add(obj);
        }
    }

    //Option
    {
        // option Material
        {
            shared_ptr<Material> material = make_shared<Material>();
            material->SetShader(renderUIShader);
            auto texture = RESOURCES->Load<Texture>(L"Option", L"..\\Resources\\Textures\\UI\\option.png");
            material->SetDiffuseMap(texture);
            MaterialDesc& desc = material->GetMaterialDesc();
            desc.ambient = Vec4(1.f);
            desc.diffuse = Vec4(1.f);
            desc.specular = Vec4(1.f);
            RESOURCES->Add(L"Option", material);
        }
        //Button Material
        {
            {
                shared_ptr<Material> material = make_shared<Material>();
                material->SetShader(renderUIShader);
                auto texture = RESOURCES->Load<Texture>(L"Continue", L"..\\Resources\\Textures\\UI\\continue.png");
                material->SetDiffuseMap(texture);
                MaterialDesc& desc = material->GetMaterialDesc();
                desc.ambient = Vec4(1.f);
                desc.diffuse = Vec4(1.f);
                desc.specular = Vec4(1.f);
                RESOURCES->Add(L"Continue", material);
            }
            {
                shared_ptr<Material> material = make_shared<Material>();
                material->SetShader(renderUIShader);
                auto texture = RESOURCES->Load<Texture>(L"GameEnd", L"..\\Resources\\Textures\\UI\\gameEnd.png");
                material->SetDiffuseMap(texture);
                MaterialDesc& desc = material->GetMaterialDesc();
                desc.ambient = Vec4(1.f);
                desc.diffuse = Vec4(1.f);
                desc.specular = Vec4(1.f);
                RESOURCES->Add(L"GameEnd", material);
            }
        }
        // Option Mesh
        {
            vector<shared_ptr<GameObject>> optionGroup;

            {
                auto obj = make_shared<GameObject>();
                obj->AddComponent(make_shared<Button>());

                obj->GetButton()->Create(Vec3(400.f, 250.f, 0.4f), Vec2(595, 404), RESOURCES->Get<Material>(L"Option"));
                obj->GetMeshRenderer()->SetAlphaBlend(true);
                obj->GetButton()->AddOnKeyPressEvent(KEY_TYPE::ESC, [obj]() { obj->SetActive(!obj->IsActive()); });

                obj->SetActive(!obj->IsActive());
                optionGroup.push_back(obj);
               // CUR_SCENE->Add(obj);
            }


            // Mesh
            {
                auto obj = make_shared<GameObject>();
                obj->AddComponent(make_shared<Button>());

                obj->GetButton()->Create(Vec3(532, 359, 0.3f), Vec2(175, 38), nullptr);
                obj->GetMeshRenderer()->SetAlphaBlend(true);
                obj->GetButton()->AddOnHoverEvent([obj]() { obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"GameEnd")); });
                obj->GetButton()->AddOnHoverEndEvent([obj]() { obj->GetMeshRenderer()->SetMaterial(nullptr); });
                obj->GetButton()->AddOnKeyPressEvent(KEY_TYPE::ESC, [obj]() { obj->SetActive(!obj->IsActive()); });
                obj->GetButton()->AddOnClickedEvent([obj]() { if (obj->IsActive())GAME->GameEnd(); });
                
                obj->SetActive(!obj->IsActive());
                optionGroup.push_back(obj);
                //CUR_SCENE->Add(obj);
            }

            // Mesh
            {
                auto obj = make_shared<GameObject>();
                obj->AddComponent(make_shared<Button>());

                obj->GetButton()->Create(Vec3(532, 308, 0.3f), Vec2(175, 38), nullptr);
                obj->GetMeshRenderer()->SetAlphaBlend(true);
                obj->GetButton()->AddOnHoverEvent([obj]() { obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"Continue")); });
                obj->GetButton()->AddOnHoverEndEvent([obj]() { obj->GetMeshRenderer()->SetMaterial(nullptr); });
                obj->GetButton()->AddOnKeyPressEvent(KEY_TYPE::ESC, [obj]() { obj->SetActive(!obj->IsActive()); });
                //obj->GetButton()->AddOnClickedEvent([obj]() { obj->SetVisible(); });
                optionGroup.push_back(obj);
                obj->GetButton()->AddOnClickedEvent([optionGroup]() {
                    if (optionGroup[0]->IsActive())
                    {
                        for (auto& uiObject : optionGroup) {
                            uiObject->SetActive(!uiObject->IsActive());
                        }
                    }
                    });

                obj->SetActive(!obj->IsActive());

               // CUR_SCENE->Add(obj);
            }
        }
    }

	// Light
	{
		auto light = make_shared<GameObject>();
		light->AddComponent(make_shared<Light>());
		LightDesc lightDesc;
		lightDesc.ambient = Vec4(0.4f);
		lightDesc.diffuse = Vec4(1.f);
		lightDesc.specular = Vec4(0.1f);
		lightDesc.direction = Vec3(40.f, 40.f, 0.f);
		light->GetLight()->SetLightDesc(lightDesc);
        SHADOW->SetLight(lightDesc.direction);
		CUR_SCENE->Add(light);
	}



	// Player
    {
        auto player = make_shared<GameObject>();

        player->SetObjectType(ObjectType::Player);
        player->GetOrAddTransform()->SetPosition(Vec3(3, 0, 3));
        player->GetOrAddTransform()->SetLocalRotation(Vec3(0, 0, 0)); // XMConvertToRadians()
        player->GetOrAddTransform()->SetScale(Vec3(0.01f));

        shared_ptr<Model> playerModel = make_shared<Model>();
        // Player::Model
        {
            playerModel->ReadModel(L"Player/Player");
            playerModel->ReadMaterial(L"Player/Player");

            playerModel->ReadAnimation(L"Player/Idle", AnimationState::Idle);
            playerModel->ReadAnimation(L"Player/Walk", AnimationState::Walk);
            playerModel->ReadAnimation(L"Player/Run", AnimationState::Run);
            playerModel->ReadAnimation(L"Player/Crab_Atk_Combo1", AnimationState::Attack1);
            playerModel->ReadAnimation(L"Player/Crab_Atk_Combo2", AnimationState::Attack2);
            playerModel->ReadAnimation(L"Player/Crab_Atk_Combo3", AnimationState::Attack3);
            playerModel->ReadAnimation(L"Player/Crab_Atk_Combo4", AnimationState::Attack4);
            playerModel->ReadAnimation(L"Player/Crab_Jump", AnimationState::Jump);
            playerModel->ReadAnimation(L"Player/Crab_Dodge", AnimationState::Dodge);
            playerModel->ReadAnimation(L"Player/Crab_BlockingIdle", AnimationState::BlockingIdle);
            playerModel->ReadAnimation(L"Player/Crab_BlockingCrawl", AnimationState::BlockingCrawl);
            playerModel->ReadAnimation(L"Player/Crab_Death", AnimationState::Die);

            // Weapon
            shared_ptr<Model> weaponModel = make_shared<Model>();
            {
                // CustomData -> Memory
                weaponModel->ReadModel(L"Fork/Fork");
                weaponModel->ReadMaterial(L"Fork/Fork");
            }
            ModelMesh& weaponMesh = *weaponModel->GetMeshes()[0];
            playerModel->AddDummyBoneAndAttach(weaponMesh, L"Hand_Grip_L", L"WeaponDummy");
        }

        // Player::ModelAnimator
        shared_ptr<ModelAnimator> ma1 = make_shared<ModelAnimator>(renderShader);
        player->AddComponent(ma1);
        {
            player->GetModelAnimator()->SetModel(playerModel);
            player->GetModelAnimator()->SetPass(5);
        }

        // Collider
        auto collider = make_shared<SphereCollider>();
        collider->SetRadius(1.f);
        collider->SetOffset(Vec3(0.f, 1.f, 0.f));
        OCTREE->InsertCollider(collider);
        player->AddComponent(collider);

        // Rigidbody
        shared_ptr<Rigidbody> rigidBody = make_shared<Rigidbody>();
        rigidBody->SetUseGravity(true);
        rigidBody->SetMass(5.0f);
        player->AddComponent(rigidBody);

        COLLISION->AddCollider(collider);
        COLLISION->AddRigidbody(rigidBody);

        // HitBox
        shared_ptr<GameObject> hitboxGO = make_shared<GameObject>();
        shared_ptr<HitBox> hitbox = make_shared<HitBox>();
        hitboxGO->AddComponent(hitbox);
        hitbox->SetOffSet(Vec3(0.f, 0.6f, 0.f));
        hitbox->Craete(player, Vec3(1.5f));
        CUR_SCENE->Add(hitboxGO);

        // Material
        shared_ptr<Material> dustMaterial = make_shared<Material>();
        dustMaterial->SetShader(particleShader);
        auto texture = RESOURCES->Load<Texture>(L"Dust", L"..\\Resources\\Textures\\Effect\\dust+.dds");
        dustMaterial->SetDiffuseMap(texture);
        MaterialDesc& desc = dustMaterial->GetMaterialDesc();
        desc.ambient = Vec4(1.f);
        desc.diffuse = Vec4(1.f);
        desc.specular = Vec4(1.f);
        RESOURCES->Add(L"Dust", dustMaterial);


        // Player::PlayerScript
        shared_ptr<PlayerController> playerScript = make_shared<PlayerController>();

        playerScript->SetPlayer(playerModel);
        playerScript->SetModelAnimator(ma1);
        playerScript->SetHitBox(hitboxGO);
        playerScript->SetDust(dustMaterial);

        player->SetController(playerScript);
        player->AddComponent(playerScript);

        CUR_SCENE->Add(player);
        CUR_SCENE->SetPlayer(player);
    }
	
    //Enemy
    {
        ENEMY->CreateFinalPhase({ 30.0f,0.f,100.0f });
    }
    // Scene2 Terrain Material
        {

            shared_ptr<Material> material = make_shared<Material>();
            material->SetShader(renderShader);
            
            auto texture = RESOURCES->Load<Texture>(L"Alpha", L"..\\Resources\\Textures\\Terrain\\Alpha01.png");
            material->SetDiffuseMap(texture);
            MaterialDesc& desc = material->GetMaterialDesc();
            desc.ambient = Vec4(1.f);
            desc.diffuse = Vec4(1.f);
            desc.specular = Vec4(1.f);
            RESOURCES->Add(L"Alpha", material);
        }

    // 그림자 문제.

    // Map
    {
        auto obj = make_shared<GameObject>();
        {
            obj->SetObjectType(ObjectType::Map);
            obj->GetOrAddTransform()->SetLocalPosition(Vec3(30, 61.0, 260));
            obj->GetOrAddTransform()->SetScale(Vec3(0.03f));

            shared_ptr<Model> objModel = make_shared<Model>();
            // Model
            {
                objModel->ReadModel(L"BossMap/dome");
                objModel->ReadMaterial(L"BossMap/dome");
            }
            shared_ptr<ModelRenderer> ma2 = make_shared<ModelRenderer>(renderShader);
            obj->AddComponent(ma2);
            {
                obj->GetModelRenderer()->SetModel(objModel);
                obj->GetModelRenderer()->SetPass(4);
            }

            CUR_SCENE->Add(obj);
        }
    }


	// Terrain
    {
        auto obj = make_shared<GameObject>();
        obj->AddComponent(make_shared<Terrain>());
        obj->GetTerrain()->Create(80, 100, RESOURCES->Get<Material>(L"Alpha"));
        obj->GetMeshRenderer()->SetPass(3);
        MAP->SetTerrain(obj->GetTerrain());
        CUR_SCENE->SetTerrain(obj);
        CUR_SCENE->Add(obj);
    }

    MAP->ImportMapObj(L"../Resources/MapFile/MapObjectLists02.txt");

   
}

void Client2::Update()
{

}

void Client2::Render()
{

}
