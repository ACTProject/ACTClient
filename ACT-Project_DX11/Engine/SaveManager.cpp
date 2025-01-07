#include "pch.h"
#include "SaveManager.h"
#include "MeshRenderer.h"
#include "Button.h"

bool SaveManager::Init()
{
    LoadSaveFiles();
    CreateSaveUI();
    return true;
}

void SaveManager::AddLoadEvent(SaveDataFuncPtr func)
{
    _loadObjList.push_back(func);
}

void SaveManager::AddScene()
{
    for (auto& ui : saveOptionUIGroup)
    {
        CUR_SCENE->Add(ui);
    }
}


void SaveManager::CreateSaveUI()
{
    shared_ptr<Shader> renderUIShader = make_shared<Shader>(L"23. RenderDemoUI.fx");
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
        // char Material
        {
            shared_ptr<Material> material = make_shared<Material>();
            material->SetShader(renderUIShader);
            auto texture = RESOURCES->Load<Texture>(L"Charecter01", L"..\\Resources\\Textures\\UI\\char.png");
            material->SetDiffuseMap(texture);
            MaterialDesc& desc = material->GetMaterialDesc();
            desc.ambient = Vec4(1.f);
            desc.diffuse = Vec4(1.f);
            desc.specular = Vec4(1.f);
            RESOURCES->Add(L"Charecter01", material);
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
        ////////////////////////////////////
        // Option Mesh
        {
            {
                auto obj = make_shared<GameObject>();
                obj->AddComponent(make_shared<Ui>(UiType::BUTTON));
                obj->GetUI()->SetUIState(UiState::STATIC);
                
                obj->AddComponent(make_shared<Button>());
                UIMANAGER->AddButton(obj->GetButton());
                obj->GetButton()->Create(Vec3(400.f, 250.f, -0.3f), Vec2(750, 550), RESOURCES->Get<Material>(L"Option"));
                obj->GetMeshRenderer()->SetAlphaBlend(true);

                saveOptionUIGroup.push_back(obj);
            }
            {
                auto obj = make_shared<GameObject>();
                obj->AddComponent(make_shared<Ui>(UiType::BUTTON));
                obj->GetUI()->SetUIState(UiState::STATIC);

                obj->AddComponent(make_shared<Button>());
                UIMANAGER->AddButton(obj->GetButton());
                obj->GetButton()->Create(Vec3(572, 120, -0.4f), Vec2(220, 120), RESOURCES->Get<Material>(L"Charecter01"));
                obj->GetMeshRenderer()->SetAlphaBlend(true);
                saveOptionUIGroup.push_back(obj);
            }

            // Mesh
            {
                auto obj = make_shared<GameObject>();
                obj->AddComponent(make_shared<Ui>(UiType::BUTTON));
                obj->GetUI()->SetUIState(UiState::STATIC);

                obj->AddComponent(make_shared<Button>());
                UIMANAGER->AddButton(obj->GetButton());
                //obj->GetButton()->Create(Vec3(532, 359, -0.3f), Vec2(175, 38), nullptr);
                obj->GetButton()->Create(Vec3(562, 399, -0.4f), Vec2(228, 56), nullptr);
                obj->GetMeshRenderer()->SetAlphaBlend(true);
                obj->GetButton()->AddOnHoverEvent([obj]() { obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"GameEnd")); });
                obj->GetButton()->AddOnHoverEndEvent([obj]() { obj->GetMeshRenderer()->SetMaterial(nullptr); });
                obj->GetButton()->AddOnClickedEvent([obj]() { if (obj->IsActive())GAME->GameEnd(); });

                saveOptionUIGroup.push_back(obj);
            }

            // Mesh
            {
                auto obj = make_shared<GameObject>();
                obj->AddComponent(make_shared<Ui>(UiType::BUTTON));
                obj->GetUI()->SetUIState(UiState::STATIC);

                obj->AddComponent(make_shared<Button>());
                UIMANAGER->AddButton(obj->GetButton());
                //obj->GetButton()->Create(Vec3(532, 308, -0.3f), Vec2(175, 38), nullptr);
                obj->GetButton()->Create(Vec3(562, 325, -0.4f), Vec2(228, 56), nullptr);
                obj->GetMeshRenderer()->SetAlphaBlend(true);
                obj->GetButton()->AddOnHoverEvent([obj]() { obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"Continue")); });
                obj->GetButton()->AddOnHoverEndEvent([obj]() { obj->GetMeshRenderer()->SetMaterial(nullptr); });
                obj->GetButton()->AddOnClickedEvent([]() {SAVE->SetAllActive(); });
                saveOptionUIGroup.push_back(obj);
            }
        }
    }

    //////////////////////////////////////////////////////////////////////
    ////// save Material
    {
        shared_ptr<Material> material = make_shared<Material>();
        material->SetShader(renderUIShader);
        auto texture = RESOURCES->Load<Texture>(L"Save", L"..\\Resources\\Textures\\UI\\save01.png");
        material->SetDiffuseMap(texture);
        MaterialDesc& desc = material->GetMaterialDesc();
        desc.ambient = Vec4(1.f);
        desc.diffuse = Vec4(1.f);
        desc.specular = Vec4(1.f);
        RESOURCES->Add(L"Save", material);
    }
    {
        shared_ptr<Material> material = make_shared<Material>();
        material->SetShader(renderUIShader);
        auto texture = RESOURCES->Load<Texture>(L"Load", L"..\\Resources\\Textures\\UI\\load01.png");
        material->SetDiffuseMap(texture);
        MaterialDesc& desc = material->GetMaterialDesc();
        desc.ambient = Vec4(1.f);
        desc.diffuse = Vec4(1.f);
        desc.specular = Vec4(1.f);
        RESOURCES->Add(L"Load", material);
    }
    ////////////////// MESH //////////////////////////////////////////
    {
        auto obj = make_shared<GameObject>();
        obj->AddComponent(make_shared<Ui>(UiType::BUTTON));
        obj->GetUI()->SetUIState(UiState::STATIC);
        obj->AddComponent(make_shared<Button>());
        UIMANAGER->AddButton(obj->GetButton());

        obj->GetButton()->Create(Vec3(150.f, 100.f, -0.4f), Vec2(180, 80), RESOURCES->Get<Material>(L"Save"));
        obj->SetObjectType(ObjectType::UI);
        obj->GetMeshRenderer()->SetAlphaBlend(true);
        obj->GetButton()->AddOnClickedEvent([]() { 
            SAVE->SaveGame(CUR_SCENE->GetPlayer());
            SOUND->PlayEffect(L"click"); });
        obj->SetActive(false);
        saveOptionUIGroup.push_back(obj);
    }

    // 기존에 있던 세이브파일 로드
    for (auto& ui : _saveDataList)
    {
        CreateButton();
    }
}

void SaveManager::OpenSaveUI()
{
    _isActive = !_isActive;
    
    if (_isActive)
    {
        for (auto& ui : saveOptionUIGroup)
        {
            ui->SetActive(true);
        }
    }
    else
    {
        SOUND->PlayEffect(L"menu_unpause");
        for (auto& ui : saveOptionUIGroup)
        {
            ui->SetActive(false);
        }
    }
}

bool SaveManager::SaveGame(shared_ptr<GameObject> obj)
{
    // ..\\save\\savefile0.txt
    wstring fileName = _saveFileName + ::to_wstring(_saveFileIndex) + L".txt";
    wstring savefile = _saveFilePath + fileName;

    FILE* fp = nullptr; 
    errno_t err = _wfopen_s(&fp, savefile.c_str(), L"wb");
    if (err != 0) return false;

    // 구조체 내용 채우기.
    SaveData data;
    {
        data.fileIndex = _saveFileIndex;
        data.playerPos = obj->GetOrAddTransform()->GetLocalPosition();
    }

    // 구조체 파일 출력
    fwrite(&data.fileIndex, sizeof(int), 1, fp);
    fwrite(&data.playerPos, sizeof(Vec3), 1, fp);


    // 파일 출력 끝
    err = fclose(fp);
    if (err != 0) return false;
    _fcloseall();

    _saveDataList.push_back(data);

    CreateButton();
    shared_ptr<GameObject> uiObj = saveOptionUIGroup.back();
    uiObj->SetActive(true);
    CUR_SCENE->Add(uiObj);

    _saveFileIndex++;
    return true;
}

void SaveManager::ResizeUI(float width, float height)
{
    if (width > 1800)
    {
        startX = 910.f;
        startY = 300.f;
        padding = 60.f;
        size = { 180, 40 };
    }
    else
    {
        startX = 350.0f;
        startY = 100.f;
        padding = 60.f;
        size = { 180, 40 };
    }
}

void SaveManager::CreateButton()
{
    auto obj = make_shared<GameObject>();
    obj->AddComponent(make_shared<Ui>(UiType::BUTTON));
    obj->GetUI()->SetUIState(UiState::STATIC);
    obj->AddComponent(make_shared<Button>());
    UIMANAGER->AddButton(obj->GetButton());
    obj->GetButton()->Create(Vec3(startX, startY + (padding * index), -0.4f), size, RESOURCES->Get<Material>(L"Load"));
    obj->GetButton()->SetID(_btnIndex);
    obj->SetObjectType(ObjectType::UI);
    obj->GetMeshRenderer()->SetAlphaBlend(true);
    int button = _btnIndex;
       obj->GetButton()->AddOnClickedEvent([this, button]() {  
           SAVE->CheckSaveFile(button); 
           SAVE->SetAllActive();
           SOUND->PlayEffect(L"click"); 
           });
    obj->SetActive(false);

    _btnIndex++;
    index++;
    saveOptionUIGroup.push_back(obj);
}

void SaveManager::SetAllActive()
{
    _isActive = false;
    for (auto ui : saveOptionUIGroup)
    {
        ui->SetActive(false);
    }
}

bool SaveManager::CheckSaveFile(int key)
{
     auto it = std::find_if(_saveDataList.begin(), _saveDataList.end(), [&key](const SaveData& data) {return data.fileIndex == key; });

    if (it != _saveDataList.end())
    {
        // key값을 지닌 세이브 데이터를 얻어왔다
        SaveData savedata = *it;
        LoadGame(savedata);
    }
    return true;
}

bool SaveManager::LoadGame(SaveData& data)
{
    // 플레이어만 바꿀지.
    // 아님 그냥 GameInit부터 시작할지.
    for (auto& func : _loadObjList)
    {
        func(data);
    }
    return true;
}

bool SaveManager::LoadSaveFiles()
{
    wstring str = _saveFilePath + L"*.txt";
    WIN32_FIND_DATAW findFileData;
    HANDLE hFind = FindFirstFileW(str.c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        return false;
    }

    do
    {
        // ../Save/savefile0.txt
        std::wstring fileName = findFileData.cFileName;
        wstring fullFilePath = _saveFilePath + fileName;
        

        // 파일 입력
        FILE* fp = nullptr;
        errno_t err = _wfopen_s(&fp, fullFilePath.c_str(), L"rb");
        if (err != 0) return false;
       
        // 구조체 채우기.
        SaveData data;
        {
            fread(&data.fileIndex,sizeof(int), 1, fp);
            fread(&data.playerPos,sizeof(Vec3), 1, fp);
        }

        // 파일 입력 끝
        err = fclose(fp);
        if (err != 0) return false;
        _fcloseall();

        // 데이터 맵에 저장.
        _saveDataList.push_back(data);
        _saveFileIndex++;

    } while (FindNextFileW(hFind, &findFileData) != 0);
    FindClose(hFind);

    return true;
}

