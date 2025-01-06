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
#include "MathUtils.h"

void Client::Init()
{
	shared_ptr<Shader> renderShader = make_shared<Shader>(L"23. RenderDemo.fx");
	shared_ptr<Shader> renderBoxShader = make_shared<Shader>(L"23. RenderDemoBox.fx");
	shared_ptr<Shader> renderUIShader = make_shared<Shader>(L"23. RenderDemoUI.fx");
    shared_ptr<Shader> particleShader = make_shared<Shader>(L"Particle.fx");
    shared_ptr<Shader> effectShader = make_shared<Shader>(L"EffectTest.fx");
    shared_ptr<Shader> bubbleShader = make_shared<Shader>(L"Bubble.fx");
    shared_ptr<Shader> bubbleMapShader = make_shared<Shader>(L"BubbleMap.fx");

    // Player
    auto player = make_shared<GameObject>();

    // Sound
    {
        SOUND->Stop(L"bgm");

        // 
        {
            SOUND->Load(L"bgm", L"bgm/Scene1_bgm");
            SOUND->Load(L"openPortal", L"bgm/open_Portal");
        }

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
            SOUND->Load(L"player_footstep", L"player/footstep_sand");
            SOUND->Load(L"player_hit1", L"player/hurt1");
            SOUND->Load(L"player_hit2", L"player/hurt2");
            SOUND->Load(L"player_hit3", L"player/hurt3");
            SOUND->Load(L"player_hit4", L"player/hurt4");
            SOUND->Load(L"player_hurt4", L"player/hurt4");
            SOUND->Load(L"player_jump", L"player/jump");
            SOUND->Load(L"player_excite", L"player/Player_Excite_1");
            SOUND->Load(L"player_pickupItem", L"player/Pickup_Item_World");
            SOUND->Load(L"player_shellConfirm", L"player/ShellConfirm");
            SOUND->Load(L"player_shellBreak", L"player/Armor Break");
            SOUND->Load(L"player_springSound", L"player/ImpactShell_SpringL11");
            SOUND->Load(L"player_impachShell", L"player/ImpactShell_MetalL11");
            SOUND->Load(L"player_scream", L"player/23_Kril_Aaaa");
            SOUND->Load(L"player_enterPortal", L"player/MSS Enter New");
            SOUND->Load(L"player_landing", L"player/Player Land Sand Sweetner 1");
            SOUND->Load(L"player_heal", L"player/HealingMunch4");
            SOUND->Load(L"player_warning", L"player/Health Low Warning");
            SOUND->Load(L"player_aerialAtk_md", L"player/Kril_Yeet_2");
            SOUND->Load(L"player_aerialAtk", L"player/aeiral_attack");
            SOUND->Load(L"player_chargeAtk", L"player/Kril_Heavy_Attack_XL_4");
            SOUND->Load(L"player_chargeUp", L"player/Firth_Charge_Up");
            SOUND->Load(L"player_dashAtk_1", L"player/Kril_Heavy_Attack_XL_2");
            SOUND->Load(L"player_dashAtk_2", L"player/Player Dash Attack NewMain EQ");
            SOUND->Load(L"player_dashAtk_3", L"player/Player Dash Attack NewSlice EQ");
            SOUND->Load(L"player_die", L"player/Kril_Die_Lg_1");
            SOUND->Load(L"player_finish", L"player/08_Kril_3");
            SOUND->Load(L"player_shutup", L"player/shutup");
            SOUND->Load(L"player_intro", L"player/kril_intro");
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
            SOUND->Load(L"shooting_fire_vo", L"monster/shooting/Pistol Shrimp Blaster Fire VO 1");
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

        // CutScene
        Vec3 start(91.7009f, 24.789f, 210.264f);
        Vec3 end(34.1963f, -2.57044f, 17.8424f);
        Vec3 focus(0.283715f, 0.134986f, 0.94936f);
        float duration = 13.0f;

        camera->GetCamera()->StartCutscene(start, end, focus, duration);
      

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
    vector<shared_ptr<GameObject>> armorGroup;
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
            armorGroup.push_back(obj);
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
            armorGroup.push_back(obj);
            CUR_SCENE->Add(obj);
        }

        // ArmorMesh
        {
            // 슬라이더 컴포넌트 추가.
            auto obj = make_shared<GameObject>();
            obj->SetObjectType(ObjectType::UI);
            obj->SetLayerIndex(Layer_UI);
            obj->AddComponent(make_shared<Slider>());
            obj->GetUI()->Create(Vec3(armorPosition.x - 75.f, armorPosition.y - 9.f, 0.1f), Vec2(161, 10), RESOURCES->Get<Material>(L"BlueBar"));
            obj->GetUI()->SetUIID("PlayerArmor");
            armorGroup.push_back(obj);
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
                UIMANAGER->AddButton(obj->GetButton());
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
                UIMANAGER->AddButton(obj->GetButton());
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
                UIMANAGER->AddButton(obj->GetButton());
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
    
    // player
    player->SetObjectType(ObjectType::Player);
	//player->GetOrAddTransform()->SetPosition(Vec3(40, 0, 40));
    player->GetOrAddTransform()->SetPosition(Vec3(424.f - 5.0f, 1.f, 335.f - 5.0f));
    CUR_SCENE->SetMissionClear(true);
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
        playerModel->ReadAnimation(L"Player/Crab_Hit", AnimationState::Hit1);
        playerModel->ReadAnimation(L"Player/Crab_AirAttack", AnimationState::AirAttack);
        playerModel->ReadAnimation(L"Player/Crab_AtkChargeThrust", AnimationState::AtkChargeThrust);
        playerModel->ReadAnimation(L"Player/Crab_DodgeStepback", AnimationState::DodgeStepback);
        playerModel->ReadAnimation(L"Player/Crab_DodgeMedium", AnimationState::DodgeMedium);
        playerModel->ReadAnimation(L"Player/Crab_BlockHit", AnimationState::BlockHit);
        playerModel->ReadAnimation(L"Player/Crab_DashAtk", AnimationState::DashAtk);

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

    // AirHitBox
    shared_ptr<GameObject> airhitboxGO = make_shared<GameObject>();
    shared_ptr<HitBox> airhitbox = make_shared<HitBox>();
    airhitboxGO->AddComponent(airhitbox);
    airhitbox->SetOffSet(Vec3(0.f, 0.6f, 0.f));
    airhitbox->AirHitCraete(player, Vec3(3.5f, 1.0f, 3.5f));
    CUR_SCENE->Add(airhitboxGO);

    // ChargeHitBox
    shared_ptr<GameObject> chargehitboxGO = make_shared<GameObject>();
    shared_ptr<HitBox> chargehitbox = make_shared<HitBox>();
    chargehitboxGO->AddComponent(chargehitbox);
    chargehitbox->SetOffSet(Vec3(0.f, 0.6f, 0.f));
    chargehitbox->Craete(player, Vec3(2.f, 1.f, 2.f));
    CUR_SCENE->Add(chargehitboxGO);

    // DashHitBox
    shared_ptr<GameObject> dashhitboxGO = make_shared<GameObject>();
    shared_ptr<HitBox> dashhitbox = make_shared<HitBox>();
    dashhitboxGO->AddComponent(dashhitbox);
    dashhitbox->SetOffSet(Vec3(0.f, 0.6f, 0.f));
    dashhitbox->Craete(player, Vec3(3.f, 1.f, 3.f));
    CUR_SCENE->Add(dashhitboxGO);

    // Dust Material 생성
    shared_ptr<Material> dustMaterial = make_shared<Material>();
    dustMaterial->SetShader(particleShader);
    {
        auto texture = RESOURCES->Load<Texture>(L"Dust", L"..\\Resources\\Textures\\Effect\\dust+.png");
        dustMaterial->SetDiffuseMap(texture);
        MaterialDesc& desc = dustMaterial->GetMaterialDesc();
        desc.ambient = Vec4(1.f);
        desc.diffuse = Vec4(1.f);
        desc.specular = Vec4(1.f);
        RESOURCES->Add(L"Dust", dustMaterial);
    }

    // Bubble Material 생성
    shared_ptr<Material> bubbleMaterial = make_shared<Material>();
    bubbleMaterial->SetShader(bubbleShader);
    {
        auto texture = RESOURCES->Load<Texture>(L"Bubble", L"..\\Resources\\Textures\\Effect\\bubble.png");
        bubbleMaterial->SetDiffuseMap(texture);
        MaterialDesc& desc = bubbleMaterial->GetMaterialDesc();
        desc.ambient = Vec4(1.f);
        desc.diffuse = Vec4(1.f);
        desc.specular = Vec4(1.f);
        RESOURCES->Add(L"Bubble", bubbleMaterial);
    }
    
    // Effect obj  
    auto effectObj = make_shared<GameObject>();
    effectObj->GetOrAddTransform()->SetLocalPosition(Vec3(0.f));
    effectObj->AddComponent(make_shared<Particle>());
    auto createMaterial = [&](const wstring& materialName, const wstring& texturePath)
        {
            shared_ptr<Material> material = make_shared<Material>();
            material->SetShader(effectShader);
            auto texture = RESOURCES->Load<Texture>(materialName, texturePath);
            material->SetDiffuseMap(texture);

            MaterialDesc& desc = material->GetMaterialDesc();
            desc.ambient = Vec4(1.f);
            desc.diffuse = Vec4(1.f);
            desc.specular = Vec4(1.f);

            RESOURCES->Add(materialName, material);
            effectObj->GetParticle()->SetMaterial(material);
        };
    createMaterial(L"AttackEffect2", L"..\\Resources\\Textures\\Effect\\TestEffect2.png");
    createMaterial(L"AttackEffect", L"..\\Resources\\Textures\\Effect\\TestEffect.png");
    createMaterial(L"AttackEffect3", L"..\\Resources\\Textures\\Effect\\TestEffect3.png");
    createMaterial(L"AttackEffect4", L"..\\Resources\\Textures\\Effect\\TestEffect4.png");
    createMaterial(L"AttackEffect5", L"..\\Resources\\Textures\\Effect\\TestEffect5.png");
    createMaterial(L"AttackEffect6", L"..\\Resources\\Textures\\Effect\\TestEffect6.png");
    createMaterial(L"AttackEffect7", L"..\\Resources\\Textures\\Effect\\TestEffect7.png");
    createMaterial(L"AttackEffect8", L"..\\Resources\\Textures\\Effect\\TestEffect8.png");
    createMaterial(L"AttackEffect9", L"..\\Resources\\Textures\\Effect\\TestEffect9.png");
    createMaterial(L"AttackEffect10", L"..\\Resources\\Textures\\Effect\\TestEffect10.png");

    effectObj->GetParticle()->SetDelayTime(0.4f);
    effectObj->GetParticle()->SetLifetime(0.5f);
    effectObj->GetParticle()->SetfadeStart(0.45f);
    effectObj->GetParticle()->SetReuse(true);
    effectObj->GetParticle()->Add(Vec3(0,0,0), Vec2(4.0f, 4.0f));
    CUR_SCENE->Add(effectObj);
        
    // HitEffect
    auto hitObj = make_shared<GameObject>();
    hitObj->GetOrAddTransform()->SetLocalPosition(Vec3(0.f));
    hitObj->AddComponent(make_shared<Particle>());
    {
        //material
        shared_ptr<Material> material = make_shared<Material>();
        material->SetShader(particleShader);
        auto texture = RESOURCES->Load<Texture>(L"HitEffect", L"..\\Resources\\Textures\\Effect\\HitEffect.png");
        material->SetDiffuseMap(texture);
        MaterialDesc& desc = material->GetMaterialDesc();
        desc.ambient = Vec4(1.f);
        desc.diffuse = Vec4(1.f);
        desc.specular = Vec4(1.f);
        RESOURCES->Add(L"HitEffect", material);

        hitObj->GetParticle()->SetMaterial(material);
    }

    hitObj->GetParticle()->SetLifetime(0.3f);
    hitObj->GetParticle()->SetfadeStart(0.15f);
    hitObj->GetParticle()->SetReuse(true);
    hitObj->GetParticle()->Add(Vec3(0, 0, 0), Vec2(2.0f, 2.0f));
    CUR_SCENE->Add(hitObj);




	// Player::PlayerScript
	shared_ptr<PlayerController> playerScript = make_shared<PlayerController>();

	playerScript->SetPlayer(playerModel);
	playerScript->SetModelAnimator(ma1);
	playerScript->SetHitBox(hitboxGO);
	playerScript->SetAirHitBox(airhitboxGO);
	playerScript->SetChargeHitBox(chargehitboxGO);
	playerScript->SetDashHitBox(dashhitboxGO);
    playerScript->SetDust(dustMaterial);
    playerScript->SetBubble(bubbleMaterial);
    playerScript->SetEffect(effectObj);
    playerScript->SetHitEffect(hitObj);
    playerScript->SetArmorGroup(armorGroup);

    player->SetController(playerScript);
	player->AddComponent(playerScript);

	CUR_SCENE->Add(player);
	CUR_SCENE->SetPlayer(player);


    // Enemy
    {
        int cnt = 0;
        ENEMY->CreateMeleeMonster({ 35.0f, 0.f, 165.0f }, cnt++);
        ENEMY->CreateMeleeMonster({ 80.0f, 0.f, 150.0f }, cnt++);
        ENEMY->CreateMeleeMonster({ 105.0f, 0.f, 105.0f }, cnt++);
        ENEMY->CreateMeleeMonster({ 400.0f, 0.f, 300.0f }, cnt++);
        ENEMY->CreateMeleeMonster({305.0f, 0.f, 130.0f}, cnt++);
        ENEMY->CreateMeleeMonster({ 155.0f, 0.f, 100.0f }, cnt++);
        ENEMY->CreateMeleeMonster({ 365.0f, 0.f, 180.0f }, cnt++);
        ENEMY->CreateMeleeMonster({ 365.0f, 0.f, 285.0f }, cnt++);
        ENEMY->CreateMeleeMonster({ 425.0f, 0.f, 270.0f }, cnt++);

        cnt = 0;
        ENEMY->CreateShootingMonster({ 44.0f, 0.f, 95.0f }, cnt++);
        ENEMY->CreateShootingMonster({ 290.0f, 0.f, 100.0f }, cnt++);
        ENEMY->CreateShootingMonster({ 410.0f, 0.f, 60.0f }, cnt++);
        ENEMY->CreateShootingMonster({ 435.0f, 0.f, 100.0f }, cnt++);
        ENEMY->CreateShootingMonster({ 400.0f, 0.f, 130.0f }, cnt++);
        ENEMY->CreateShootingMonster({ 165.0f, 0.f, 150.0f }, cnt++);
        ENEMY->CreateShootingMonster({ 234.0f, 0.f, 170.0f }, cnt++);
        ENEMY->CreateShootingMonster({ 287.0f, 0.f, 254.0f }, cnt++);
        ENEMY->CreateShootingMonster({ 405.0f, 0.f, 330.0f }, cnt++);

    }
    
    

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
    //Bubble in Map
    {
        int bubbleCount = 150;
        float mapSize = 512.f;
        
        //material
        shared_ptr<Material> bubbleMaterial = make_shared<Material>();
        bubbleMaterial->SetShader(bubbleMapShader);
        auto texture = RESOURCES->Load<Texture>(L"BubbleMap", L"..\\Resources\\Textures\\Effect\\bubble.png");
        bubbleMaterial->SetDiffuseMap(texture);
        MaterialDesc& desc = bubbleMaterial->GetMaterialDesc();
        desc.ambient = Vec4(1.f);
        desc.diffuse = Vec4(1.f);
        desc.specular = Vec4(1.f);
        RESOURCES->Add(L"BubbleMap", bubbleMaterial);
        
        for (int i = 0; i < bubbleCount; i++)
        {
            auto bubbleObj = make_shared<GameObject>();
            bubbleObj->GetOrAddTransform()->SetLocalPosition(Vec3(0.f));
            bubbleObj->AddComponent(make_shared<Particle>());

            bubbleObj->GetParticle()->SetMaterial(bubbleMaterial);
            
            bubbleObj->GetParticle()->SetReuse(true);
            bubbleObj->GetParticle()->SetBubble(true);
            float y = MathUtils::Random(-100.f, -8.f);

            float lifetime = 4 - y / 20;
             
            bubbleObj->GetParticle()->SetLifetime(lifetime);
            bubbleObj->GetParticle()->SetfadeStart(lifetime);
            bubbleObj->GetParticle()->Add(Vec3(MathUtils::Random(0.f,mapSize), y, MathUtils::Random(0.f, mapSize)), Vec2(2.0f, 2.0f));

            CUR_SCENE->Add(bubbleObj);
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
    {
        shared_ptr<Material> material = make_shared<Material>();
        material->SetShader(renderUIShader);
        auto texture = RESOURCES->Load<Texture>(L"0", L"..\\Resources\\Textures\\UI\\0.png");
        material->SetDiffuseMap(texture);
        MaterialDesc& desc = material->GetMaterialDesc();
        desc.ambient = Vec4(1.f);
        desc.diffuse = Vec4(1.f);
        desc.specular = Vec4(1.f);
        RESOURCES->Add(L"0", material);
    }
    {
        shared_ptr<Material> material = make_shared<Material>();
        material->SetShader(renderUIShader);
        auto texture = RESOURCES->Load<Texture>(L"1", L"..\\Resources\\Textures\\UI\\1.png");
        material->SetDiffuseMap(texture);
        MaterialDesc& desc = material->GetMaterialDesc();
        desc.ambient = Vec4(1.f);
        desc.diffuse = Vec4(1.f);
        desc.specular = Vec4(1.f);
        RESOURCES->Add(L"1", material);
    }
    {
        shared_ptr<Material> material = make_shared<Material>();
        material->SetShader(renderUIShader);
        auto texture = RESOURCES->Load<Texture>(L"2", L"..\\Resources\\Textures\\UI\\2.png");
        material->SetDiffuseMap(texture);
        MaterialDesc& desc = material->GetMaterialDesc();
        desc.ambient = Vec4(1.f);
        desc.diffuse = Vec4(1.f);
        desc.specular = Vec4(1.f);
        RESOURCES->Add(L"2", material);
    }
    {
        shared_ptr<Material> material = make_shared<Material>();
        material->SetShader(renderUIShader);
        auto texture = RESOURCES->Load<Texture>(L"3", L"..\\Resources\\Textures\\UI\\3.png");
        material->SetDiffuseMap(texture);
        MaterialDesc& desc = material->GetMaterialDesc();
        desc.ambient = Vec4(1.f);
        desc.diffuse = Vec4(1.f);
        desc.specular = Vec4(1.f);
        RESOURCES->Add(L"3", material);
    }
    {
        shared_ptr<Material> material = make_shared<Material>();
        material->SetShader(renderUIShader);
        auto texture = RESOURCES->Load<Texture>(L"4", L"..\\Resources\\Textures\\UI\\4.png");
        material->SetDiffuseMap(texture);
        MaterialDesc& desc = material->GetMaterialDesc();
        desc.ambient = Vec4(1.f);
        desc.diffuse = Vec4(1.f);
        desc.specular = Vec4(1.f);
        RESOURCES->Add(L"4", material);
    }
    {
        shared_ptr<Material> material = make_shared<Material>();
        material->SetShader(renderUIShader);
        auto texture = RESOURCES->Load<Texture>(L"5", L"..\\Resources\\Textures\\UI\\5.png");
        material->SetDiffuseMap(texture);
        MaterialDesc& desc = material->GetMaterialDesc();
        desc.ambient = Vec4(1.f);
        desc.diffuse = Vec4(1.f);
        desc.specular = Vec4(1.f);
        RESOURCES->Add(L"5", material);
    }
    {
        shared_ptr<Material> material = make_shared<Material>();
        material->SetShader(renderUIShader);
        auto texture = RESOURCES->Load<Texture>(L"6", L"..\\Resources\\Textures\\UI\\6.png");
        material->SetDiffuseMap(texture);
        MaterialDesc& desc = material->GetMaterialDesc();
        desc.ambient = Vec4(1.f);
        desc.diffuse = Vec4(1.f);
        desc.specular = Vec4(1.f);
        RESOURCES->Add(L"6", material);
    }
    {
        shared_ptr<Material> material = make_shared<Material>();
        material->SetShader(renderUIShader);
        auto texture = RESOURCES->Load<Texture>(L"7", L"..\\Resources\\Textures\\UI\\7.png");
        material->SetDiffuseMap(texture);
        MaterialDesc& desc = material->GetMaterialDesc();
        desc.ambient = Vec4(1.f);
        desc.diffuse = Vec4(1.f);
        desc.specular = Vec4(1.f);
        RESOURCES->Add(L"7", material);
    }
    {
        shared_ptr<Material> material = make_shared<Material>();
        material->SetShader(renderUIShader);
        auto texture = RESOURCES->Load<Texture>(L"8", L"..\\Resources\\Textures\\UI\\8.png");
        material->SetDiffuseMap(texture);
        MaterialDesc& desc = material->GetMaterialDesc();
        desc.ambient = Vec4(1.f);
        desc.diffuse = Vec4(1.f);
        desc.specular = Vec4(1.f);
        RESOURCES->Add(L"8", material);
    }
    {
        shared_ptr<Material> material = make_shared<Material>();
        material->SetShader(renderUIShader);
        auto texture = RESOURCES->Load<Texture>(L"9", L"..\\Resources\\Textures\\UI\\9.png");
        material->SetDiffuseMap(texture);
        MaterialDesc& desc = material->GetMaterialDesc();
        desc.ambient = Vec4(1.f);
        desc.diffuse = Vec4(1.f);
        desc.specular = Vec4(1.f);
        RESOURCES->Add(L"9", material);
    }
    {
        shared_ptr<Material> material = make_shared<Material>();
        material->SetShader(renderUIShader);
        auto texture = RESOURCES->Load<Texture>(L"10", L"..\\Resources\\Textures\\UI\\10.png");
        material->SetDiffuseMap(texture);
        MaterialDesc& desc = material->GetMaterialDesc();
        desc.ambient = Vec4(1.f);
        desc.diffuse = Vec4(1.f);
        desc.specular = Vec4(1.f);
        RESOURCES->Add(L"10", material);
    }
    {
        shared_ptr<Material> material = make_shared<Material>();
        material->SetShader(renderUIShader);
        auto texture = RESOURCES->Load<Texture>(L"slash", L"..\\Resources\\Textures\\UI\\slash.png");
        material->SetDiffuseMap(texture);
        MaterialDesc& desc = material->GetMaterialDesc();
        desc.ambient = Vec4(1.f);
        desc.diffuse = Vec4(1.f);
        desc.specular = Vec4(1.f);
        RESOURCES->Add(L"slash", material);
    }


    {
        // 점수판 ui
        auto obj = make_shared<GameObject>();
        obj->SetObjectType(ObjectType::UI);
        obj->AddComponent(make_shared<Ui>(UiType::NONE));
        obj->GetUI()->SetUIID("MissionUI");
        obj->SetLayerIndex(Layer_UI);
        obj->GetOrAddTransform()->SetLocalPosition(Vec3(-310, -190, 0));
        obj->GetOrAddTransform()->SetScale(Vec3(14, 14, 1));
        obj->AddComponent(make_shared<MeshRenderer>());
        obj->SetLayerIndex(Layer_UI);
        {
            obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"0"));
        }
        {
            auto mesh = RESOURCES->Get<Mesh>(L"Quad");
            obj->GetMeshRenderer()->SetMesh(mesh);
            obj->GetMeshRenderer()->SetAlphaBlend(true);
            obj->GetMeshRenderer()->SetPass(0);
        }
        UIMANAGER->AddUI(obj->GetUI()->GetUIID(), obj->GetUI());
        CUR_SCENE->Add(obj);
    }
    {
        // 점수판 ui
        auto obj = make_shared<GameObject>();
        obj->SetObjectType(ObjectType::UI);
        obj->SetLayerIndex(Layer_UI);
        obj->GetOrAddTransform()->SetLocalPosition(Vec3(-290, -190, 0));
        obj->GetOrAddTransform()->SetScale(Vec3(12, 12, 1));
        obj->AddComponent(make_shared<MeshRenderer>());
        obj->SetLayerIndex(Layer_UI);
        {
            obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"slash"));
        }
        {
            auto mesh = RESOURCES->Get<Mesh>(L"Quad");
            obj->GetMeshRenderer()->SetMesh(mesh);
            obj->GetMeshRenderer()->SetAlphaBlend(true);
            obj->GetMeshRenderer()->SetPass(0);
        }
        CUR_SCENE->Add(obj);
    }
    {
        // 점수판 ui
        auto obj = make_shared<GameObject>();
        obj->SetObjectType(ObjectType::UI);
        obj->SetLayerIndex(Layer_UI);
        obj->GetOrAddTransform()->SetLocalPosition(Vec3(-270, -190, 0));
        obj->GetOrAddTransform()->SetScale(Vec3(16, 14, 1));
        obj->AddComponent(make_shared<MeshRenderer>());
        obj->SetLayerIndex(Layer_UI);
        {
            obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"10"));
        }
        {
            auto mesh = RESOURCES->Get<Mesh>(L"Quad");
            obj->GetMeshRenderer()->SetMesh(mesh);
            obj->GetMeshRenderer()->SetAlphaBlend(true);
            obj->GetMeshRenderer()->SetPass(0);
        }
        CUR_SCENE->Add(obj);
    }
}

void Client::Update()
{

}

void Client::Render()
{

}
