#include "pch.h"
#include "Game.h"
#include "IExecute.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

WPARAM Game::Run(GameDesc& desc)
{
	_desc = desc;
	assert(_desc.app != nullptr);

	// 1) 윈도우 창 정보 등록
	MyRegisterClass();

	// 2) 윈도우 창 생성
	if (!InitInstance(SW_SHOWNORMAL))
		return FALSE;
	
    DEBUG->CreateConsoleWindow();
    // Octree
    {
        // 전체 월드 공간 옥트리
        BoundingBox worldBounds(Vec3(300.f, 100.f, 300.f), Vec3(300.f, 100.f, 300.f));
        int maxDepth = 4;
        OCTREE->Init(worldBounds, maxDepth);
    }

	GRAPHICS->Init(_desc.hWnd);
	TIME->Init();
	INPUT->Init(_desc.hWnd);
	GUI->Init();
	RESOURCES->Init();
	
	MAP->Init();
	SCENE->Awake();
	_desc.app->Init(); // 게임오브젝트 생성
	SCENE->Start();

    SHADOW->Init();



    _init = true;



	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{
			Update();
		}
	}
	MAP->ExportMapObj();

	return msg.wParam;
}

void Game::Update()
{
	TIME->Update();
	INPUT->Update();
	ShowFps();

    // SHADOW RENDER //
    GRAPHICS->RenderShadowBegin();
    SHADOW->Update();
    SHADOW->RenderShadow();
    ///////////////////

    GRAPHICS->RenderBegin();
	GUI->Update();


    SHADOW->updatelightPos();

	SCENE->FixedUpdate();
	SCENE->Update();

	_desc.app->Update();
	_desc.app->Render();
	GUI->Render();

	GRAPHICS->RenderEnd();




}

void Game::ShowFps()
{
	uint32 fps = GET_SINGLE(TimeManager)->GetFps();

	WCHAR text[100] = L"";
	::wsprintf(text, L"FPS : %d", fps);

	::SetWindowText(_desc.hWnd, text);

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
	RECT windowRect = { 0, 0, _desc.width, _desc.height };
	::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

	_desc.hWnd = CreateWindowW(_desc.appName.c_str(), _desc.appName.c_str(), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, nullptr, nullptr, _desc.hInstance, nullptr);

	if (!_desc.hWnd)
		return FALSE;

	::ShowWindow(_desc.hWnd, cmdShow);
	::UpdateWindow(_desc.hWnd);

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