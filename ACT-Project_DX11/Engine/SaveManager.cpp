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
    //////////////////
    {
        auto obj = make_shared<GameObject>();
        obj->AddComponent(make_shared<Button>());
        UIMANAGER->AddButton(obj->GetButton());

        obj->GetButton()->Create(Vec3(100.f, 100.f, 0.1f), Vec2(180, 80), RESOURCES->Get<Material>(L"Save"));
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

void SaveManager::CreateButton()
{
    auto obj = make_shared<GameObject>();
    obj->AddComponent(make_shared<Button>());
    UIMANAGER->AddButton(obj->GetButton());
    obj->GetButton()->Create(Vec3(300.f, startY + (padding * index), 0.1f), Vec2(180, 40), RESOURCES->Get<Material>(L"Load"));
    obj->GetButton()->SetID(_btnIndex);
    obj->SetObjectType(ObjectType::UI);
    obj->GetMeshRenderer()->SetAlphaBlend(true);
    int button = _btnIndex;
       obj->GetButton()->AddOnClickedEvent([this, button]() {  
           SAVE->CheckSaveFile(button); 
           SOUND->PlayEffect(L"click"); 
           });
    obj->SetActive(false);

    _btnIndex++;
    index++;
    saveOptionUIGroup.push_back(obj);
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

