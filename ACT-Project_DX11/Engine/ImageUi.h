#pragma once
#include "Ui.h"

class ImageUi : public Ui
{
    using Super = Ui;

public:
    ImageUi();
    virtual ~ImageUi() = default;

    void Create(Vec3 screenPos, Vec2 size, shared_ptr<class Material> material) override;
    void Update() override;

    void SetMaterial(shared_ptr<class Material> material);
    shared_ptr<class Material> GetMaterial() const;

private:
    shared_ptr<class MeshRenderer> _meshRenderer; // 이미지 렌더링용 클래스
};