#pragma once

enum class KEY_TYPE
{
    UP = VK_UP,
    DOWN = VK_DOWN,
    LEFT = VK_LEFT,
    RIGHT = VK_RIGHT,

	SPACE = VK_SPACE,
	ENTER = VK_RETURN,

	SHIFT = VK_SHIFT,
	CTRL = VK_CONTROL,
	CAPSLOCK = VK_CAPITAL,

    DELETEKEY = VK_DELETE,

    W = 'W',
    A = 'A',
    S = 'S',
    D = 'D',

    Q = 'Q',
    E = 'E',
    Z = 'Z',
    C = 'C',

    R = 'R',
    T = 'T',
    Y = 'Y',
    U = 'U',

    I = 'I',
    O = 'O',
    P = 'P',
    ESC = VK_ESCAPE,


	KEY_1 = '1',
	KEY_2 = '2',
	KEY_3 = '3',
	KEY_4 = '4',

	KEY_F1 = VK_F1,
	KEY_F2 = VK_F2,

	LBUTTON = VK_LBUTTON,
	RBUTTON = VK_RBUTTON,

};

enum class KEY_STATE
{
	NONE,
	PRESS,
	DOWN,
	UP,
	END
};

enum
{
	KEY_TYPE_COUNT = static_cast<int32>(UINT8_MAX + 1),
	KEY_STATE_COUNT = static_cast<int32>(KEY_STATE::END),
};

class InputManager
{
	DECLARE_SINGLE(InputManager);

public:
	void Init(HWND hwnd);
	void Update();

	// 누르고 있을 때
	bool GetButton(KEY_TYPE key) { return GetState(key) == KEY_STATE::PRESS; }
	// 맨 처음 눌렀을 때
	bool GetButtonDown(KEY_TYPE key) { return GetState(key) == KEY_STATE::DOWN; }
	// 맨 처음 눌렀다 뗐을 때
	bool GetButtonUp(KEY_TYPE key) { return GetState(key) == KEY_STATE::UP; }
	
	const POINT& GetMousePos() { return _mousePos; }

	const float GetMouseDeltaX()  { return _deltaX; }
	const float GetMouseDeltaY()  { return _deltaY; }

private:
	inline KEY_STATE GetState(KEY_TYPE key) { return _states[static_cast<uint8>(key)]; }

private:
	HWND _hwnd;
	vector<KEY_STATE> _states;
	POINT _mousePos = {};

private:
	int _prevMouseX;
	int _prevMouseY;
	float _deltaX;
	float _deltaY;
};

