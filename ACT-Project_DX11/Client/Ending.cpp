#include "pch.h"
#include "Ending.h"
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

void Ending::Init()
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
        camera->GetCamera()->SetIsTitle(true);
        CUR_SCENE->Add(camera);
    }    

}

void Ending::Update()
{
}

void Ending::Render()
{
}
