#pragma once
#include "Cutscene.h"
class Iexecute;

enum class SceneTag
{
    NONE = 0,
    TITLE = 1,
    INGAME = 2,
    INGAME2 = 3,
};

struct GameDesc
{
	shared_ptr<class IExecute> app = nullptr;
	wstring appName = L"GameCoding";
	HINSTANCE hInstance = 0;
	HWND hWnd = 0;
	float width = 800;
	float height = 600;
	bool vsync = false;
	bool windowed = true;
	Color clearColor = Color(0.5f, 0.5f, 0.5f, 0.5f);
    SceneTag tag = SceneTag::NONE;
};

class Game
{
    DECLARE_SINGLE(Game);
public:
    WPARAM Run(uint32 num);
    GameDesc& GetGameDesc() { return _scenes[_num]; }
    void SetGameDesc(GameDesc desc) { _scenes[_num] = desc; }
    void GameEnd() { _msg.message = WM_QUIT; }
    void AddScene(GameDesc desc) { _scenes.push_back(desc); }
    void ChangeScene(uint32 num);

private:
    ATOM MyRegisterClass();
    BOOL InitInstance(int cmdShow);

    void Update(uint32 num);
    void ShowFps();

    static LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
    // CutScene
    void PlayIntroCutscene();
private:
    GameDesc _desc;
    vector<GameDesc> _scenes;
    MSG _msg = { 0 };
    bool _changeScene = false;
    uint32 _num = 0;
    bool _init = false;

    // Cutscene
    Cutscene _introCutscene; // 게임 첫 시작 컷신
    bool _hasPlayedIntroCutscene = false; // 첫 컷신 재생 여부 확인 변수
};

