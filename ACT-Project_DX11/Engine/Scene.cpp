#include "pch.h"
#include "Scene.h"
#include "GameObject.h"
#include "BaseCollider.h"
#include "Camera.h"
#include "Terrain.h"
#include "Button.h"
#include "Rigidbody.h"

void Scene::Awake()
{
	unordered_set<shared_ptr<GameObject>> objects = _objects;

	for (shared_ptr<GameObject> object : objects)
	{
		object->Awake();
	}
}

void Scene::Start()
{
	unordered_set<shared_ptr<GameObject>> objects = _objects;

	for (shared_ptr<GameObject> object : objects)
	{
        if (!object->IsActive())
            continue;

		object->Start();
	}
}

void Scene::Update()
{
	unordered_set<shared_ptr<GameObject>> objects = _objects;

	for (shared_ptr<GameObject> object : objects)
	{
        if (!object->IsActive())
            continue;

		object->Update();
	}

	UpdateUI();

	MAP->Update();

}

void Scene::FixedUpdate()
{
	unordered_set<shared_ptr<GameObject>> objects = _objects;

	for (shared_ptr<GameObject> object : objects)
	{
        if (!object->IsActive())
            continue;

		object->FixedUpdate();
	}
}

void Scene::LateUpdate()
{
	unordered_set<shared_ptr<GameObject>> objects = _objects;

	for (shared_ptr<GameObject> object : objects)
	{
        if (!object->IsActive())
            continue;

		object->LateUpdate();
	}

	CheckPhysicCollision();
}

void Scene::Render()
{
    shared_ptr<Camera> mainCamera = make_shared<Camera>();

	for (auto& camera : _cameras)
	{
        if (camera->GetCamera()->IsMainCamera() == true)
            mainCamera = camera->GetCamera();

		camera->GetCamera()->SortGameObject();
 		camera->GetCamera()->Render_Forward();
	}
    if (DEBUG->IsDebugEnabled() || INPUT->GetButton(KEY_TYPE::CAPSLOCK))
    {
        Camera::S_MatView = mainCamera->GetViewMatrix();
        Camera::S_MatProjection = mainCamera->GetProjectionMatrix();
        FRUSTUM->FinalUpdate();
        vector<std::shared_ptr<GameObject>> visibleObjects = FrustumCulling(mainCamera->GetVecForward());
        INSTANCING->RenderCollider(visibleObjects);
        
        if (INPUT->GetButton(KEY_TYPE::KEY_F2))
        {
            OCTREE->RenderOctree();
        }
    }
}

void Scene::Add(shared_ptr<GameObject> object)
{
    if (object == nullptr)
    {
        return;
    }

	_objects.insert(object);

	if (object->GetCamera() != nullptr)
	{
		_cameras.insert(object);
	}

	if (object->GetLight() != nullptr)
	{
		_lights.insert(object);
	}
}

void Scene::Remove(shared_ptr<GameObject> object)
{
	_objects.erase(object);

	_cameras.erase(object);

	_lights.erase(object);
}

void Scene::Clear()
{
    std::vector<std::shared_ptr<GameObject>> objectsToRemove(_objects.begin(), _objects.end());

    for (const auto& object : objectsToRemove)
    {
        if (object)
        {
            Remove(object);
        }
    }

    _objects.clear();
    _cameras.clear();
    _lights.clear();
}

std::shared_ptr<GameObject> Scene::GetMainCamera()
{
    for (auto& camera : _cameras)
    {
        if (camera->GetCamera()->IsMainCamera())
            return camera;
    }

    return nullptr;
}

std::shared_ptr<GameObject> Scene::GetUICamera()
{
	for (auto& camera : _cameras)
	{
		if (camera->GetCamera()->GetProjectionType() == ProjectionType::Orthographic)
			return camera;
	}

	return nullptr;
}

void Scene::UpdateUI()
{
	if (GetUICamera() == nullptr)
		return;


    UIMANAGER->Update();
	// Slider
	const auto gameObjects = GetObjects();

	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetUI() == nullptr)
			continue;
	}


	// PickUI
	POINT screenPt = INPUT->GetMousePos();

	shared_ptr<Camera> camera = GetUICamera()->GetCamera();

	for (auto& gameObject : gameObjects)
	{
        if (gameObject->GetButton() == nullptr)
            continue;

        gameObject->GetButton()->CheckHover(screenPt);
		
        if (INPUT->GetButtonDown(KEY_TYPE::LBUTTON))
        {
            if (gameObject->GetButton()->Picked(screenPt))
                gameObject->GetButton()->InvokeOnClicked();
        }

        gameObject->GetButton()->CheckKeyInput();
	}
}

std::shared_ptr<class GameObject> Scene::Pick(int32 screenX, int32 screenY)
{
	shared_ptr<Camera> camera = GetMainCamera()->GetCamera();

	float width = GRAPHICS->GetViewport().GetWidth();
	float height = GRAPHICS->GetViewport().GetHeight();

	Matrix projectionMatrix = camera->GetProjectionMatrix();

	float viewX = (+2.0f * screenX / width - 1.0f) / projectionMatrix(0, 0);
	float viewY = (-2.0f * screenY / height + 1.0f) / projectionMatrix(1, 1);

	Matrix viewMatrix = camera->GetViewMatrix();
	Matrix viewMatrixInv = viewMatrix.Invert();

	const auto& gameObjects = GetObjects();

	float minDistance = FLT_MAX;
	shared_ptr<GameObject> picked;

	for (auto& gameObject : gameObjects)
	{
		if (camera->IsCulled(gameObject->GetLayerIndex()))
			continue;
		if (gameObject->GetCollider() == nullptr)
			continue;

		// ViewSpace에서의 Ray 정의
		Vec4 rayOrigin = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
		Vec4 rayDir = Vec4(viewX, viewY, 1.0f, 0.0f);

		// WorldSpace에서의 Ray 정의
		Vec3 worldRayOrigin = XMVector3TransformCoord(rayOrigin, viewMatrixInv);
		Vec3 worldRayDir = XMVector3TransformNormal(rayDir, viewMatrixInv);
		worldRayDir.Normalize();

		// WorldSpace에서 연산
		Ray ray = Ray(worldRayOrigin, worldRayDir);

		float distance = 0.f;
		if (gameObject->GetCollider()->Intersects(ray, OUT distance) == false)
			continue;

		MAP->ChekMapObjectSelect(gameObject);


		if (distance < minDistance)
		{
			minDistance = distance;
			picked = gameObject;
		}
	}

	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetTerrain() == nullptr)
			continue;

		if (MAP->ChekMapDescSelect())
			continue;

		Vec3 pickPos;
		float distance = 0.f;
		if (gameObject->GetTerrain()->Pick(screenX, screenY, OUT pickPos, OUT distance) == false)
			continue;

		shared_ptr<GameObject> obj = MAP->Create(pickPos);
		Add(obj);

		if (distance < minDistance)
		{
			minDistance = distance;
			picked = gameObject;
		}
	}

	return picked;
}

Vec3 Scene::Picking(int32 screenX, int32 screenY)
{
	Vec3 pickPos;
	const auto& gameObjects = GetObjects();
	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetTerrain() == nullptr)
			continue;


		float distance = 0.f;
		if (gameObject->GetTerrain()->Pick(screenX, screenY, OUT pickPos, OUT distance) == false)
			continue;
	}
	return pickPos;
}

void Scene::CheckPhysicCollision()
{
	COLLISION->Update();
}

vector<shared_ptr<GameObject>> Scene::FrustumCulling(const vector<shared_ptr<GameObject>>& allObjects)
{
    vector<shared_ptr<GameObject>> visibleObjects;

    for (const auto& object : allObjects)
    {
        if (object->GetBullet())
        {
            visibleObjects.push_back(object);
            continue;
        }

        // Terrain은 무조건 보이게
        if (object->GetTerrain())
        {
            visibleObjects.push_back(object);
            continue;
        }

        if (object->GetSkybox())
        {
            visibleObjects.push_back(object);
            continue;
        }

        if (object->GetParticle())
        {
            visibleObjects.push_back(object);
            continue;
        }
        if (object->GetObjectType() == ObjectType::MapMesh)
        {
            visibleObjects.push_back(object);
            continue;
        }

        if (object->GetObjectType() == ObjectType::Map)
        {
            visibleObjects.push_back(object);
            continue;
        }

        // 객체가 비활성화되어 있으면 무시
        if (!object->IsActive())
            continue;

        // Collider 또는 Bound 정보가 없으면 무시
        auto collider = object->GetCollider();

        if (!collider || !collider->IsActive())
            continue;

        // Collider 타입에 따라 프러스텀 안에 있는지 확인
        switch (collider->GetColliderType())
        {
            case ColliderType::Sphere:
            {
                shared_ptr<SphereCollider> sphere = dynamic_pointer_cast<SphereCollider>(collider);
                // Sphere의 중심과 반경 계산
                Vec3 sphereCenter = sphere->GetColliderCenter();
                float sphereRadius = sphere->GetBoundingSphere().Radius;

                if (FRUSTUM->ContainsSphere(sphereCenter, sphereRadius))
                {
                    visibleObjects.push_back(object);
                }
                break;
            }
            case ColliderType::AABB:
            {
                // AABB 경계 정보 가져오기
                shared_ptr<AABBBoxCollider> aabb = dynamic_pointer_cast<AABBBoxCollider>(collider);
                BoundingBox box = aabb->GetBoundingBox();

                if (FRUSTUM->ContainsAABB(box))
                {
                    visibleObjects.push_back(object);
                }
                break;
            }
            case ColliderType::OBB:
            {
                // TODO
                break;
            }
            default:
                // 다른 Collider 타입은 무시
                break;
        }
    }

    return visibleObjects;
}