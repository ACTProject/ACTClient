#include "pch.h"
#include "RawBuffer.h"
#include "TextureBuffer.h"
#include "Material.h"
#include "Client.h"
#include "GeometryHelper.h"
#include "Camera.h"
#include "GameObject.h"
#include "CameraScript.h"
#include "PlayerScript.h"
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
#include "RangoonScript.h"
#include "Rigidbody.h"
#include "Slider.h"
#include "Skybox.h"
#include "HitBox.h"
#include "Particle.h"

void Client::Init()
{
	shared_ptr<Shader> renderShader = make_shared<Shader>(L"23. RenderDemo.fx");
	shared_ptr<Shader> renderBoxShader = make_shared<Shader>(L"23. RenderDemoBox.fx");
	shared_ptr<Shader> renderUIShader = make_shared<Shader>(L"23. RenderDemoUI.fx");
    shared_ptr<Shader> hpShader = make_shared<Shader>(L"HpUI.fx");
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
            material->SetShader(hpShader);
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
			auto obj = make_shared<GameObject>(GameObjectType::UI);
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
            auto obj = make_shared<GameObject>(GameObjectType::UI);
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
            auto obj = make_shared<GameObject>(GameObjectType::UI);
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
            auto obj = make_shared<GameObject>(GameObjectType::UI);
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
            auto obj = make_shared<GameObject>(GameObjectType::UI);
            obj->AddComponent(make_shared<Slider>());
            obj->GetUI()->Create(Vec3(healPosition.x - 27.f, healPosition.y - 1.f, 0.1f), Vec2(65, 10), RESOURCES->Get<Material>(L"RedBar"));
            obj->GetUI()->SetUIID("HP");
            //obj->GetUI()->SetOwner();
            CUR_SCENE->Add(obj);
        }

        // RedBar ARmor Mesh
        {
            // 슬라이더 컴포넌트 추가.
            auto obj = make_shared<GameObject>(GameObjectType::UI);
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
                obj->GetButton()->AddOnKeyPressEvent(KEY_TYPE::ESC, [obj]() { obj->SetVisible(); });

                obj->SetVisible();
                optionGroup.push_back(obj);
                CUR_SCENE->Add(obj);
            }

           
            // Mesh
            {
                auto obj = make_shared<GameObject>();
                obj->AddComponent(make_shared<Button>());

                obj->GetButton()->Create(Vec3(532, 359, 0.3f), Vec2(175, 38), nullptr);
                obj->GetMeshRenderer()->SetAlphaBlend(true);
                obj->GetButton()->AddOnHoverEvent([obj]() { obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"GameEnd")); });
                obj->GetButton()->AddOnHoverEndEvent([obj]() { obj->GetMeshRenderer()->SetMaterial(nullptr); });
                obj->GetButton()->AddOnKeyPressEvent(KEY_TYPE::ESC, [obj]() { obj->SetVisible(); });
                obj->GetButton()->AddOnClickedEvent([obj]() { if (obj->GetVisible())GAME->GameEnd(); });

                obj->SetVisible();
                optionGroup.push_back(obj);
                CUR_SCENE->Add(obj);
            }

            // Mesh
            {
                auto obj = make_shared<GameObject>();
                obj->AddComponent(make_shared<Button>());

                obj->GetButton()->Create(Vec3(532, 308, 0.3f), Vec2(175, 38), nullptr);
                obj->GetMeshRenderer()->SetAlphaBlend(true);
                obj->GetButton()->AddOnHoverEvent([obj]() { obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"Continue")); });
                obj->GetButton()->AddOnHoverEndEvent([obj]() { obj->GetMeshRenderer()->SetMaterial(nullptr); });
                obj->GetButton()->AddOnKeyPressEvent(KEY_TYPE::ESC, [obj]() { obj->SetVisible(); });
                //obj->GetButton()->AddOnClickedEvent([obj]() { obj->SetVisible(); });
                optionGroup.push_back(obj);
                obj->GetButton()->AddOnClickedEvent([optionGroup]() {
                    if (optionGroup[0]->GetVisible())
                    {
                        for (auto& uiObject : optionGroup) {
                            uiObject->SetVisible();
                        }
                    }
                    });

                obj->SetVisible();
                
                CUR_SCENE->Add(obj);
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
		lightDesc.direction = Vec3(1.f, 0.f, 1.f);
		light->GetLight()->SetLightDesc(lightDesc);
		CUR_SCENE->Add(light);
	}

	// Player
	auto player = make_shared<GameObject>(GameObjectType::PLAYER);


	// Player
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

		//playerModel->ReadAnimation(L"Player/Crab_Death");
		//playerModel->ReadAnimation(L"Player/Crab_GetUp");

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
	collider->SetRadius(5.0f);
	collider->SetOffset(Vec3(0.f, 1.f, 0.f));
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
	hitbox->Craete(player, Vec3(1.4f));
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
	shared_ptr<PlayerScript> playerScript = make_shared<PlayerScript>();

	playerScript->SetPlayer(playerModel);
	playerScript->SetModelAnimator(ma1);
	playerScript->SetHitBox(hitboxGO);
    playerScript->SetDust(dustMaterial);

	player->AddComponent(playerScript);

	CUR_SCENE->Add(player);
	CUR_SCENE->SetPlayer(player);


	// Enemy
	auto enemy = make_shared<GameObject>(GameObjectType::ENEMY);
	{
		enemy->GetOrAddTransform()->SetPosition(Vec3(20, 0, 20));
		enemy->GetOrAddTransform()->SetLocalRotation(Vec3(0, 0, 0)); // XMConvertToRadians()
		enemy->GetOrAddTransform()->SetScale(Vec3(0.0001f));

		shared_ptr<Model> enemyModel = make_shared<Model>();
		// Model
		{
			enemyModel->ReadModel(L"Enemy/Rangoon");
			enemyModel->ReadMaterial(L"Enemy/Rangoon");

			enemyModel->ReadAnimation(L"Enemy/Rangoon_idle", AnimationState::Idle);
			enemyModel->ReadAnimation(L"Enemy/Rangoon_run", AnimationState::Run);
			enemyModel->ReadAnimation(L"Enemy/Rangoon_aggro", AnimationState::Aggro);
			enemyModel->ReadAnimation(L"Enemy/Rangoon_atkBigSnippy", AnimationState::Attack1);
			enemyModel->ReadAnimation(L"Enemy/Rangoon_atkSmallSnippy", AnimationState::Attack2);
			enemyModel->ReadAnimation(L"Enemy/Rangoon_atkSmash", AnimationState::Attack3);
			enemyModel->ReadAnimation(L"Enemy/Rangoon_die", AnimationState::Die);
			enemyModel->ReadAnimation(L"Enemy/Rangoon_hit", AnimationState::Hit);
			enemyModel->ReadAnimation(L"Enemy/Rangoon_roar", AnimationState::Roar);
		}
		shared_ptr<ModelAnimator> ma2 = make_shared<ModelAnimator>(renderShader);
		enemy->AddComponent(ma2);
		{
			enemy->GetModelAnimator()->SetModel(enemyModel);
			enemy->GetModelAnimator()->SetPass(2);
		}
		shared_ptr<RangoonScript> rangoon = make_shared<RangoonScript>();

		rangoon->SetEnemy(enemyModel);
		rangoon->SetModelAnimator(ma2);

		enemy->AddComponent(rangoon);

		// Collider
		auto collider = make_shared<AABBBoxCollider>();
		collider->GetBoundingBox().Extents = Vec3(1.2f);
		collider->SetOffset(Vec3(0.f, 1.f, 0.f));
		enemy->AddComponent(collider);

		// Rigidbody
		shared_ptr<Rigidbody> rigidBody = make_shared<Rigidbody>();
		rigidBody->SetUseGravity(true);
		rigidBody->SetMass(2.0f);
		enemy->AddComponent(rigidBody);

        auto ui = make_shared<GameObject>(GameObjectType::UI);
        ui->AddComponent(make_shared<Slider>());
       
        ui->GetUI()->Create(Vec3(0, 0, 0.0f), Vec2(60, 8), RESOURCES->Get<Material>(L"RedBar"));
        ui->GetUI()->SetOwner(enemy);
        ui->GetUI()->SetUIID("Enemy");
        CUR_SCENE->Add(ui);

		COLLISION->AddRigidbody(rigidBody);
		COLLISION->AddCollider(collider);

		CUR_SCENE->Add(enemy);
	}

	//auto enemy2 = make_shared<GameObject>();
	//{
	//	enemy2->GetOrAddTransform()->SetPosition(Vec3(5, 0, 10));
	//	enemy2->GetOrAddTransform()->SetLocalRotation(Vec3(0, 0, 0)); // XMConvertToRadians()
	//	enemy2->GetOrAddTransform()->SetScale(Vec3(0.0001f));

	//	shared_ptr<Model> enemyModel = make_shared<Model>();
	//	// Model
	//	{
	//		enemyModel->ReadModel(L"Enemy/pistol");
	//		enemyModel->ReadMaterial(L"Enemy/pistol");

	//		enemyModel->ReadAnimation(L"Enemy/pistol_Idle", AnimationState::Idle);
	//		//enemyModel->ReadAnimation(L"Enemy/pistol_shoot", AnimationState::Hit);
	//		//enemyModel->ReadAnimation(L"Enemy/pistol_Idle", AnimationState::Atk);*/

	//	}
	//	shared_ptr<ModelAnimator> ma2 = make_shared<ModelAnimator>(renderShader);
	//	enemy2->AddComponent(ma2);
	//	{
	//		enemy2->GetModelAnimator()->SetModel(enemyModel);
	//		enemy2->GetModelAnimator()->SetPass(2);
	//	}
	//	CUR_SCENE->Add(enemy2);
	//}

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


		auto obj = make_shared<GameObject>(GameObjectType::TERRAIN);
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
