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

private:
    // 슬라이더 스케일 변화 함수.
    void UpdateScale();
private:
	float _maxRatio = 0.0f;
	float _uvRatio = 1.0f;
};
