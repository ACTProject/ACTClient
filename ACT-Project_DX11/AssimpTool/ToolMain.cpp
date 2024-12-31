#include "pch.h"
#include "ToolMain.h"
#include "Engine/Game.h"
#include "AssimpTool.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    GameDesc desc;
    desc.appName = L"GameCoding";
    desc.hInstance = hInstance;
    desc.vsync = false;
    desc.hWnd = NULL;
    desc.width = 800;
    desc.height = 600;
    desc.clearColor = Color(0.f, 0.f, 0.f, 0.f);
    desc.tag = SceneTag::NONE;
    desc.app = make_shared<AssimpTool>();

    GAME->AddScene(desc);
    GAME->Run(0);

	return 0;
}
