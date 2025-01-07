#pragma once
#include "Component.h"

enum class UiType
{
    NONE,
    SLIDER,
    BUTTON,
    TEXT,
    IMAGE,
    END,
};

enum class UiState
{
    NONE,
    STATIC,
    HOVERED,
    PRESSED,
    DISABLED,
    END,
};

class Ui : public Component
{
    using Super = Component;
public:
    Ui(UiType type) : Super(ComponentType::UI), _type(type) {};
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
    const Vec3& GetPositionUI() const { return _addPos; }

    void SetUIState(const UiState id) { _state = id; }
    const UiState GetUIState() const { return _state; }

    // 이벤트 핸들링
    void SetOnClick(const function<void()>& callback);
    void SetOnHover(const function<void()>& callback);
    void SetOnPress(const function<void()>& callback);

protected:
    void PreUpdate();
protected:
    UiType _type = UiType::NONE;
    UiState _state = UiState::NONE;
    bool _isVisible = true;
    Vec3 _addPos = { 0, 3.0f ,0 };

    // 오너가 있으면 active=false로 시작.
    weak_ptr<GameObject> _owner;
    string _uiID;

    // 이벤트 핸들러
    function<void()> _onClick;
    function<void()> _onHover;
    function<void()> _onPress;
};

