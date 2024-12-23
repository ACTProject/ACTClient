#pragma once
#include "Ui.h"


class Button : public Component
{
	using Super = Component;

public:
	Button();
	virtual ~Button();

	bool Picked(POINT screenPos);

	void Create(Vec3 screenPos, Vec2 size, shared_ptr<class Material> material);
	void AddOnClickedEvent(std::function<void(void)> func);
	void InvokeOnClicked();

    void AddOnHoverEvent(std::function<void(void)> func);
    void AddOnHoverEndEvent(std::function<void(void)> func);
    void CheckHover(POINT screenPos);

    void AddOnKeyPressEvent(KEY_TYPE key, std::function<void(void)> func);
    void InvokeOnKeyPress(KEY_TYPE key);
    void CheckKeyInput();

private:
	std::function<void(void)> _onClicked;
    std::function<void(void)> _onHover;
    std::function<void(void)> _onHoverEnd;
    std::unordered_map<KEY_TYPE, std::function<void(void)>> _onKeyPress;
    bool _isHoverd = false;
	RECT _rect;
};

