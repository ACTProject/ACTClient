#include "pch.h"
#include "SaveManager.h"

bool SaveManager::Init()
{
    LoadSaveFiles();
    return true;
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

