﻿#pragma once
#include "Component.h"

class HitBox : public Component
{
	using Super = Component;
public:
	HitBox();
	virtual ~HitBox();

	Vec3 GetOffSet() { return _offset; }
	void SetOffSet(Vec3 offset) { _offset = offset; }
	bool GetActive() { return _active; }
	void SetActive(bool active) { _active = active; }

	void Craete(shared_ptr<GameObject> target, Vec3 size);

private:
	Vec3 _offset = Vec3(0.f);
	bool _active = false;
};
