#pragma once
#include "MonoBehaviour.h"

class CameraController : public MonoBehaviour
{
public:
	virtual void Start() override;
	virtual void Update() override;

	float _speed = 10.f;
};

