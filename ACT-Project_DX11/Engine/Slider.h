#pragma once
#include "Ui.h"
class Slider : public Ui
{
	using Super = Ui;
public:
	Slider();
	virtual ~Slider();

	virtual void Update() override;
	virtual void Create(Vec3 screenPos, Vec2 size, shared_ptr<class Material> material) override;

    float GetRatio() { return _uvRatio; };
    void SetRatio(float ratio) { _uvRatio = ratio; };

    void SetMaxRatio(float ratio) { _maxRatio = ratio; };

    Vec3 GetSub() { return _sub; };
    void SetSub(Vec3 sub) { _sub = sub; };
private:
    // 슬라이더 스케일 변화 함수.
    void UpdateScale();
private:
	float _maxRatio = 0.0f;
	float _uvRatio = 1.0f;
    // 슬라이더는 screenPos가 부모위치고 thisPos가 자기위치임.
    Vec3 _thisPos;
    Vec3 _sub;
};
