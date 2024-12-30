#include "pch.h"
#include "Main.h"
#include "Engine/Game.h"
#include "Client.h"
#include "Title.h"
#include "EffectTest.h"
#include "Client2.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    {
        GameDesc desc;
        desc.appName = L"GameCoding";
        desc.hInstance = hInstance;
        desc.vsync = false;
        desc.hWnd = NULL;
        desc.width = 800;
        desc.height = 600;
        desc.clearColor = Color(0.f, 0.5f, 1.0f, 0.5f);
        desc.app = make_shared<Title>(); // 0
        desc.tag = SceneTag::TITLE;
        GAME->AddScene(desc);
    }
	
    {
        GameDesc desc;
        desc.appName = L"GameCoding";
        desc.hInstance = hInstance;
        desc.vsync = false;
        desc.hWnd = NULL;
        desc.width = 800;
        desc.height = 600;
        desc.clearColor = Color(0.f, 0.5f, 1.0f, 0.5f);
        desc.app = make_shared<Client>(); // 1
        desc.tag = SceneTag::INGAME;
        GAME->AddScene(desc);
    }

    {
        GameDesc desc;
        desc.appName = L"GameCoding";
        desc.hInstance = hInstance;
        desc.vsync = false;
        desc.hWnd = NULL;
        desc.width = 800;
        desc.height = 600;
        desc.clearColor = Color(0.f, 0.5f, 1.0f, 0.5f);
        desc.app = make_shared<Client2>(); // 2
        desc.tag = SceneTag::INGAME2;
        GAME->AddScene(desc);
    }

	GAME->Run(0);

	return 0;
}
