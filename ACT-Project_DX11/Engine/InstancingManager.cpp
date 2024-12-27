#include "pch.h"
#include "InstancingManager.h"
#include "InstancingBuffer.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "ModelRenderer.h"
#include "ModelAnimator.h"
#include "Transform.h"
#include "Camera.h"
#include "BaseCollider.h"
#include "Shader.h"
#include "AABBBoxCollider.h"
#include "SphereCollider.h"
#include "Particle.h"

void InstancingManager::Render(vector<shared_ptr<GameObject>>& gameObjects)
{
	ClearData();

	RenderMeshRenderer(gameObjects);
	RenderModelRenderer(gameObjects);
	RenderAnimRenderer(gameObjects);
}

void InstancingManager::RenderMeshRenderer(vector<shared_ptr<GameObject>>& gameObjects)
{
	map<InstanceID, vector<shared_ptr<GameObject>>> cache;

    vector<shared_ptr<GameObject>> alphaObjects;
	// 분류 단계
	for (shared_ptr<GameObject>& gameObject : gameObjects)
	{
		if (gameObject->GetMeshRenderer() == nullptr)
			continue;

        if (gameObject->GetUI() != nullptr)
        {
            if (gameObject->GetUI()->GetActive() != true)
            {
                continue;
            }
            
        }
        if (gameObject->GetMeshRenderer()->GetAlphaBlend())
        {
            alphaObjects.push_back(gameObject);
            continue;
        }
        
        if (gameObject->GetMeshRenderer()->GetParticleRender())
        {
            alphaObjects.push_back(gameObject);
            continue;
        }
		const InstanceID instanceId = gameObject->GetMeshRenderer()->GetInstanceID();
		cache[instanceId].push_back(gameObject);
	}

	for (auto& pair : cache)
	{
		const vector<shared_ptr<GameObject>>& vec = pair.second;

		if (vec.size() == 1)
		{
			vec[0]->GetMeshRenderer()->RenderSingle();
		}
		else
		{
			const InstanceID instanceId = pair.first;

			for (int32 i = 0; i < vec.size(); i++)
			{
				const shared_ptr<GameObject>& gameObject = vec[i];
				InstancingData data;
				data.world = gameObject->GetTransform()->GetWorldMatrix();

				AddData(instanceId, data);
			}

			shared_ptr<InstancingBuffer>& buffer = _buffers[instanceId];
			vec[0]->GetMeshRenderer()->RenderInstancing(buffer);
		}
	}
    std::sort(alphaObjects.begin(), alphaObjects.end(), [](const auto& a, const auto& b) {
        return a->GetTransform()->GetPosition().z > b->GetTransform()->GetPosition().z; // z값 기준 내림차순 정렬
        });

    for (const auto& obj : alphaObjects)
    {
        obj->GetMeshRenderer()->RenderSingle(); // 투명 UI 객체는 개별적으로 렌더링
    }
}


void InstancingManager::RenderModelRenderer(vector<shared_ptr<GameObject>>& gameObjects)
{
	map<InstanceID, vector<shared_ptr<GameObject>>> cache;

	for (shared_ptr<GameObject>& gameObject : gameObjects)
	{
		if (gameObject->GetModelRenderer() == nullptr)
			continue;

		const InstanceID instanceId = gameObject->GetModelRenderer()->GetInstanceID();
		cache[instanceId].push_back(gameObject);
	}

	for (auto& pair : cache)
	{
		const vector<shared_ptr<GameObject>>& vec = pair.second;

		if (vec.size() == 1)
		{
			vec[0]->GetModelRenderer()->RenderSingle();
		}
		else
		{
			const InstanceID instanceId = pair.first;

			for (int32 i = 0; i < vec.size(); i++)
			{
				const shared_ptr<GameObject>& gameObject = vec[i];
				InstancingData data;
				data.world = gameObject->GetTransform()->GetWorldMatrix();

				AddData(instanceId, data);
			}

			shared_ptr<InstancingBuffer>& buffer = _buffers[instanceId];
			vec[0]->GetModelRenderer()->RenderInstancing(buffer);
		}
	}
}

void InstancingManager::RenderAnimRenderer(vector<shared_ptr<GameObject>>& gameObjects)
{
	map<InstanceID, vector<shared_ptr<GameObject>>> cache;

	for (shared_ptr<GameObject>& gameObject : gameObjects)
	{
		if (gameObject->GetModelAnimator() == nullptr)
			continue;

		const InstanceID instanceId = gameObject->GetModelAnimator()->GetInstanceID();
		cache[instanceId].push_back(gameObject);
	}

	for (auto& pair : cache)
	{
		const vector<shared_ptr<GameObject>>& vec = pair.second;

		if (vec.size() == 1)
		{
			vec[0]->GetModelAnimator()->RenderSingle();
		}
		else
		{
			shared_ptr<InstancedTweenDesc> tweenDesc = make_shared<InstancedTweenDesc>();

			const InstanceID instanceId = pair.first;

			for (int32 i = 0; i < vec.size(); i++)
			{
				const shared_ptr<GameObject>& gameObject = vec[i];
				InstancingData data;
				data.world = gameObject->GetTransform()->GetWorldMatrix();

				AddData(instanceId, data);

				// INSTANCING
				gameObject->GetModelAnimator()->UpdateTweenData();
				tweenDesc->tweens[i] = gameObject->GetModelAnimator()->GetTweenDesc();
			}

			vec[0]->GetModelAnimator()->GetShader()->PushInstancedTweenData(*tweenDesc.get());

			shared_ptr<InstancingBuffer>& buffer = _buffers[instanceId];
			vec[0]->GetModelAnimator()->RenderInstancing(buffer);
		}
	}
}

void InstancingManager::RenderCollider(vector<shared_ptr<GameObject>>& gameObjects)
{
    if (!(DEBUG->IsDebugEnabled() || INPUT->GetButton(KEY_TYPE::CAPSLOCK)))
        return;

    // InstanceID를 기반으로 Collider 그룹화
    map<InstanceID, vector<shared_ptr<GameObject>>> colliderGroups;

    for (shared_ptr<GameObject>& gameObject : gameObjects)
    {
        shared_ptr<BaseCollider> collider = gameObject->GetCollider();
        if (!collider || !collider->IsActive())
            continue;

        // 거리 필터링: 카메라와 너무 먼 객체는 제외
        Vec3 colliderCenter = collider->GetColliderCenter();
        Vec3 cameraPosition = CUR_SCENE->GetMainCamera()->GetCamera()->GetCameraPosition();
        float distanceToCamera = (colliderCenter - cameraPosition).Length();
        if (distanceToCamera > MAX_RENDER_DISTANCE)
            continue;

        const InstanceID instanceId = collider->GetInstanceID();
        colliderGroups[instanceId].push_back(gameObject);
    }

    // 렌더링 단계
    for (auto& pair : colliderGroups)
    {
        const vector<shared_ptr<GameObject>>& objects = pair.second;

        const InstanceID instanceId = pair.first;

        for (int32 i = 0; i < objects.size(); i++)
        {
            const shared_ptr<GameObject>& gameObject = objects[i];
            InstancingData data;
            // 월드 행렬 가져오기
            Matrix worldMatrix = gameObject->GetTransform()->GetWorldMatrix();

            // 스케일, 회전, 위치 분리
            Vec3 scale, translation;
            Quaternion q_rotation;
            worldMatrix.Decompose(scale, q_rotation, translation);

            // 스케일을 1.0으로 변경
            Vec3 adjustedScale = Vec3(1.0f, 1.0f, 1.0f);

            Matrix adjustedWorldMatrix;
            Vec3 rotation = gameObject->GetTransform()->ToEulerAngles(q_rotation);
            translation += gameObject->GetCollider()->GetOffset();

            Matrix matScale = Matrix::CreateScale(adjustedScale);
            adjustedWorldMatrix *= matScale;
            Matrix matRotation = Matrix::CreateRotationX(rotation.x);
            matRotation *= Matrix::CreateRotationY(rotation.y);
            matRotation *= Matrix::CreateRotationZ(rotation.z);
            adjustedWorldMatrix *= matRotation;
            Matrix matTranslation = Matrix::CreateTranslation(translation);
            adjustedWorldMatrix *= matTranslation;

            data.world = adjustedWorldMatrix;

            AddData(instanceId, data);
        }
        shared_ptr<InstancingBuffer>& buffer = _buffers[instanceId];
        objects[0]->GetCollider()->RenderCollider(buffer);
        // 기본 토폴로지 복구
        DC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }
}

void InstancingManager::AddData(InstanceID instanceId, InstancingData& data)
{
	if (_buffers.find(instanceId) == _buffers.end())
		_buffers[instanceId] = make_shared<InstancingBuffer>();

	_buffers[instanceId]->AddData(data);
}

void InstancingManager::ClearData()
{
	for (auto& pair : _buffers)
	{
		shared_ptr<InstancingBuffer>& buffer = pair.second;
		buffer->ClearData();
	}
}
