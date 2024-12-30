#include "pch.h"
#include "MapManager.h"
#include "MeshRenderer.h"
#include "ModelRenderer.h"
#include "AABBBoxCollider.h"
#include "Terrain.h"
#include "AABBBoxCollider.h"
#include "CollisionManager.h"
#include "Rigidbody.h"
#include "Material.h"
#include "Billboard.h"
#include "DynamicObj.h"
#include "Potal.h"
#include "JumpObj.h"
#include "SaveObj.h"
#include "HealObj.h"


void MapManager::Init()
{
    //clear
    //ClearMap();

    ////MapObj
    shared_ptr<MapObjDesc> src;
    {
        src = make_shared<MapObjDesc>(L"MapObject/recyclingBox01", L"23. RenderDemo.fx");
        MAP->AddMapObj(src);

        src = make_shared<MapObjDesc>(L"MapObject/PencilHedgehog", L"23. RenderDemo.fx");
        MAP->AddMapObj(src);

        src = make_shared<MapObjDesc>(L"MapObject/StarFish", L"23. RenderDemo.fx");
        MAP->AddMapObj(src);

        src = make_shared<MapObjDesc>(L"MapObject/pigeon", L"23. RenderDemo.fx");
        MAP->AddMapObj(src);

        src = make_shared<MapObjDesc>(L"MapObject/flagpole", L"23. RenderDemo.fx");
        MAP->AddMapObj(src);

        src = make_shared<MapObjDesc>(L"MapObject/WallLeft", L"23. RenderDemo.fx");
        MAP->AddMapObj(src);

        src = make_shared<MapObjDesc>(L"MapObject/Bottle", L"23. RenderDemo.fx", false, true, DynamicType::Heal, true);
        MAP->AddMapObj(src);

        src = make_shared<MapObjDesc>(L"MapObject/CardHouseEntity", L"23. RenderDemo.fx");
        MAP->AddMapObj(src);

        src = make_shared<MapObjDesc>(L"MapObject/Cylinder01", L"23. RenderDemo.fx");
        MAP->AddMapObj(src);

        src = make_shared<MapObjDesc>(L"MapObject/Cylinder07", L"23. RenderDemo.fx");
        MAP->AddMapObj(src);

        src = make_shared<MapObjDesc>(L"MapObject/KelpGround", L"23. RenderDemo.fx");
        MAP->AddMapObj(src);

        src = make_shared<MapObjDesc>(L"MapObject/road", L"23. RenderDemo.fx");
        MAP->AddMapObj(src);

        src = make_shared<MapObjDesc>(L"MapObject/rock", L"23. RenderDemo.fx");
        MAP->AddMapObj(src);

        src = make_shared<MapObjDesc>(L"MapObject/Umbrella", L"23. RenderDemo.fx");
        MAP->AddMapObj(src);

        src = make_shared<MapObjDesc>(L"MapObject/wall01", L"23. RenderDemo.fx");
        MAP->AddMapObj(src);

        src = make_shared<MapObjDesc>(L"MapObject/wall02", L"23. RenderDemo.fx");
        MAP->AddMapObj(src);

        src = make_shared<MapObjDesc>(L"MapObject/moonjelly", L"23. RenderDemo.fx", false, true, DynamicType::Jump, true);
        MAP->AddMapObj(src);

        src = make_shared<MapObjDesc>(L"MapObject/rock2", L"23. RenderDemo.fx");
        MAP->AddMapObj(src);
        //
        src = make_shared<MapObjDesc>(L"MapObject/Urock01", L"23. RenderDemo.fx");
        MAP->AddMapObj(src);
        src = make_shared<MapObjDesc>(L"MapObject/Urock02", L"23. RenderDemo.fx");
        MAP->AddMapObj(src);
        src = make_shared<MapObjDesc>(L"MapObject/Urock03", L"23. RenderDemo.fx");
        MAP->AddMapObj(src);
        src = make_shared<MapObjDesc>(L"MapObject/Urock04", L"23. RenderDemo.fx");
        MAP->AddMapObj(src);
        src = make_shared<MapObjDesc>(L"MapObject/Urock05", L"23. RenderDemo.fx");
        MAP->AddMapObj(src);
        src = make_shared<MapObjDesc>(L"MapObject/Urock06", L"23. RenderDemo.fx");
        MAP->AddMapObj(src);
        src = make_shared<MapObjDesc>(L"MapObject/Udrock01", L"23. RenderDemo.fx");
        MAP->AddMapObj(src);
        src = make_shared<MapObjDesc>(L"MapObject/Udrock02", L"23. RenderDemo.fx");
        MAP->AddMapObj(src);
        src = make_shared<MapObjDesc>(L"MapObject/Udrock04", L"23. RenderDemo.fx");
        MAP->AddMapObj(src);

        src = make_shared<MapObjDesc>(L"MapObject/Chips", L"23. RenderDemo.fx");
        MAP->AddMapObj(src);
        src = make_shared<MapObjDesc>(L"MapObject/FishCar", L"23. RenderDemo.fx");
        MAP->AddMapObj(src);
        src = make_shared<MapObjDesc>(L"MapObject/Milk Carton", L"23. RenderDemo.fx");
        MAP->AddMapObj(src);
        src = make_shared<MapObjDesc>(L"MapObject/Prop_ChineseTakeout", L"23. RenderDemo.fx");
        MAP->AddMapObj(src);
        src = make_shared<MapObjDesc>(L"MapObject/80s_radio", L"23. RenderDemo.fx");
        MAP->AddMapObj(src);
        src = make_shared<MapObjDesc>(L"MapObject/BottomFeeders_Barrels", L"23. RenderDemo.fx");
        MAP->AddMapObj(src);
        src = make_shared<MapObjDesc>(L"MapObject/can_crushed_03", L"23. RenderDemo.fx");
        MAP->AddMapObj(src);
        src = make_shared<MapObjDesc>(L"MapObject/CastleKit_Tower_01", L"23. RenderDemo.fx");
        MAP->AddMapObj(src); 

        src = make_shared<MapObjDesc>(L"MapObject/SeaObj01", L"normalRender.fx");
        MAP->AddMapObj(src);                                   
        src = make_shared<MapObjDesc>(L"MapObject/SeaObj02", L"normalRender.fx");
        MAP->AddMapObj(src);                                   
        src = make_shared<MapObjDesc>(L"MapObject/SeaObj03", L"normalRender.fx");
        MAP->AddMapObj(src);                                  
        src = make_shared<MapObjDesc>(L"MapObject/SeaObj04", L"normalRender.fx");
        MAP->AddMapObj(src);                                   
        src = make_shared<MapObjDesc>(L"MapObject/SeaObj05", L"normalRender.fx");
        MAP->AddMapObj(src);                                   
        src = make_shared<MapObjDesc>(L"MapObject/SeaObj06", L"normalRender.fx");
        MAP->AddMapObj(src);                                  
        src = make_shared<MapObjDesc>(L"MapObject/SeaObj07", L"normalRender.fx");
        MAP->AddMapObj(src);

        src = make_shared<MapObjDesc>(L"MapObject/Medieval_Door", L"23. RenderDemo.fx" , false, true, DynamicType::Potal);
        MAP->AddMapObj(src);  
        src = make_shared<MapObjDesc>(L"MapObject/Dynamic/MagicGate", L"23. RenderDemo.fx", true, true, DynamicType::Save,true);
        MAP->AddMapObj(src);

        // 바닥텍스처
        src = make_shared<MapObjDesc>(L"tailtexture01.png", L"23. RenderDemo.fx", false, false, DynamicType::None,false, true);
        MAP->AddMapObj(src);

        // 빌보드메시
        src = make_shared<MapObjDesc>(L"grass.png", L"28. BillBoardDemo.fx", false,false, DynamicType::None, false, false, true);
        MAP->AddMapObj(src);

        // ImGui용 함수.
        MAP->InitMapText();
    }
}

// 맵 오브젝트 피킹했는지 체크
bool MapManager::ChekMapObjectSelect(shared_ptr<GameObject> obj)
{
    for (auto& go : _mapObjList)
    {
        if (go == obj)
        {
            _mapSelectObj = go;
            return true;
        }
    }
    return false;
}

void MapManager::Update()
{
    if (DEBUG->IsDebugEnabled())
    {

        // 오브젝트 삭제 함수.
        if (_mapSelectObj != nullptr && InputManager::GetInstance()->GetButtonDown(KEY_TYPE::DELETEKEY))
        {
            RemoveMapObj(_mapSelectObj);
        }

        ImGuiSelectMapObject();


        ImGui::RadioButton("Position", &_transformSelect, 0); ImGui::SameLine();
        ImGui::RadioButton("Rotation", &_transformSelect, 1); ImGui::SameLine();
        ImGui::RadioButton("Scale", &_transformSelect, 2); ImGui::SameLine();
        ImGui::RadioButton("Collider", &_transformSelect, 3);

        if (_mapSelectDesc != nullptr)
        {
            UpdateMapDescTransform();
            //PreViewMapObject();
        }
        if (_mapSelectObj != nullptr)
        {
            UpdateMapObjTransform();
        }



        // 실시간 클릭 시 해당 위치에 오브젝트 배치 하는 코드.
        if (INPUT->GetButtonDown(KEY_TYPE::RBUTTON))
        {
            POINT screenPt = INPUT->GetMousePos();
            SCENE->GetCurrentScene()->Pick(screenPt.x, screenPt.y);
        }


        // 선택한 오브젝트 트랜스폼 변경 함수.
        if (_mapSelectObj != nullptr && InputManager::GetInstance()->GetButton(KEY_TYPE::RBUTTON) && _transformSelect == 0)
        {
            ChangeMapObjPosition();
        }
        else if (_mapSelectObj != nullptr && InputManager::GetInstance()->GetButton(KEY_TYPE::RBUTTON) && _transformSelect == 1)
        {
            ChangeMapObjRotation();
        }
        else if (_mapSelectObj != nullptr && InputManager::GetInstance()->GetButton(KEY_TYPE::RBUTTON) && _transformSelect == 2)
        {
            ChangeMapObjScale();
        }


    }
}

shared_ptr<GameObject> MapManager::Create(Vec3& pos)
{
    if (_mapSelectDesc->isBillBoard == true)
    {
        _billBoardCount++;
        AddBillBoard(pos);
        return nullptr;
    }
    shared_ptr<GameObject> obj = make_shared<GameObject>();
    {
        if (_mapSelectDesc->isMesh == true)
        {
            pos.y = 0;
        }
        obj->GetOrAddTransform()->SetPosition(pos);
        obj->GetOrAddTransform()->SetLocalRotation(_mapSelectDesc->rotation);
        obj->GetOrAddTransform()->SetScale(_mapSelectDesc->scale);

        auto it = _mapInfoList.find(_mapSelectDesc->filename);
        if (it != _mapInfoList.end())
        {
            if (_mapSelectDesc->isAnim == true)
            {
                shared_ptr<ModelAnimator> ma2 = make_shared<ModelAnimator>(it->second->_shader);
                obj->AddComponent(ma2);
                obj->GetModelAnimator()->SetModel(it->second->_model);
                obj->GetModelAnimator()->SetPass(2);
            }
            else if (_mapSelectDesc->isMesh == true)
            {
                auto meshrender = make_shared<MeshRenderer>();
                obj->AddComponent(meshrender);


                //auto mesh = RESOURCES->Get<Mesh>(L"Quads");
                auto mesh = make_shared<Mesh>();
                mesh->CreateGrid(1, 1);
                CreateQuadTerrain(mesh, obj, pos);
                obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(_mapSelectDesc->filename));
                obj->GetMeshRenderer()->SetMesh(mesh);
                obj->GetMeshRenderer()->SetPass(3);
                obj->GetMeshRenderer()->SetAlphaBlend(false);
                obj->SetObjectType(ObjectType::MapMesh);
            } 
            else
            {
                auto modelrender = make_shared<ModelRenderer>(it->second->_shader);
                obj->AddComponent(modelrender);
                obj->GetModelRenderer()->SetModel(it->second->_model);
                obj->GetModelRenderer()->SetPass(1);
            }

        }

        if (_mapSelectDesc->isCollision == true)
        {
            auto collider = make_shared<AABBBoxCollider>();
            collider->SetBoundingBox(BoundingBox(obj->GetTransform()->GenerateBoundingBox().Center, _mapSelectDesc->extent));
            collider->SetOffset(_mapSelectDesc->offset);
            obj->AddComponent(collider);
            OCTREE->InsertCollider(collider);
            COLLISION->AddCollider(collider);
        }

        if (_mapSelectDesc->isDynamic == true)
        {
           CreateDynamicObject(obj,_mapSelectDesc->dynamicType);
        }
    }
    _mapObjList.push_back(obj);
    return obj;
}


shared_ptr<GameObject> MapManager::Create(MapObjDesc& desc)
{

    shared_ptr<GameObject> obj = make_shared<GameObject>();
    {
        obj->GetOrAddTransform()->SetPosition(desc.pos);
        obj->GetOrAddTransform()->SetLocalRotation(desc.rotation); // XMConvertToRadians()
        obj->GetOrAddTransform()->SetScale(desc.scale);


        auto it = _mapInfoList.find(desc.filename);
        if (it != _mapInfoList.end())
        {
            if (desc.isAnim == true)
            {
                //shared_ptr<ModelAnimator> ma2 = make_shared<ModelAnimator>(it->second->_shader);
                //obj->AddComponent(ma2);
                //obj->GetModelAnimator()->SetModel(it->second->_model);
                //obj->GetModelAnimator()->SetPass(2);

                auto modelrender = make_shared<ModelRenderer>(it->second->_shader);
                obj->AddComponent(modelrender);
                obj->GetModelRenderer()->SetModel(it->second->_model);
                obj->GetModelRenderer()->SetPass(1);
            }
            else if (desc.isMesh == true)
            {
                auto meshrender = make_shared<MeshRenderer>();
                obj->AddComponent(meshrender);


                //auto mesh = RESOURCES->Get<Mesh>(L"Quads");
                auto mesh = make_shared<Mesh>();
                mesh->CreateGrid(1, 1);
                CreateQuadTerrain(mesh, obj, desc.pos);
                obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(desc.filename));
                obj->GetMeshRenderer()->SetMesh(mesh);
                obj->GetMeshRenderer()->SetPass(3);
                obj->GetMeshRenderer()->SetAlphaBlend(false);
                obj->SetObjectType(ObjectType::MapMesh);
            }
            else
            {
                auto modelrender = make_shared<ModelRenderer>(it->second->_shader);
                obj->AddComponent(modelrender);
                obj->GetModelRenderer()->SetModel(it->second->_model);
                obj->GetModelRenderer()->SetPass(1);
            }

        }
        if (desc.isDynamic == true)
        {
            CreateDynamicObject(obj, desc.dynamicType);
        }

        if (desc.isCollision == true)
        {
            auto collider = make_shared<AABBBoxCollider>();
            obj->AddComponent(collider);
            collider->SetOffset(desc.offset);
            collider->SetBoundingBox(BoundingBox(collider->GetBoundingBox().Center, desc.extent));
            OCTREE->InsertCollider(collider);
            COLLISION->AddCollider(collider);
        }


    }
    _mapObjList.push_back(obj);
    return obj;
}


void MapManager::AddMapObj(shared_ptr<MapObjDesc> obj)
{
    // 파일네임이 다를 경우 포인터로 만듦.
    auto it = _mapInfoList.find(obj->filename);
    if (it != _mapInfoList.end())
    {
        return;
    }
    _mapInitInfoList.push_back(obj);


    shared_ptr<MapModel> models = make_shared<MapModel>();

    if (obj->isBillBoard == true)
    {
        CreateBillBoardMesh(obj);
    }
    else if (obj->isAnim == true)
    {
        models->_model = make_shared<Model>();
        {
            models->_model->ReadModel(obj->filename);
            models->_model->ReadMaterial(obj->filename);
        }
        models->_shader = make_shared<Shader>(obj->shadername);
      
        //models->_model->ReadAnimation(obj->filename, AnimationState::Idle);
    }
    else if (obj->isMesh != true)
    {
        models->_model = make_shared<Model>();
        {
            models->_model->ReadModel(obj->filename);
            models->_model->ReadMaterial(obj->filename);
        }
        models->_shader = make_shared<Shader>(obj->shadername);
    }
    else
    {
        models->_shader = make_shared<Shader>(obj->shadername);

        shared_ptr<Mesh> mesh = make_shared<Mesh>();
        mesh->CreateGrid(1, 1);
        models->_mesh = mesh;
        RESOURCES->Add(L"Quads", mesh);

        auto material = make_shared<Material>();
        material->SetShader(models->_shader);
        material->SetName(obj->filename);
        wstring result = _meshFileName + obj->filename;
        auto texture = RESOURCES->Load<Texture>(obj->filename, result);
        material->SetDiffuseMap(texture);
        MaterialDesc& desc = material->GetMaterialDesc();
        desc.ambient = Vec4(1.f);
        desc.diffuse = Vec4(1.f);
        desc.specular = Vec4(1.f);
        RESOURCES->Add(obj->filename, material);

        models->_material = material;
    }

    _mapInfoList.insert(make_pair(obj->filename, models));
}

void MapManager::InitMapText()
{
    _fileTextList.resize(_mapInitInfoList.size());
    for (int i = 0; i < _fileTextList.size(); i++)
    {
        _fileTextList[i].assign(_mapInitInfoList[i]->filename.begin(), _mapInitInfoList[i]->filename.end());
    }
}

Matrix MapManager::UpdateWaveMat()
{
    Matrix result = Matrix::Identity;
    float time = TIME->GetGameTime();

    // time을 라디안값으로 생각하면서 값 계산.
    // -1 ~ 1 사이 값.
    float sinVal = amplitude * XMScalarSin(time * frequency);

    result._21 = sinVal;

    return result;
}

void MapManager::AddBillBoard(Vec3 pos)
{
    _mapBillBoradList.emplace_back(pos);
    _mapBillBoard->GetBillboard()->Add({ pos.x,pos.y + 1.5f,pos.z },{3,3});
}

// IMGUI 초기화 (변수값)
void MapManager::ImGuiSelectMapObject()
{
    ImGui::Begin("SelectMapObject");

    if (ImGui::BeginListBox(" "))
    {
        for (int i = 0; i < _fileTextList.size(); i++)
        {
            const bool is_selected = (_selectedObjIdx == i);
            if (ImGui::Selectable(_fileTextList[i].c_str(), is_selected))
            {
                if (i != _selectedObjIdx)
                {
                    _selectedObjIdx = i;
                    _isSelect = true;
                }
                else
                {
                    _selectedObjIdx = -1;
                    _isSelect = true;
                }

            }

            if (_isSelect && is_selected == true)
            {
                if (_selectedObjIdx >= 0)
                {
                    _mapSelectDesc = _mapInitInfoList[_selectedObjIdx];
                    RemoveMapObj(_mapPreviewObj);
                    PreViewMapObject();
                    _mapPreviewObj = CreatePreViewObj(_pickPos);
                    _isSelect = false;
                }
                else
                {
                    RemoveMapObj(_mapPreviewObj);
                    _isSelect = false;
                    _mapSelectDesc = nullptr;
                }


            }
        }
        ImGui::EndListBox();
    }

    ImGui::End();
}

void MapManager::ChangeMapObjPosition()
{
    Vec3 position = { 0.0f,0.0f,0.0f };
    float dx = INPUT->GetMouseDeltaX() * 0.01f;
    float dy = INPUT->GetMouseDeltaY() * 0.01f;
    if (_isZState)
    {
        position.z = -dy;
    }
    else
    {
        position.x = dx;
        position.y = -dy;
    }

    _mapSelectObj->GetTransform()->AddLocalPosition(position);
}

void MapManager::ChangeMapObjRotation()
{
    Vec3 rotation = { 0.0f,0.0f,0.0f };
    float dx = INPUT->GetMouseDeltaX() * 0.01;
    float dy = INPUT->GetMouseDeltaY() * 0.01;
    if (_isZState)
    {
        rotation.z = -dx;
    }
    else
    {
        rotation.x = -dy;
        rotation.y = -dx;
    }
    _mapSelectObj->GetTransform()->AddLocalRotation(rotation);
}


// TODO : 스케일값을 바꿀 때, 콜라이더도 같이 커지게 하는 기능 필요.
void MapManager::ChangeMapObjScale()
{
    Vec3 scale = { 0.0f,0.0f,0.0f };
    float dx = INPUT->GetMouseDeltaX() * 0.0001;
    float dy = INPUT->GetMouseDeltaY() * 0.0001;

    float all = dx - dy;
    scale.x = all;
    scale.y = all;
    scale.z = all;

    _mapSelectObj->GetTransform()->AddLocalScale(scale);
}

void MapManager::CreateQuadTerrain(shared_ptr<Mesh> mesh, shared_ptr<GameObject> obj, Vec3 pos)
{
    // 맵 오브젝트 설치하듯이 tailtexture01.png이걸 눌러서 Scale 전부 111 만들어 주고 설치했음.

    // 내가 바꿀 Grid에 Mesh-> Vertex 가져옴.                      
    shared_ptr<Geometry<VertexTextureNormalTangentData>> geom = mesh->GetGeometry();
    vector<VertexTextureNormalTangentData> vertices = geom->GetVertices();

    // 1,1 그리드에 스케일값을 곱할 예정이니까 스케일값도 가져와서 정점찾을 때 사용.
    Vec3 scale = obj->GetTransform()->GetLocalScale();
    Vec3 newScale = { scale.x, scale.y, scale.z };
    obj->GetOrAddTransform()->SetLocalScale(newScale);
    
    float offsetY = 0.001f;

    for (int index = 0; index < 4; index++)
    {
        switch (index)
        {
        case 0:
        {
            Vec2 inPos = Vec2{ pos.x,pos.z };
            vertices[index].position.y = _terrain->GetHeightAtPosition(inPos.x, inPos.y) + offsetY;
        }
            break;
        case 1:
        {
            Vec2 inPos = Vec2{ pos.x + scale.x, pos.z };
            vertices[index].position.y = _terrain->GetHeightAtPosition(inPos.x, inPos.y) + offsetY;
        }
            break;
        case 2:
        {
            Vec2 inPos = Vec2{ pos.x,pos.z + scale.z };
            vertices[index].position.y = _terrain->GetHeightAtPosition(inPos.x, inPos.y) + offsetY;
        }
            break;
        case 3:
        {
            Vec2 inPos = Vec2{ pos.x + scale.x ,pos.z + scale.z };
            vertices[index].position.y = _terrain->GetHeightAtPosition(inPos.x, inPos.y) + offsetY;
        }
            break;
        }
    }


    geom->SetVertices(vertices);
    // 메쉬에 있는 버텍스버퍼 정보도 갱신함.
    mesh->GetVertexBuffer()->Create(vertices);
    mesh->GetIndexBuffer()->Create(mesh->GetGeometry()->GetIndices());

}

void MapManager::CreateBillBoardMesh(shared_ptr<MapObjDesc> objs)
{
    // 빌보드 컴포넌트 넣은 오브젝트 생성.
    auto obj = make_shared<GameObject>();
    obj->GetOrAddTransform()->SetLocalPosition(Vec3(0.f));
    obj->AddComponent(make_shared<Billboard>());
    {
        // Material
        {
            auto _shader = make_shared<Shader>(objs->shadername);
            shared_ptr<Material> material = make_shared<Material>();
            material->SetShader(_shader);
            wstring resultFileName = _meshFileName + objs->filename;
            auto texture = RESOURCES->Load<Texture>(objs->filename, resultFileName);
            material->SetDiffuseMap(texture);
            MaterialDesc& desc = material->GetMaterialDesc();
            desc.ambient = Vec4(1.f);
            desc.diffuse = Vec4(1.f);
            desc.specular = Vec4(1.f);
            RESOURCES->Add(objs->filename, material);

            obj->GetBillboard()->SetMaterial(material);
        }
    }
    _mapBillBoard = obj;
    _mapObjList.push_back(obj);
    CUR_SCENE->Add(obj);
}

bool MapManager::ExportMapObj(wstring _fileName)
{
    int length = _mapObjList.size();

    if (length <= 0)
    {
        return true;
    }

    FILE* fp = nullptr;
    errno_t err = _wfopen_s(&fp, _fileName.c_str(), L"wb");
    if (err != 0) return false;

    fwrite(&length, sizeof(int), 1, fp);
    fwrite(&_billBoardCount, sizeof(int), 1, fp);

    for (int i = 0; i < length; i++)
    {
        MapObjDesc dec;
        dec.isAnim = (_mapObjList[i]->GetModelAnimator() == nullptr) ? false : true;
        fwrite(&dec.isAnim, sizeof(bool), 1, fp);

        dec.isDynamic = (_mapObjList[i]->GetDynamicObj() == nullptr) ? false : true;
        fwrite(&dec.isDynamic, sizeof(bool), 1, fp);
        if(dec.isDynamic == true)
        {
            dec.dynamicType = _mapObjList[i]->GetDynamicObj()->GetDynamicType();
            fwrite(&dec.dynamicType, sizeof(DynamicType), 1, fp);
        }

        dec.isMesh = (_mapObjList[i]->GetMeshRenderer() == nullptr) ? false : true;
        fwrite(&dec.isMesh, sizeof(bool), 1, fp);

        dec.isCollision = (_mapObjList[i]->GetCollider() == nullptr) ? false : true;
        fwrite(&dec.isCollision, sizeof(bool), 1, fp);

        dec.isBillBoard = (_mapObjList[i]->GetBillboard() == nullptr) ? false : true;
        fwrite(&dec.isBillBoard, sizeof(bool), 1, fp);


        if (dec.isCollision == true)
        {
            shared_ptr<AABBBoxCollider> collider = dynamic_pointer_cast<AABBBoxCollider>(_mapObjList[i]->GetCollider());
            Vec3 extents = collider->GetBoundingBox().Extents;
            dec.extent = extents;
            fwrite(&dec.extent, sizeof(Vec3), 1, fp);
            dec.offset = _mapObjList[i]->GetCollider()->GetOffset();
            fwrite(&dec.offset, sizeof(Vec3), 1, fp);
        }

        dec.pos = _mapObjList[i]->GetTransform()->GetLocalPosition();
        fwrite(&dec.pos, sizeof(Vec3), 1, fp);
        dec.scale = _mapObjList[i]->GetTransform()->GetLocalScale();
        fwrite(&dec.scale, sizeof(Vec3), 1, fp);
        dec.rotation = _mapObjList[i]->GetTransform()->GetLocalRotation();
        fwrite(&dec.rotation, sizeof(Vec3), 1, fp);

        if (dec.isBillBoard == true)
        {
            continue;
        }

        if (dec.isAnim == true)
        {
            dec.filename = _mapObjList[i]->GetModelAnimator()->GetModel()->GetTextureName();
        }
        else if (dec.isMesh != true)
        {
            dec.filename = _mapObjList[i]->GetModelRenderer()->GetModel()->GetTextureName();
        }
        else
        {
            dec.filename = _mapObjList[i]->GetMeshRenderer()->GetMaterial()->GetName();
        }
        dec.fileLength = dec.filename.length();
        wchar_t szLoadfilename[256] = { 0, };
        dec.filename.copy(szLoadfilename, dec.fileLength);
        fwrite(&dec.fileLength, sizeof(int), 1, fp);
        fwrite(&szLoadfilename, sizeof(wchar_t), dec.fileLength, fp);

        if (dec.isAnim == true)
        {
            dec.shadername = _mapObjList[i]->GetModelAnimator()->GetShader()->GetFile();
        }
        else if (dec.isMesh != true)
        {
            dec.shadername = _mapObjList[i]->GetModelRenderer()->GetShader()->GetFile();
        }
        else
        {
            dec.shadername = _mapObjList[i]->GetMeshRenderer()->GetMaterial()->GetShader()->GetFile();
        }
        dec.shaderLength = dec.shadername.length();
        wchar_t szLoadShadername[256] = { 0, };
        dec.shadername.copy(szLoadShadername, dec.shaderLength);
        fwrite(&dec.shaderLength, sizeof(int), 1, fp);
        fwrite(&szLoadShadername, sizeof(wchar_t), dec.shaderLength, fp);
    }

    for (auto& billboard : _mapBillBoradList)
    {
        fwrite(&billboard, sizeof(Vec3), 1, fp);
    }

    err = fclose(fp);
    if (err != 0) return false;
    _fcloseall();
    return true;
}

bool MapManager::ImportMapObj(wstring _fileName)
{
    FILE* fp = nullptr;
    errno_t err = _wfopen_s(&fp, _fileName.c_str(), L"rb");
    if (err != 0) return false;

    int lengths = 0;
    fread(&lengths, sizeof(int), 1, fp);

    fread(&_billBoardCount, sizeof(int), 1, fp);


    MapObjDesc dec;
    for (int i = 0; i < lengths; i++)
    {
        fread(&dec.isAnim, sizeof(bool), 1, fp);

        fread(&dec.isDynamic, sizeof(bool), 1, fp);
        if (dec.isDynamic == true)
        {
            fread(&dec.dynamicType, sizeof(DynamicType), 1, fp);
        }

        fread(&dec.isMesh, sizeof(bool), 1, fp);

        fread(&dec.isCollision, sizeof(bool), 1, fp);
        
        fread(&dec.isBillBoard, sizeof(bool), 1, fp);

        if (dec.isCollision == true)
        {
            fread(&dec.extent, sizeof(Vec3), 1, fp);
            fread(&dec.offset, sizeof(Vec3), 1, fp);
        }

        fread(&dec.pos, sizeof(Vec3), 1, fp);
        fread(&dec.scale, sizeof(Vec3), 1, fp);
        fread(&dec.rotation, sizeof(Vec3), 1, fp);
        if (dec.isBillBoard == true)
        {
            continue;
        }



        fread(&dec.fileLength, sizeof(int), 1, fp);
        wchar_t szLoadfilename[256] = { 0, };
        fread(szLoadfilename, sizeof(wchar_t), dec.fileLength, fp);
        dec.filename = szLoadfilename;

        fread(&dec.shaderLength, sizeof(int), 1, fp);
        wchar_t szLoadshadername[256] = { 0, };
        fread(szLoadshadername, sizeof(wchar_t), dec.shaderLength, fp);
        dec.shadername = szLoadshadername;

        Create(dec); 
    }

    for (int i = 0; i < _billBoardCount; i++)
    {
        Vec3 tempVec = { 0,0,0 };
        fread(&tempVec, sizeof(Vec3), 1, fp);
        AddBillBoard(tempVec);
    }

    err = fclose(fp);
    if (err != 0) return false;
    _fcloseall();

    for (auto gameObj : _mapObjList)
    {
        SCENE->GetCurrentScene()->Add(gameObj);
    }
    return true;
}

void MapManager::CreateDynamicObject(shared_ptr<GameObject> obj,DynamicType type)
{
    switch (type)
    {
    case DynamicType::None:
        break;
    case DynamicType::Potal:
    {
        obj->AddComponent(make_shared<Potal>(DynamicType::Potal));
    }
        break;
    case DynamicType::Jump:
    {
        obj->AddComponent(make_shared<JumpObj>(DynamicType::Jump));
    }
        break;
    case DynamicType::Save:
    {
        obj->AddComponent(make_shared<SaveObj>(DynamicType::Save));
    }
        break;
    case DynamicType::Heal:
    {
        obj->AddComponent(make_shared<HealObj>(DynamicType::Heal));
    }
    break;
    default:
        break;
    }
}

void MapManager::ClearMap()
{
    // 다음 씬으로 넘어가기 위한 함수
    // shared_ptr 주의
    _mapSelectDesc.reset();
    _mapSelectObj.reset();
    _mapPreviewObj.reset();
    _mapBillBoard.reset();
    _mapSelectDesc = nullptr;
    _mapSelectObj = nullptr;
    _mapPreviewObj = nullptr;
    _mapBillBoard = nullptr;

    _fileTextList.clear();

    // terrain은 이거 전에 set해놔서 reset기달.
    _terrain.reset();

    for (auto& obj : _mapObjList)
    {
        RemoveMapObj(obj);
    }
    _mapObjList.clear();
    _mapInfoList.clear();
    _mapInitInfoList.clear();
}



void MapManager::PreViewMapObject()
{
    POINT screenPt = INPUT->GetMousePos();
    _pickPos = SCENE->GetCurrentScene()->Picking(screenPt.x, screenPt.y);

    if (_mapPreviewObj != nullptr)
    {
        _mapPreviewObj->GetTransform()->SetLocalPosition(_pickPos);
        _mapPreviewObj->GetTransform()->SetLocalRotation(_mapInitInfoList[_selectedObjIdx]->rotation);
        _mapPreviewObj->GetTransform()->SetLocalScale(_mapInitInfoList[_selectedObjIdx]->scale);
    }

}

shared_ptr<GameObject> MapManager::CreatePreViewObj(Vec3 pickPos)
{
    if (_mapSelectDesc->isBillBoard == true)
    {
        return nullptr;
    }
    shared_ptr<GameObject> obj = make_shared<GameObject>();
    {
        obj->GetOrAddTransform()->SetPosition(pickPos);
        obj->GetOrAddTransform()->SetLocalRotation(_mapSelectDesc->rotation); // XMConvertToRadians()
        obj->GetOrAddTransform()->SetScale(_mapSelectDesc->scale);

        if (_mapSelectDesc->isMesh != true)
        {
            shared_ptr<Model> model = make_shared<Model>();
            model->ReadModel(_mapSelectDesc->filename);
            model->ReadMaterial(_mapSelectDesc->filename);
            shared_ptr<Shader> shader = make_shared<Shader>(_mapSelectDesc->shadername);
            auto modelrender = make_shared<ModelRenderer>(shader);
            obj->AddComponent(modelrender);
            obj->GetModelRenderer()->SetModel(model);
            obj->GetModelRenderer()->SetPass(1);
        }
        else
        {
            auto meshrender = make_shared<MeshRenderer>();
            obj->AddComponent(meshrender);
            auto mesh = RESOURCES->Get<Mesh>(L"Quads");
            obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(_mapSelectDesc->filename));
            obj->GetMeshRenderer()->SetMesh(mesh);
            obj->GetMeshRenderer()->SetPass(0);
            obj->GetMeshRenderer()->SetAlphaBlend(true);
        }
    }
    SCENE->GetCurrentScene()->Add(obj);
    return obj;
}

void MapManager::UpdateMapDescTransform()
{
    ImGui::Begin("MapDesc Inspector");


    switch (_transformSelect)
    {
    case 0:
    {
        ImGui::DragFloat("positionX", &_mapSelectDesc->pos.x, 0.1f);
        ImGui::DragFloat("positionY", &_mapSelectDesc->pos.y, 0.1f);
        ImGui::DragFloat("positionZ", &_mapSelectDesc->pos.z, 0.1f);
        break;
    }
    case 1:
    {
        ImGui::DragFloat("rotationX", &_mapSelectDesc->rotation.x, 0.1f);
        ImGui::DragFloat("rotationY", &_mapSelectDesc->rotation.y, 0.1f);
        ImGui::DragFloat("rotationZ", &_mapSelectDesc->rotation.z, 0.1f);
        break;
    }
    case 2:
    {
        ImGui::DragFloat("scaleX", &_mapSelectDesc->scale.x, 0.001f);
        ImGui::DragFloat("scaleY", &_mapSelectDesc->scale.y, 0.001f);
        ImGui::DragFloat("scaleZ", &_mapSelectDesc->scale.z, 0.001f);

        break;
    }
    case 3:
    {
        ImGui::DragFloat("colliderX", &_mapSelectDesc->extent.x, 0.1f);
        ImGui::DragFloat("colliderY", &_mapSelectDesc->extent.y, 0.1f);
        ImGui::DragFloat("colliderZ", &_mapSelectDesc->extent.z, 0.1f);
        ImGui::NewLine;
        ImGui::DragFloat("offsetX", &_mapSelectDesc->offset.x, 0.01f);
        ImGui::DragFloat("offsetY", &_mapSelectDesc->offset.y, 0.01f);
        ImGui::DragFloat("offsetZ", &_mapSelectDesc->offset.z, 0.01f);
        break;
    }
    default:
        break;
    }

    ImGui::End();

}

void MapManager::UpdateMapObjTransform()
{
    ImGui::Begin("MapObject Inspector");

    Vec3 position = _mapSelectObj->GetTransform()->GetLocalPosition();
    Vec3 rotation = _mapSelectObj->GetTransform()->GetLocalRotation();
    Vec3 scale = _mapSelectObj->GetTransform()->GetLocalScale();
    shared_ptr<AABBBoxCollider> collider = dynamic_pointer_cast<AABBBoxCollider>(_mapSelectObj->GetCollider());
    Vec3 extent = collider->GetBoundingBox().Extents;
    Vec3 offset = _mapSelectObj->GetCollider()->GetOffset();

    switch (_transformSelect)
    {
    case 0:
    {
        ImGui::DragFloat("positionX", &position.x, 0.1f);
        ImGui::DragFloat("positionY", &position.y, 0.1f);
        ImGui::DragFloat("positionZ", &position.z, 0.1f);
        _mapSelectObj->GetTransform()->SetLocalPosition(position);
        break;
    }
    case 1:
    {
        ImGui::DragFloat("rotationX", &rotation.x, 0.1f);
        ImGui::DragFloat("rotationY", &rotation.y, 0.1f);
        ImGui::DragFloat("rotationZ", &rotation.z, 0.1f);
        _mapSelectObj->GetTransform()->SetLocalRotation(rotation);
        break;
    }
    case 2:
    {
        ImGui::DragFloat("scaleX", &scale.x, 0.001f);
        ImGui::DragFloat("scaleY", &scale.y, 0.001f);
        ImGui::DragFloat("scaleZ", &scale.z, 0.001f);
        _mapSelectObj->GetTransform()->SetLocalScale(scale);

        break;
    }
    case 3:
    {
        ImGui::DragFloat("extentX", &extent.x, 0.1f);
        ImGui::DragFloat("extentY", &extent.y, 0.1f);
        ImGui::DragFloat("extentZ", &extent.z, 0.1f);
        ImGui::NewLine;
        ImGui::NewLine;
        ImGui::DragFloat("offsetX", &offset.x, 0.01f);
        ImGui::DragFloat("offsetY", &offset.y, 0.01f);
        ImGui::DragFloat("offsetZ", &offset.z, 0.01f);

        collider->SetBoundingBox(BoundingBox({ collider->GetBoundingBox().Center }, extent));
        _mapSelectObj->GetCollider()->SetOffset(offset);
        OCTREE->UpdateCollider(collider);
        break;
    }
    default:
        break;
    }

    ImGui::End();
}



// TODO : 맵 오브젝트가 단 한개도 없으면 파일에 저장되지 않음.
void MapManager::RemoveMapObj(shared_ptr<GameObject> obj)
{
    if (obj != nullptr)
    {
        auto it = std::find(_mapObjList.begin(), _mapObjList.end(), obj);
        if (it != _mapObjList.end())
        {
            _mapObjList.erase(it);
        }

        SCENE->GetCurrentScene()->Remove(obj);
        //COLLISION->GetInstance()->Remove(obj);


        obj.reset();
    }

}
