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

void SaveManager::CreateSaveUI()
{
    //shared_ptr<Shader> renderUIShader = make_shared<Shader>(L"23. RenderDemoUI.fx");
    //// option Material
    //{
    //    shared_ptr<Material> material = make_shared<Material>();
    //    material->SetShader(renderUIShader);
    //    auto texture = RESOURCES->Load<Texture>(L"Option", L"..\\Resources\\Textures\\UI\\option.png");
    //    material->SetDiffuseMap(texture);
    //    MaterialDesc& desc = material->GetMaterialDesc();
    //    desc.ambient = Vec4(1.f);
    //    desc.diffuse = Vec4(1.f);
    //    desc.specular = Vec4(1.f);
    //    RESOURCES->Add(L"Option", material);
    //}


    //{
    //    auto obj = make_shared<GameObject>();
    //    obj->AddComponent(make_shared<Button>());

    //    obj->GetButton()->Create(Vec3(400.f, 250.f, 0.4f), Vec2(595, 404), RESOURCES->Get<Material>(L"Option"));
    //    obj->GetMeshRenderer()->SetAlphaBlend(true);
    //    obj->GetButton()->AddOnKeyPressEvent(KEY_TYPE::ESC, [obj]() { obj->SetActive(!obj->IsActive()); });

    //    obj->SetActive(!obj->IsActive());
    //    saveOptionUIGroup.push_back(obj);
    //}

    //// Mesh
    //{
    //    auto obj = make_shared<GameObject>();
    //    obj->GetOrAddTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.5f));
    //    obj->GetOrAddTransform()->SetScale(Vec3(800.f, 600.f, 0.f));
    //    obj->AddComponent(make_shared<MeshRenderer>());

    //    obj->SetLayerIndex(Layer_UI);
    //    {
    //        obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"Background"));
    //    }
    //    {
    //        auto mesh = RESOURCES->Get<Mesh>(L"Quad");
    //        obj->GetMeshRenderer()->SetMesh(mesh);
    //        obj->GetMeshRenderer()->SetPass(0);
    //    }

    //    CUR_SCENE->Add(obj);
    //}


    //// Button Mesh
    //{
    //    auto obj = make_shared<GameObject>();
    //    obj->AddComponent(make_shared<Button>());

    //    obj->GetButton()->Create(Vec3(250, 300, 0.3), Vec2(166, 44), RESOURCES->Get<Material>(L"StartBtn"));
    //    obj->GetMeshRenderer()->SetAlphaBlend(true);
    //    obj->GetButton()->AddOnHoverEvent([obj]() { obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"StartBtn_Hover")); });
    //    obj->GetButton()->AddOnHoverEndEvent([obj]() { obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"StartBtn")); });

    //    obj->GetButton()->AddOnClickedEvent([]() { GAME->ChangeScene(1); });

    //    CUR_SCENE->Add(obj);
    //}
}

void SaveManager::OpenSaveUI()
{



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
    _saveFileIndex++;
    return true;
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

