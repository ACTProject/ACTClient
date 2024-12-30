#include "pch.h"
#include "RawBuffer.h"
#include "TextureBuffer.h"
#include "Material.h"
#include "Client.h"
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
#include "SoundManager.h"

void Client::Init()
{
	shared_ptr<Shader> renderShader = make_shared<Shader>(L"23. RenderDemo.fx");
	shared_ptr<Shader> renderBoxShader = make_shared<Shader>(L"23. RenderDemoBox.fx");
	shared_ptr<Shader> renderUIShader = make_shared<Shader>(L"23. RenderDemoUI.fx");
    shared_ptr<Shader> particleShader = make_shared<Shader>(L"Particle.fx");
    shared_ptr<Shader> effectShader = make_shared<Shader>(L"EffectTest.fx");

    // Player
    auto player = make_shared<GameObject>();

    // Sound
    {
        SOUND->Release();
        if (!SOUND->Initialize())
        {
            std::cout << "Failed to init SoundManager" << std::endl;
        }

        SOUND->Load(L"bgm", L"bgm/Scene1_bgm");

        //Player
        {
            SOUND->Load(L"player_atk1", L"player/atk_main_1");
            SOUND->Load(L"player_atk2", L"player/atk_main_2");
            SOUND->Load(L"player_atk3", L"player/atk_main_3");
            SOUND->Load(L"player_atk4", L"player/atk_main_4");
            SOUND->Load(L"player_atk1_md", L"player/atk_md_1");
            SOUND->Load(L"player_atk2_md", L"player/atk_md_2");
            SOUND->Load(L"player_atk3_md", L"player/atk_md_3");
            SOUND->Load(L"player_atk4_md", L"player/atk_md_4");
            SOUND->Load(L"player_atk1_sw", L"player/atk_sweetner_1");
            SOUND->Load(L"player_atk2_sw", L"player/atk_sweetner_2");
            SOUND->Load(L"player_atk3_sw", L"player/atk_sweetner_3");
            SOUND->Load(L"player_atk4_sw", L"player/atk_sweetner_4");
            SOUND->Load(L"player_dash", L"player/dash");
            SOUND->Load(L"player_footstep_default", L"player/footstep_default");
            SOUND->Load(L"player_footstep_sand", L"player/footstep_sand");
            SOUND->Load(L"player_hit1", L"player/hurt1");
            SOUND->Load(L"player_hit2", L"player/hurt2");
            SOUND->Load(L"player_hit3", L"player/hurt3");
            SOUND->Load(L"player_hit4", L"player/hurt4");
            SOUND->Load(L"player_hurt4", L"player/hurt4");
            SOUND->Load(L"player_jump", L"player/jump");
            SOUND->Load(L"player_excite", L"player/Player_Excite_1");
            SOUND->Load(L"player_pickupItem", L"player/Pickup_Item_World");
        }

        //Melle
        {
            SOUND->Load(L"melle_aggro", L"monster/melle/aggro");
            SOUND->Load(L"melle_die", L"monster/melle/die");
            SOUND->Load(L"melle_hit", L"monster/melle/hurt");
            SOUND->Load(L"melle_footstep", L"monster/melle/footstep");
            SOUND->Load(L"melle_swing0", L"monster/melle/swing_1");
            SOUND->Load(L"melle_swing1", L"monster/melle/swing_2");
            SOUND->Load(L"melle_swing2", L"monster/melle/slash");
        }
        
        //Shooting
        {
            SOUND->Load(L"shooting_aggro", L"monster/shooting/aggro");
            SOUND->Load(L"shooting_die", L"monster/shooting/die");
            SOUND->Load(L"shooting_fire", L"monster/shooting/fire");
            SOUND->Load(L"shooting_hit", L"monster/shooting/hit");
        }

        SOUND->Play(L"bgm", true);
    }

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
            auto texture = RESOURCES->Load<Texture>(L"RedBar", L"..\\Resources\\Textures\\UI\\MonsterHP.png");
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
        healPosition.x = -270.f;
        healPosition.y = -260.f;
        armorPosition.x = healPosition.x + 2.f;
        armorPosition.y = healPosition.y + 40.f;
      
        // MeshHealBar_Shadow
        {
            auto obj = make_shared<GameObject>();
            obj->SetObjectType(ObjectType::UI);
            obj->SetLayerIndex(Layer_UI);
            obj->GetOrAddTransform()->SetLocalPosition(Vec3(healPosition.x, healPosition.y, 0.2f));
            obj->GetOrAddTransform()->SetScale(Vec3(197.5, 29.75, 100));
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

        // MeshHealBar
        {
            auto obj = make_shared<GameObject>();
            obj->SetObjectType(ObjectType::UI);
            obj->SetLayerIndex(Layer_UI);
            obj->GetOrAddTransform()->SetLocalPosition(Vec3(healPosition.x, healPosition.y, 0.f));
            obj->GetOrAddTransform()->SetScale(Vec3(197.5, 29.75, 100));
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
        // RedBar HPMesh
        {
            // 슬라이더 컴포넌트 추가.
            auto obj = make_shared<GameObject>();
            obj->SetObjectType(ObjectType::UI);
            obj->SetLayerIndex(Layer_UI);
            obj->AddComponent(make_shared<Slider>());
            obj->GetUI()->Create(Vec3(healPosition.x - 75.f, healPosition.y - 1.f, 0.1f), Vec2(161, 10), RESOURCES->Get<Material>(L"hpBar"));
            obj->GetUI()->SetUIID("PlayerHP");

            UIMANAGER->AddUI(obj->GetUI()->GetUIID(), obj->GetUI());

            CUR_SCENE->Add(obj);
        }

        // MeshArmorBar_Shadow
        {
            auto obj = make_shared<GameObject>();
            obj->SetObjectType(ObjectType::UI);
            obj->SetLayerIndex(Layer_UI);
            obj->GetOrAddTransform()->SetPosition(Vec3(armorPosition.x, armorPosition.y, 0.2f));
            obj->GetOrAddTransform()->SetScale(Vec3(197.5, 46, 100));
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

        // MeshArmorBar
        {
            auto obj = make_shared<GameObject>();
            obj->SetObjectType(ObjectType::UI);
            obj->SetLayerIndex(Layer_UI);
            obj->GetOrAddTransform()->SetLocalPosition(Vec3(armorPosition.x, armorPosition.y, 0.f));
            obj->GetOrAddTransform()->SetScale(Vec3(197.5, 46, 100));
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

        // RedBar ArmorMesh
        {
            // 슬라이더 컴포넌트 추가.
            auto obj = make_shared<GameObject>();
            obj->SetObjectType(ObjectType::UI);
            obj->SetLayerIndex(Layer_UI);
            obj->AddComponent(make_shared<Slider>());
            obj->GetUI()->Create(Vec3(armorPosition.x - 75.f, armorPosition.y - 9.f, 0.1f), Vec2(161, 10), RESOURCES->Get<Material>(L"BlueBar"));
            obj->GetUI()->SetUIID("PlayerArmor");
            UIMANAGER->AddUI(obj->GetUI()->GetUIID(), obj->GetUI());
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
    // HitEffect
    {
        
        {
            auto obj = make_shared<GameObject>();
            obj->GetOrAddTransform()->SetLocalPosition(Vec3(0.f));
            obj->AddComponent(make_shared<Particle>());
            {
                //material
                shared_ptr<Material> material = make_shared<Material>();
                material->SetShader(effectShader);
                auto texture = RESOURCES->Load<Texture>(L"HitEffect", L"..\\Resources\\Textures\\Effect\\TestEffect.png");
                material->SetDiffuseMap(texture);
                MaterialDesc& desc = material->GetMaterialDesc();
                desc.ambient = Vec4(1.f);
                desc.diffuse = Vec4(1.f);
                desc.specular = Vec4(1.f);
                RESOURCES->Add(L"HitEffect", material);

                obj->GetParticle()->SetMaterial(material);
            }
            //obj->GetMeshRenderer()->SetParticleRender(true);
            obj->GetParticle()->SetLifetime(0.5f);
            obj->GetParticle()->SetfadeStart(0.25f);
            obj->GetParticle()->SetReuse(true);
            obj->GetParticle()->Add(Vec3(40,0,40), Vec2(5.0f, 5.0f));
            CUR_SCENE->Add(obj);
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
        // 일단 디렉션 임시로 사용.
		lightDesc.direction = Vec3(40.f, 40.f, 0.f);
		light->GetLight()->SetLightDesc(lightDesc);
        SHADOW->SetLight(lightDesc.direction);
		CUR_SCENE->Add(light);
	}

    // Shell
    {
        auto shell = make_shared<GameObject>();
        shell->SetObjectType(ObjectType::Shell);
        shell->GetOrAddTransform()->SetPosition(Vec3(35, -5.f, 35));
        shell->GetOrAddTransform()->SetScale(Vec3(0.01f));

        shared_ptr<Model> shellModel = make_shared<Model>();
        {
            // CustomData -> Memory
            shellModel->ReadModel(L"Shell/Shell_SodaCan");
            shellModel->ReadMaterial(L"Shell/Shell_SodaCan");
        }

        // Shell::ModelRenderer
        shared_ptr<ModelRenderer> mr = make_shared<ModelRenderer>(renderShader);
        shell->AddComponent(mr);
        {
            shell->GetModelRenderer()->SetModel(shellModel);
            shell->GetModelRenderer()->SetPass(1);
        }

        // Collider
        auto collider = make_shared<AABBBoxCollider>();
        collider->SetBoundingBox(BoundingBox(Vec3(0.f), Vec3(0.5f,0.5f,1.f)));
        collider->SetOffset(Vec3(0.f, 1.f, 0.f));
        OCTREE->InsertCollider(collider);
        shell->AddComponent(collider);
        CUR_SCENE->Add(shell);
    }
    // Portal
    {
        auto portal = make_shared<GameObject>();
        portal->SetObjectType(ObjectType::Portal);
        portal->GetOrAddTransform()->SetPosition(Vec3(424.f, 1.f, 335.f));
        portal->GetOrAddTransform()->SetScale(Vec3(0.01f));

        shared_ptr<Model> portalModel = make_shared<Model>();
        {
            portalModel->ReadModel(L"Shell/Shell_SodaCan");
            portalModel->ReadMaterial(L"Shell/Shell_SodaCan");
        }

        shared_ptr<ModelRenderer> mr = make_shared<ModelRenderer>(renderShader);
        portal->AddComponent(mr);
        {
            portal->GetModelRenderer()->SetModel(portalModel);
            portal->GetModelRenderer()->SetPass(1);
        }

        // Collider
        auto collider = make_shared<AABBBoxCollider>();
        collider->SetBoundingBox(BoundingBox(Vec3(0.f), Vec3(10.f, 10.f, 3.f)));
        collider->SetOffset(Vec3(0.f, 1.f, 0.f));
        OCTREE->InsertCollider(collider);
        portal->AddComponent(collider);
        CUR_SCENE->Add(portal);
    }
    // player
    player->SetObjectType(ObjectType::Player);
	player->GetOrAddTransform()->SetPosition(Vec3(40, 0, 40));
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
		player->GetModelAnimator()->SetPass(2);
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
	hitbox->Craete(player, Vec3(2.0f));
	CUR_SCENE->Add(hitboxGO);

    // Dust Material 생성
    shared_ptr<Material> dustMaterial = make_shared<Material>();
    dustMaterial->SetShader(particleShader);
    auto texture = RESOURCES->Load<Texture>(L"Dust", L"..\\Resources\\Textures\\Effect\\dust+.png");
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


	//// Enemy
 //   {
 //       int cnt = 0;
 //       ENEMY->CreateMeleeMonster({ 35.0f, 0.f, 165.0f }, cnt++);
 //       ENEMY->CreateMeleeMonster({ 80.0f, 0.f, 150.0f }, cnt++);
 //       ENEMY->CreateMeleeMonster({ 105.0f, 0.f, 105.0f }, cnt++);
 //       ENEMY->CreateMeleeMonster({ 65.0f, 0.f, 65.0f }, cnt++);
 //       ENEMY->CreateMeleeMonster({305.0f, 0.f, 130.0f}, cnt++);
 //       ENEMY->CreateMeleeMonster({ 155.0f, 0.f, 100.0f }, cnt++);
 //       ENEMY->CreateMeleeMonster({ 365.0f, 0.f, 180.0f }, cnt++);
 //       ENEMY->CreateMeleeMonster({ 365.0f, 0.f, 285.0f }, cnt++);
 //       ENEMY->CreateMeleeMonster({ 425.0f, 0.f, 270.0f }, cnt++);

 //       cnt = 0;
 //       ENEMY->CreateShootingMonster({ 44.0f, 0.f, 95.0f }, cnt++);
 //       ENEMY->CreateShootingMonster({ 290.0f, 0.f, 100.0f }, cnt++);
 //       ENEMY->CreateShootingMonster({ 410.0f, 0.f, 60.0f }, cnt++);
 //       ENEMY->CreateShootingMonster({ 435.0f, 0.f, 100.0f }, cnt++);
 //       ENEMY->CreateShootingMonster({ 400.0f, 0.f, 130.0f }, cnt++);
 //       ENEMY->CreateShootingMonster({ 165.0f, 0.f, 150.0f }, cnt++);
 //       ENEMY->CreateShootingMonster({ 234.0f, 0.f, 170.0f }, cnt++);
 //       ENEMY->CreateShootingMonster({ 287.0f, 0.f, 254.0f }, cnt++);
 //       ENEMY->CreateShootingMonster({ 405.0f, 0.f, 330.0f }, cnt++);

 //   }
    
    

	// Skybox
	{
		// Material
		{
			auto createMaterial = [&](const wstring& texturePath, const wstring& materialName)
			{
				shared_ptr<Material> material = make_shared<Material>();
				material->SetShader(renderBoxShader);
				auto texture = RESOURCES->Load<Texture>(materialName, texturePath);
				material->SetDiffuseMap(texture);

				MaterialDesc& desc = material->GetMaterialDesc();
				desc.ambient = Vec4(1.f);
				desc.diffuse = Vec4(1.f);
				desc.specular = Vec4(1.f);

				RESOURCES->Add(materialName, material);
			};
			createMaterial(L"..\\Resources\\Textures\\SkyBox\\Cubemap_Front.png", L"Cubemap_Front");
			createMaterial(L"..\\Resources\\Textures\\SkyBox\\Cubemap_Back.png", L"Cubemap_Back");
			createMaterial(L"..\\Resources\\Textures\\SkyBox\\Cubemap_Left.png", L"Cubemap_Left");
			createMaterial(L"..\\Resources\\Textures\\SkyBox\\Cubemap_Right.png", L"Cubemap_Right");
			createMaterial(L"..\\Resources\\Textures\\SkyBox\\Cubemap_Top.png", L"Cubemap_Top");
			createMaterial(L"..\\Resources\\Textures\\SkyBox\\Cubemap_Bottom.png", L"Cubemap_Bottom");
		}

		// Mesh
		{
			auto createFace = [&](int32 i, const wstring& materialName)
			{
				auto obj = make_shared<GameObject>();
				obj->GetOrAddTransform()->SetLocalPosition(Vec3(256.f,0.f,256.f));
				obj->GetOrAddTransform()->SetScale(Vec3(550.f));
				obj->AddComponent(make_shared<Skybox>());

				obj->GetSkybox()->Create(i, RESOURCES->Get<Material>(materialName));
				//obj->GetMeshRenderer()->SetMesh(RESOURCES->Get<Mesh>); // Quad mesh for each face
				obj->GetMeshRenderer()->SetPass(0);
                auto ab = RESOURCES->Get<Material>(L"Save");
				CUR_SCENE->Add(obj);
			};
			//앞 뒤 위 아래 왼쪽 오른쪽
			createFace(1, L"Cubemap_Front");   // Front
			createFace(2, L"Cubemap_Back");   // Back
			createFace(3, L"Cubemap_Top");    // Top
			createFace(4, L"Cubemap_Bottom");// Bottom
			createFace(5, L"Cubemap_Left");   // Left
			createFace(6, L"Cubemap_Right");  // Right
		}
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
        std::vector<uint8> expandedPixelBuffer((width + 1)* (height + 1));

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
                    uint8 height = expandedPixelBuffer[idx] / 255.f * 10.f;
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

void Client::Update()
{

}

void Client::Render()
{

}
