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
#include "SoundManager.h"
#include "MathUtils.h"

void Client2::Init()
{
	shared_ptr<Shader> renderShader = make_shared<Shader>(L"23. RenderDemo.fx");
	shared_ptr<Shader> renderBoxShader = make_shared<Shader>(L"23. RenderDemoBox.fx");
	shared_ptr<Shader> renderUIShader = make_shared<Shader>(L"23. RenderDemoUI.fx");
    shared_ptr<Shader> particleShader = make_shared<Shader>(L"Particle.fx");
    shared_ptr<Shader> effectShader = make_shared<Shader>(L"EffectTest.fx");
    shared_ptr<Shader> bubbleShader = make_shared<Shader>(L"Bubble.fx");
    shared_ptr<Shader> bubbleMapShader = make_shared<Shader>(L"BubbleMap.fx");

    // SOUND
    {
        SOUND->Stop(L"bgm");
        SOUND->Load(L"bgm", L"bgm/Nephro Draft ogg");

        SOUND->Load(L"player_footstep", L"player/Footstep Stone 3");

        //Boss
        {
            SOUND->Load(L"boss_footstep1", L"monster/boss/sbc_footstep1");
            SOUND->Load(L"boss_footstep2", L"monster/boss/sbc_footstep2");
            SOUND->Load(L"boss_footstep3", L"monster/boss/sbc_footstep3");
            SOUND->Load(L"boss_footstep4", L"monster/boss/sbc_footstep4");
            SOUND->Load(L"boss_punch1", L"monster/boss/punch1");
            SOUND->Load(L"boss_punch2", L"monster/boss/punch2");
            SOUND->Load(L"boss_punch3", L"monster/boss/punch3");
            SOUND->Load(L"boss_punch4", L"monster/boss/punch4");
            SOUND->Load(L"boss_punch_vo1", L"monster/boss/punch_vo1");
            SOUND->Load(L"boss_punch_vo2", L"monster/boss/punch_vo2");
            SOUND->Load(L"boss_punch_vo3", L"monster/boss/punch_vo3");
            SOUND->Load(L"boss_bubbleMove", L"monster/boss/bubbleBulletMovement");
            SOUND->Load(L"boss_bubbleSpawn", L"monster/boss/bubbleBulletSpawn");
            SOUND->Load(L"boss_bubbleBullet_vo", L"monster/boss/bubbleBullet_vo");
            SOUND->Load(L"boss_moenySpawn", L"monster/boss/moenyFire");
            SOUND->Load(L"boss_moenyMove", L"monster/boss/moneyMovement");
            SOUND->Load(L"boss_slam", L"monster/boss/Nephro EffortHeavy 3");
            SOUND->Load(L"boss_slam2", L"monster/boss/Bomb Explode 1");
            SOUND->Load(L"boss_atk1", L"monster/boss/Nephro_Attack_Long_1");
            SOUND->Load(L"boss_down", L"monster/boss/down");
            SOUND->Load(L"boss_die", L"monster/boss/die");
            SOUND->Load(L"boss_hit1", L"monster/boss/hit1");
            SOUND->Load(L"boss_hit2", L"monster/boss/Nephro_Hurt_1");
            SOUND->Load(L"boss_hit3", L"monster/boss/Nephro_Hurt_3");
            SOUND->Load(L"boss_narrate_9", L"monster/boss/narrate_init");
            SOUND->Load(L"boss_narrate_8", L"monster/boss/Nephro Intro 1");
            SOUND->Load(L"boss_narrate_laugh_short", L"monster/boss/narrate_laugh_short");
            SOUND->Load(L"boss_narrate_laugh_full", L"monster/boss/narrate_laugh_full_1");
            SOUND->Load(L"boss_narrate_laugh_full2", L"monster/boss/narrate_laugh_full_2");
            SOUND->Load(L"boss_narrate_1", L"monster/boss/narrate_1");
            SOUND->Load(L"boss_narrate_2", L"monster/boss/narrate_2");
            SOUND->Load(L"boss_narrate_3", L"monster/boss/narrate_3");
            SOUND->Load(L"boss_narrate_4", L"monster/boss/narrate_4");
            SOUND->Load(L"boss_narrate_5", L"monster/boss/narrate_5");
            SOUND->Load(L"boss_narrate_6", L"monster/boss/narrate_6");
            SOUND->Load(L"boss_narrate_7", L"monster/boss/narrate_7");
            SOUND->Load(L"boss_roar_1", L"monster/boss/heikea_roar");
            SOUND->Load(L"boss_roar_2", L"monster/boss/roar_2");
            SOUND->Load(L"boss_roar_3", L"monster/boss/roar_3");
            SOUND->Load(L"boss_hurricane", L"monster/boss/FirthIntro_Wind");
            SOUND->Load(L"boss_hurricane_vo", L"monster/boss/Nephro EffortHeavy 5");
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
        Vec3 start(43.4162f, 5.72609f, 77.2965f);
        Vec3 end(33.0786f, 6.01206f, 51.8149f);
        Vec3 focus(0.375908, -0.0103984f, 0.926599f);
        float duration = 7.0f;

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
        // MeshArmorBar_Shadow
        {
            auto obj = make_shared<GameObject>();
            obj->SetObjectType(ObjectType::UI);
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
                armorGroup.push_back(obj);
            }

            CUR_SCENE->Add(obj);
        }

        // MeshHealBar
        {
            auto obj = make_shared<GameObject>();
            obj->SetObjectType(ObjectType::UI);
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
        // MeshArmorBar
        {
            auto obj = make_shared<GameObject>();
            obj->SetObjectType(ObjectType::UI);
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
                armorGroup.push_back(obj);
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

        // RedBar ArmorMesh
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

    // Boss HP UI
    {
        // BossBarLeft Material
        {
            shared_ptr<Material> material = make_shared<Material>();
            material->SetShader(renderUIShader);
            auto texture = RESOURCES->Load<Texture>(L"BossBarLeft", L"..\\Resources\\Textures\\UI\\BossBar1.png");
            material->SetDiffuseMap(texture);
            MaterialDesc& desc = material->GetMaterialDesc();
            desc.ambient = Vec4(1.f);
            desc.diffuse = Vec4(1.f);
            desc.specular = Vec4(1.f);
            RESOURCES->Add(L"BossBarLeft", material);
        }
        // BossBarRight Material
        {
            shared_ptr<Material> material = make_shared<Material>();
            material->SetShader(renderUIShader);
            auto texture = RESOURCES->Load<Texture>(L"BossBarRight", L"..\\Resources\\Textures\\UI\\BossBar2.png");
            material->SetDiffuseMap(texture);
            MaterialDesc& desc = material->GetMaterialDesc();
            desc.ambient = Vec4(1.f);
            desc.diffuse = Vec4(1.f);
            desc.specular = Vec4(1.f);
            RESOURCES->Add(L"BossBarRight", material);
        }
        // BossName Material
        {
            shared_ptr<Material> material = make_shared<Material>();
            material->SetShader(renderUIShader);
            auto texture = RESOURCES->Load<Texture>(L"BossName", L"..\\Resources\\Textures\\UI\\BossName.png");
            material->SetDiffuseMap(texture);
            MaterialDesc& desc = material->GetMaterialDesc();
            desc.ambient = Vec4(1.f);
            desc.diffuse = Vec4(1.f);
            desc.specular = Vec4(1.f);
            RESOURCES->Add(L"BossName", material);
        }

        // BossBarLeft Object
        {
            auto obj = make_shared<GameObject>();
            obj->SetObjectType(ObjectType::UI);
            obj->GetOrAddTransform()->SetLocalPosition(Vec3(-270, 253, 0.f));
            obj->GetOrAddTransform()->SetScale(Vec3(60));
            obj->AddComponent(make_shared<MeshRenderer>());

            obj->SetLayerIndex(Layer_UI);
            {
                obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"BossBarLeft"));

            }
            {
                auto mesh = RESOURCES->Get<Mesh>(L"Quad");
                obj->GetMeshRenderer()->SetMesh(mesh);
                obj->GetMeshRenderer()->SetAlphaBlend(true);
                obj->GetMeshRenderer()->SetPass(0);
            }

            CUR_SCENE->Add(obj);
        }

        // BossBarRight Object
        {
            auto obj = make_shared<GameObject>();
            obj->SetObjectType(ObjectType::UI);
            obj->GetOrAddTransform()->SetLocalPosition(Vec3(270, 253, 0.f));
            obj->GetOrAddTransform()->SetScale(Vec3(60));
            obj->AddComponent(make_shared<MeshRenderer>());

            obj->SetLayerIndex(Layer_UI);
            {
                obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"BossBarRight"));

            }
            {
                auto mesh = RESOURCES->Get<Mesh>(L"Quad");
                obj->GetMeshRenderer()->SetMesh(mesh);
                obj->GetMeshRenderer()->SetAlphaBlend(true);
                obj->GetMeshRenderer()->SetPass(0);
            }

            CUR_SCENE->Add(obj);
        }

        // BossName Object
        {
            auto obj = make_shared<GameObject>();
            obj->SetObjectType(ObjectType::UI);
            obj->GetOrAddTransform()->SetLocalPosition(Vec3(0, 270, 0.f));
            obj->GetOrAddTransform()->SetScale(Vec3(98.25, 27, 1));
            obj->AddComponent(make_shared<MeshRenderer>());

            obj->SetLayerIndex(Layer_UI);
            {
                obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"BossName"));

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
    createMaterial(L"AttackEffect11", L"..\\Resources\\Textures\\Effect\\TestEffect11.png");

    effectObj->GetParticle()->SetDelayTime(0.4f);
    effectObj->GetParticle()->SetLifetime(0.5f);
    effectObj->GetParticle()->SetfadeStart(0.45f);
    effectObj->GetParticle()->SetReuse(true);
    effectObj->GetParticle()->Add(Vec3(0, 0, 0), Vec2(4.0f, 4.0f));
    CUR_SCENE->Add(effectObj);


    // HitEffect
    auto hitObj = make_shared<GameObject>();
    hitObj->GetOrAddTransform()->SetLocalPosition(Vec3(0.f));
    hitObj->AddComponent(make_shared<Particle>());
    {
        //material
        shared_ptr<Material> material = make_shared<Material>();
        material->SetShader(effectShader);
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

    // Shell
    {
        auto shell = make_shared<GameObject>();
        shell->SetObjectType(ObjectType::Shell);
        shell->GetOrAddTransform()->SetPosition(Vec3(12.f, -0.5f, -3.f));
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
        collider->SetBoundingBox(BoundingBox(Vec3(0.f), Vec3(0.5f, 0.5f, 1.f)));
        collider->SetOffset(Vec3(0.f, 1.f, 0.f));
        OCTREE->InsertCollider(collider);
        shell->AddComponent(collider);
        CUR_SCENE->Add(shell);
    }
    // Shell
    {
        auto shell = make_shared<GameObject>();
        shell->SetObjectType(ObjectType::Shell);
        shell->GetOrAddTransform()->SetPosition(Vec3(-18.f, -0.5f, 22.f));
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
        collider->SetBoundingBox(BoundingBox(Vec3(0.f), Vec3(0.5f, 0.5f, 1.f)));
        collider->SetOffset(Vec3(0.f, 1.f, 0.f));
        OCTREE->InsertCollider(collider);
        shell->AddComponent(collider);
        CUR_SCENE->Add(shell);
    }

    // Shell
    {
        auto shell = make_shared<GameObject>();
        shell->SetObjectType(ObjectType::Shell);
        shell->GetOrAddTransform()->SetPosition(Vec3(-25.f, -0.5f, 63.f));
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
        collider->SetBoundingBox(BoundingBox(Vec3(0.f), Vec3(0.5f, 0.5f, 1.f)));
        collider->SetOffset(Vec3(0.f, 1.f, 0.f));
        OCTREE->InsertCollider(collider);
        shell->AddComponent(collider);
        CUR_SCENE->Add(shell);
    }

    // Shell
    {
        auto shell = make_shared<GameObject>();
        shell->SetObjectType(ObjectType::Shell);
        shell->GetOrAddTransform()->SetPosition(Vec3(3.f, -0.5f, 98.f));
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
        collider->SetBoundingBox(BoundingBox(Vec3(0.f), Vec3(0.5f, 0.5f, 1.f)));
        collider->SetOffset(Vec3(0.f, 1.f, 0.f));
        OCTREE->InsertCollider(collider);
        shell->AddComponent(collider);
        CUR_SCENE->Add(shell);
    }

    // Shell
    {
        auto shell = make_shared<GameObject>();
        shell->SetObjectType(ObjectType::Shell);
        shell->GetOrAddTransform()->SetPosition(Vec3(82.f, -0.5f, 76.f));
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
        collider->SetBoundingBox(BoundingBox(Vec3(0.f), Vec3(0.5f, 0.5f, 1.f)));
        collider->SetOffset(Vec3(0.f, 1.f, 0.f));
        OCTREE->InsertCollider(collider);
        shell->AddComponent(collider);
        CUR_SCENE->Add(shell);
    }

    // Shell
    {
        auto shell = make_shared<GameObject>();
        shell->SetObjectType(ObjectType::Shell);
        shell->GetOrAddTransform()->SetPosition(Vec3(88.f, -0.5f, 33.f));
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
        collider->SetBoundingBox(BoundingBox(Vec3(0.f), Vec3(0.5f, 0.5f, 1.f)));
        collider->SetOffset(Vec3(0.f, 1.f, 0.f));
        OCTREE->InsertCollider(collider);
        shell->AddComponent(collider);
        CUR_SCENE->Add(shell);
    }
    // Shell
    {
        auto shell = make_shared<GameObject>();
        shell->SetObjectType(ObjectType::Shell);
        shell->GetOrAddTransform()->SetPosition(Vec3(62.f, -0.5f, 2.f));
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
        collider->SetBoundingBox(BoundingBox(Vec3(0.f), Vec3(0.5f, 0.5f, 1.f)));
        collider->SetOffset(Vec3(0.f, 1.f, 0.f));
        OCTREE->InsertCollider(collider);
        shell->AddComponent(collider);
        CUR_SCENE->Add(shell);
    }
	// Player
    {
        auto player = make_shared<GameObject>();

        player->SetObjectType(ObjectType::Player);
        player->GetOrAddTransform()->SetPosition(Vec3(10, 0, 10));
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
            playerModel->ReadAnimation(L"Player/Crab_Dance", AnimationState::Dance);
            playerModel->ReadAnimation(L"Player/Crab_Struggle", AnimationState::Struggle);

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
    }

    // Enemy
    {
        ENEMY->CreateFinalBoss({ 50.0f,0.f, 93.0f });
        //ENEMY->CreateFinalPhase({ 30.0f,0.f, 50.0f });
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

    //Bubble in Map
    {
        int bubbleCount = 30;
        float mapSize = 100.f;

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
            bubbleObj->GetParticle()->Add(Vec3(MathUtils::Random(0.f, mapSize), y, MathUtils::Random(0.f, mapSize)), Vec2(2.0f, 2.0f));

            CUR_SCENE->Add(bubbleObj);
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
