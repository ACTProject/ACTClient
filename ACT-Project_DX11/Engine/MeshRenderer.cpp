#include "pch.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Game.h"
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "Light.h"

MeshRenderer::MeshRenderer() : Super(ComponentType::MeshRenderer)
{

}

MeshRenderer::~MeshRenderer()
{

}

void MeshRenderer::RenderInstancing(shared_ptr<class InstancingBuffer>& buffer)
{
	if (_mesh == nullptr || _material == nullptr)
		return;

	_shader = _material->GetShader();
	if (_shader == nullptr)
		return;

	// GlobalData
	if (GetGameObject()->GetLayerIndex() == LayerMask::Layer_UI)
		_shader->PushGlobalData(Camera::S_UIMatView, Camera::S_UIMatProjection);
	else
		_shader->PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);

	// Light
	auto lightObj = SCENE->GetCurrentScene()->GetLight();
	if (lightObj)
		_shader->PushLightData(lightObj->GetLight()->GetLightDesc());

    _shader->PushShadowData(SHADOW->GetShadowDesc());
    _shader->GetSRV("ShadowDepthTexture")->SetResource(GRAPHICS->GetShadowSRV().Get());
    //_shader->GetSRV("ShadowColorTexture")->SetResource(GRAPHICS->GetShadowColorSRV().Get());

	// Light
	_material->Update();

	// IA
	_mesh->GetVertexBuffer()->PushData(); 
	_mesh->GetIndexBuffer()->PushData();



	buffer->PushData();

	if (Camera::S_IsWireFrame)
		_technique = 2;
	else
		_technique = 0;


	_shader->DrawIndexedInstanced(_technique, _pass, _mesh->GetIndexBuffer()->GetCount(), buffer->GetCount());
}

void MeshRenderer::RenderSingle()
{
	if (_mesh == nullptr || _material == nullptr)
		return;

	_shader = _material->GetShader();
	if (_shader == nullptr)
		return;

	// GlobalData
	if (GetGameObject()->GetLayerIndex() == LayerMask::Layer_UI)
		_shader->PushGlobalData(Camera::S_UIMatView, Camera::S_UIMatProjection);
	else
		_shader->PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);

	// Light
	auto lightObj = SCENE->GetCurrentScene()->GetLight();
	if (lightObj)
		_shader->PushLightData(lightObj->GetLight()->GetLightDesc());

    _shader->PushShadowData(SHADOW->GetShadowDesc());

	// Light
	_material->Update();

	// Transform
	auto world = GetTransform()->GetWorldMatrix();
	_shader->PushTransformData(TransformDesc{ world });

	// IA
	_mesh->GetVertexBuffer()->PushData();
	_mesh->GetIndexBuffer()->PushData();

    if (GetGameObject()->GetBillboard() != nullptr)
    {
        auto wave = MAP->UpdateWaveMat();
        _shader->PushWaveData(WaveDesc{wave});
    }

	if (Camera::S_IsWireFrame)
		_technique = 3;
	else
		_technique = 1;

	if (_isAlphaBlend)
		_technique = 4; 
	_shader->DrawIndexed(_technique, _pass, _mesh->GetIndexBuffer()->GetCount(), 0, 0);
}

void MeshRenderer::RenderShadowMap(Matrix view, Matrix proj)
{
    if (_mesh == nullptr || _material == nullptr)
        return;

    _shader = _material->GetShader();
    if (_shader == nullptr)
        return;

    // GlobalData
    _shader->PushGlobalData(view, proj);

    // Light
    auto lightObj = SCENE->GetCurrentScene()->GetLight();
    if (lightObj)
        _shader->PushLightData(lightObj->GetLight()->GetLightDesc());

    _shader->PushShadowData(SHADOW->GetShadowDesc());

    // Light
    _material->Update();

    // Transform
    auto world = GetTransform()->GetWorldMatrix();
    _shader->PushTransformData(TransformDesc{ world });

    // IA
    _mesh->GetVertexBuffer()->PushData();
    _mesh->GetIndexBuffer()->PushData();

    _shader->DrawIndexed(_technique, _pass, _mesh->GetIndexBuffer()->GetCount(), 0, 0);
}

InstanceID MeshRenderer::GetInstanceID()
{
	return make_pair((uint64)_mesh.get(), (uint64)_material.get());
}
