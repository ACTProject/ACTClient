#include "pch.h"
#include "Camera.h"
#include "CameraController.h"
#include "Transform.h"

void CameraController::Start()
{

}

void CameraController::Update()
{
	ProcessInput();	
}

void CameraController::ProcessInput()
{
	if (INPUT->GetButtonDown(KEY_TYPE::KEY_F1)) // F1 디버그 모드 on/off
	{
		bool currentState = DEBUG->IsDebugEnabled();
		DEBUG->EnableDebug(!currentState);
	}
}
