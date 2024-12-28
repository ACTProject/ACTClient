#pragma once

struct SaveData
{
    int fileIndex = 0;
    Vec3 playerPos = { 0,0,0 };
    //float playerHp = 0;
    //float _shellHp;
    //현재 씬 num

};


class SaveManager
{
    DECLARE_SINGLE(SaveManager);
public:
    bool Init();
public:
    // 플레이어가 키 눌렀을 때, 호출되는 함수, SaveData를 파일로 저장한다.
    bool SaveGame(shared_ptr<GameObject> obj);


    // 어느 세이브파일이 선택됐는지 걸러주는 함수.
    bool CheckSaveFile(int key);
    // 선택한 세이브파일을 불러들인다. 버튼을 클릭하면 
    bool LoadGame(SaveData& data);
private:
    // 저장되있는 세이브 파일을 전부 불러오는 함수. Init에 추가했다.
    bool LoadSaveFiles();
private:
    int _saveFileIndex = 0;
    wstring _saveFilePath = L"..\\Save\\";
    wstring _saveFileName = L"savefile";
    vector<SaveData> _saveDataList;
};

