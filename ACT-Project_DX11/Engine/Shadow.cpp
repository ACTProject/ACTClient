#include "pch.h"
#include "Shadow.h"
#include "ModelRenderer.h"
#include "MeshRenderer.h"
#include "Material.h"

void Shadow::Init()
{
    shadowShader = make_shared<Shader>(_shaderName);
    testShadowShader = make_shared<Shader>(L"ShadowDebug.fx");

    float fWidthLength = 512 * 512;
    float fHeightLength = 512 * 512;
    float fMaxViewDistance = sqrt(fWidthLength + fHeightLength);
    
    _lookVec = { 256,0, 256 };
    _lightPos = {256, 500,0 };
    _upVec = { 0,1,0 };
    // 쉐도우 행렬 초기화
    _shadowViewMat = ::XMMatrixLookAtLH(_lightPos, _lookVec, _upVec);

    float offset = 60;
    _shadowProjMat = ::XMMatrixOrthographicOffCenterLH(-fMaxViewDistance / 2+ offset, fMaxViewDistance / 2 - offset, -fMaxViewDistance / 2 + offset, fMaxViewDistance / 2 - offset, 0.1f, 100000.0f);


    _texture = Matrix(
          0.5f, 0.0f, 0.0f, 0.0f
        , 0.0f, -0.5f, 0.0f, 0.0f
        , 0.0f, 0.0f, 1.0f, 0.0f
        , 0.5f, 0.5f, 0.0f, 1.0f);

    _shadowDesc.Shadow = _shadowViewMat * _shadowProjMat * _texture;

    //DebugShadow();
}

void Shadow::updatelightPos()
{
    //게임에서 업데이트에 이거 하면 디버깅가능.
    ImGui::DragFloat("1", &_lightPos.x,5.0f);
    ImGui::DragFloat("2", &_lightPos.y,5.0f);
    ImGui::DragFloat("3", &_lightPos.z, 5.0f);
    ImGui::DragFloat("4", &_bias, 10.0f);
}

void Shadow::Update()
{
    _shadowViewMat = ::XMMatrixLookAtLH(_lightPos, _lookVec, _upVec);
    _shadowDesc.Shadow = _shadowViewMat * _shadowProjMat * _texture;
    RenderShadow();
}

void Shadow::DebugShadow()
{
    /*auto obj = make_shared<GameObject>();
    obj->GetOrAddTransform()->SetLocalPosition(Vec3(0.6, 0.6, 0.1f));
    obj->GetOrAddTransform()->SetLocalScale(Vec3(0.5, 0.5, 0));
    obj->AddComponent(make_shared<MeshRenderer>());

    obj->SetLayerIndex(Layer_UI);
    {
        {
            shared_ptr<Material> material = make_shared<Material>();
            material->SetShader(testShadowShader);
            auto texture = RESOURCES->Load<Texture>(L"RedBar", L"..\\Resources\\Textures\\UI\\Front_HPBar2.png");
            material->SetDiffuseMap(texture);
            MaterialDesc& desc = material->GetMaterialDesc();
            desc.ambient = Vec4(1.f);
            desc.diffuse = Vec4(1.f);
            desc.specular = Vec4(1.f);
            RESOURCES->Add(L"Shadow", material);
        }
        obj->GetMeshRenderer()->SetShader(testShadowShader);
        obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"RedBar"));
    }
    {
        auto mesh = make_shared<Mesh>();
        mesh->CreateQuad();
        obj->GetMeshRenderer()->GetMaterial()->SetShader(testShadowShader);
        obj->GetMeshRenderer()->SetMesh(mesh);
        obj->GetMeshRenderer()->SetTechnique(0);
        obj->GetMeshRenderer()->SetPass(0);
    }
    CUR_SCENE->Add(obj);*/
}

void Shadow::RenderShadow()
{
    unordered_set<shared_ptr<GameObject>> _gameObject = CUR_SCENE->GetObjects();
    for (auto& obj : _gameObject)
    {
        if (obj->GetLayerIndex() == LayerMask::Layer_UI)
            continue;
        if (obj->GetBillboard() != nullptr)
            continue;
        if (obj->GetObjectType() == ObjectType::Player)
            continue;
        if (obj->GetSkybox() != nullptr)
            continue;
        

        // 게임오브젝트를 그림자맵에 렌더링
        shared_ptr<Shader> shader;
        if (obj->GetModelRenderer() != nullptr)
        {
            shader = obj->GetModelRenderer()->GetShader();
            uint8 pass = obj->GetModelRenderer()->GetPass();
            uint8 tech = obj->GetModelRenderer()->GetTechnique();

            // 섀도우 쉐이더와 패스로 변경
            obj->GetModelRenderer()->SetShader(shadowShader);
            obj->GetModelRenderer()->SetTechnique(0);
            obj->GetModelRenderer()->SetPass(0);

            // 그림자 뎁스버퍼에 z값 기입
            obj->GetModelRenderer()->RenderShadowMap(_shadowViewMat, _shadowProjMat);


            // 다시 기존 쉐이더와 패스로 변경.
            obj->GetModelRenderer()->SetShader(shader);
            obj->GetModelRenderer()->SetPass(pass);
            obj->GetModelRenderer()->SetTechnique(tech);
        }

        if (obj->GetMeshRenderer() != nullptr && obj->GetTerrain() != nullptr)
        {
            shader = obj->GetMeshRenderer()->GetMaterial()->GetShader();
            uint8 pass = obj->GetMeshRenderer()->GetPass();
            uint8 tech = obj->GetMeshRenderer()->GetTech();

            obj->GetMeshRenderer()->GetMaterial()->SetShader(shadowShader);
            obj->GetMeshRenderer()->SetTechnique(0);
            obj->GetMeshRenderer()->SetPass(1);

            obj->GetMeshRenderer()->RenderShadowMap(_shadowViewMat, _shadowProjMat);
            obj->GetMeshRenderer()->GetMaterial()->SetShader(shader);
            obj->GetMeshRenderer()->SetPass(pass);
            obj->GetMeshRenderer()->SetTechnique(tech);
        }
    }
}


