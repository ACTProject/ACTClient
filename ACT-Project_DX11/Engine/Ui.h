#pragma once
#include "Component.h"

enum class UiType
{
    NONE,
    SLIDER,
    BUTTON,

    END,
};

class Ui : public Component
{
    using Super = Component;
public:
    Ui(UiType type = UiType::NONE) : Super(ComponentType::UI), _uType(type) { };
    virtual ~Ui() = default;
public:
    void SetOwner(weak_ptr<GameObject> obj);
    const shared_ptr<GameObject> GetOwner() { return _owner.lock(); };
    void SetActive(bool active) { _isVisible = active; };
    const bool GetActive() { return _isVisible; };

    virtual void Create(Vec3 screenPos, Vec2 size, shared_ptr<class Material> material) { };
    virtual void Update() { };

    void SetUIID(const string& id) { _uiID = id; }
    const string& GetUIID() const { return _uiID; }
    void SetPositionUI(Vec3 addPos) { _addPos = addPos; }

    Vec3 GetScreenPos() { return _screenPos; };
    void SetScreenPos(Vec3 pos) { _screenPos = pos; };

    Vec2 GetSize() { return _size; };
    void SetSize(Vec2 size) { _size = size; };

    UiType GetuType() { return _uType; };
    void SetuType(UiType uType) { _uType = uType; };
protected:
    void PreUpdate();
protected:
    UiType _uType = UiType::NONE;
    bool _isVisible = true;
    Vec3 _addPos = { 0, 3.0f ,0 };

    Vec3 _screenPos = { 0,0,0 };

    Vec2 _size;

    // 오너가 있으면 active=false로 시작.
    weak_ptr<GameObject> _owner;
    string _uiID;
};

