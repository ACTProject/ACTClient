#pragma once

#define DECLARE_SINGLE(classname)			\
private:									\
	classname() { }							\
public:										\
	static classname* GetInstance()			\
	{										\
		static classname s_instance;		\
		return &s_instance;					\
	}

#define GET_SINGLE(classname)	classname::GetInstance()

#define CHECK(p)	assert(SUCCEEDED(p))
#define GAME		GET_SINGLE(Game)		
#define GRAPHICS	GET_SINGLE(Graphics)
#define DEVICE		GRAPHICS->GetDevice()
#define DC			GRAPHICS->GetDeviceContext()
#define INPUT		GET_SINGLE(InputManager)
#define TIME		GET_SINGLE(TimeManager)
#define DT			TIME->GetDeltaTime()
#define RESOURCES	GET_SINGLE(ResourceManager)
#define INSTANCING	GET_SINGLE(InstancingManager)
#define GUI			GET_SINGLE(ImGuiManager)
#define SCENE		GET_SINGLE(SceneManager)
#define DEBUG		GET_SINGLE(DebugManager)
#define MAP			GET_SINGLE(MapManager)
#define COLLISION	GET_SINGLE(CollisionManager)
#define OCTREE		GET_SINGLE(Octree)
#define CUR_SCENE	SCENE->GetCurrentScene()
#define UIMANAGER       GET_SINGLE(UIManager)

enum LayerMask
{
	Layer_Default = 0,
	Layer_UI = 1
};
