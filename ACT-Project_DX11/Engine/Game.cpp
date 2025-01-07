#include "pch.h"
#include "Game.h"
#include "IExecute.h"
#include "GameObject.h"

HWND g_hWnd = nullptr;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

WPARAM Game::Run(uint32 num)
{
    if (num < _scenes.size() && num >= 0)
    {
        _num = num;
    }
    assert(_scenes[_num].app != nullptr);

    if (g_hWnd == nullptr)
    {
        // 1) 윈도우 창 정보 등록
        MyRegisterClass();

        // 2) 윈도우 창 생성
        if (!InitInstance(SW_SHOWNORMAL))
            return FALSE;

        GRAPHICS->Init(g_hWnd);
        TIME->Init();
        INPUT->Init(g_hWnd);
    }

    _scenes[_num].hWnd = g_hWnd;
    // Save Data Load
    if (_scenes[_num].tag == SceneTag::TITLE)
    {
        SAVE->Init();
    }
	//0
    DEBUG->CreateConsoleWindow();
    COLLISION->Init();
    // Octree
    {
        // 전체 월드 공간 옥트리
        BoundingBox worldBounds(Vec3(200.f, 70.f, 200.f), Vec3(350.f, 100.f, 350.f));
        int maxDepth = 4;
        OCTREE->Init(worldBounds, maxDepth);
    }
    //1
	GUI->Init();
	RESOURCES->Init();
    if (_scenes[_num].tag == SceneTag::TITLE)
    {
        MAP->Init();
    }
	SCENE->Awake();
    _scenes[_num].app->Init(); // 게임오브젝트 생성

    if (_scenes[_num].tag == SceneTag::INGAME)
    {
        MAP->ImportMapObj(L"../Resources/MapFile/MapObjectLists.txt");
        SAVE->AddScene();
    }
    if (_scenes[_num].tag == SceneTag::INGAME2)
    {
        MAP->ClearMap();
        MAP->ImportMapObj(L"../Resources/MapFile/MapObjectLists02.txt");
        SAVE->AddScene();
    }

	SCENE->Start();

    if (_scenes[_num].tag == SceneTag::INGAME)
    {
        SHADOW->Init();
        // SHADOW RENDER //
        GRAPHICS->RenderShadowBegin();
        SHADOW->Update();
        ///////////////////
    }

    _init = true;

	while (_msg.message != WM_QUIT)
	{
		if (::PeekMessage(&_msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&_msg);
			::DispatchMessage(&_msg);
		}
		else
		{
            if (_changeScene)
            {
                CUR_SCENE->Clear();
                _changeScene = false;
                return Run(_num);
            }
			Update(_num);
		}
	}
    if (_scenes[_num].tag == SceneTag::INGAME)
    {
        MAP->ExportMapObj(L"../Resources/MapFile/MapObjectLists.txt");
    }
    if (_scenes[_num].tag == SceneTag::INGAME2)
    {
        MAP->ExportMapObj(L"../Resources/MapFile/MapObjectLists02.txt");
    }

	return _msg.wParam;
}

void Game::Update(uint32 _num)
{
	TIME->Update();
	INPUT->Update();
	ShowFps();



    GRAPHICS->RenderBegin();
	GUI->Update();

	SCENE->FixedUpdate();
	SCENE->Update();

	_scenes[_num].app->Update();
    _scenes[_num].app->Render();
	GUI->Render();

	GRAPHICS->RenderEnd();

    SOUND->Update();


}

void Game::ShowFps()
{
	uint32 fps = GET_SINGLE(TimeManager)->GetFps();

	WCHAR text[100] = L"";
	::wsprintf(text, L"FPS : %d", fps);

	::SetWindowText(_scenes[_num].hWnd, text);

}

void Game::ChangeScene(uint32 num, bool previousSceneShell, shared_ptr<GameObject> previousShellObject)
{
    _num = num;
    _isPreviousSceneShellEquipped = previousSceneShell;
    _previousShellObject = previousShellObject;
    _changeScene = true;
}

ATOM Game::MyRegisterClass()
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = _desc.hInstance;
	wcex.hIcon = ::LoadIcon(NULL, IDI_WINLOGO);
	wcex.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = _desc.appName.c_str();
	wcex.hIconSm = wcex.hIcon;

	return RegisterClassExW(&wcex);
}

BOOL Game::InitInstance(int cmdShow)
{
	RECT windowRect = { 0, 0, _scenes[0].width, _scenes[0].height};
	::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);
    
    g_hWnd = CreateWindowW(_scenes[0].appName.c_str(), _scenes[0].appName.c_str(), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, nullptr, nullptr, _scenes[0].hInstance, nullptr);

	if (!g_hWnd)
		return FALSE;

	::ShowWindow(g_hWnd, cmdShow);
	::UpdateWindow(g_hWnd);

	return TRUE;
}

LRESULT CALLBACK Game::WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(handle, message, wParam, lParam))
		return true;

	switch (message)
	{
    case WM_SIZE:
    {
        if (!GAME->_init)
            break;
        if (wParam != SIZE_MINIMIZED)
        {
            int width = LOWORD(lParam);  // 새로운 창 너비
            int height = HIWORD(lParam); // 새로운 창 높이
            GRAPHICS->OnResize(width, height);
        }
        break;
    }
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return ::DefWindowProc(handle, message, wParam, lParam);
	}
}

HWND Game::GetHWND()
{
    return g_hWnd;
}
