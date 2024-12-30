#pragma once

struct SaveData
{
    int fileIndex = 0;
    Vec3 playerPos = { 0,0,0 };
    //float playerHp = 0;
    //float _shellHp;
    //현재 씬 num

};
// 모델 애니메이션
// ui 버튼 추가.

class SaveManager
{
    DECLARE_SINGLE(SaveManager);
public:
    using SaveDataFuncPtr = std::function<void(SaveData)>;

    bool Init();
public:
    // 버튼을 눌렀을 때, 발생할 함수포인터 저장.
    void AddLoadEvent(SaveDataFuncPtr func);

    // Save버튼 ui
    void CreateSaveUI();
    void OpenSaveUI();


    // 플레이어가 키 눌렀을 때, 호출되는 함수, SaveData를 파일로 저장한다. => 나중엔 버튼이벤트로 변경할 예정.
    bool SaveGame(shared_ptr<GameObject> obj);
private:
    // 어느 세이브파일이 선택됐는지 걸러주는 함수.
    // 만약 구조에서 나중엔 그냥 선택된 SaveData구조체를 넘겨줄 수 도.
    bool CheckSaveFile(int key);
    // 선택한 세이브파일을 불러들인다. 버튼을 클릭하면 -> 이건 나중에 플레이어한테 넘겨줄수도.
    bool LoadGame(SaveData& data);

    // 저장되있는 세이브 파일을 전부 불러오는 함수. Init에 추가했다.
    bool LoadSaveFiles();
private:
    vector<shared_ptr<GameObject>> saveOptionUIGroup;

    int _saveFileIndex = 0;
    wstring _saveFilePath = L"..\\Save\\";
    wstring _saveFileName = L"savefile";
    vector<SaveData> _saveDataList;


    vector<SaveDataFuncPtr> _loadObjList;
};

