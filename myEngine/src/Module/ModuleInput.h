#ifndef _MODEL_INPUT_H
#define _MODEL_INPUT_H

#include "Module.h"

#include <unordered_map>

enum class KeyState 
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

enum class KeyCode
{
	A =  4, B =  5, C =  6, D =  7, E =  8, F =  9, G = 10,
	H = 11, I = 12, J = 13, K = 14, L = 15, M = 16, N = 17,
	O = 18, P = 19, Q = 20, R = 21, S = 22, T = 23, U = 24,
	V = 25, W = 26, X = 27, Y = 28, Z = 29,

	LSHIFT = 225,
	LALT = 226
};

enum class MouseButton
{
	MOUSE_LEFT = 0,
	MOUSE_RIGHT = 1,
	MOUSE_MIDDLE = 2
};

class ModuleInput : public Module
{
public:
	ModuleInput();
	virtual ~ModuleInput();

	bool Init() override;
	UpdateState PreUpdate() override;
	UpdateState PostUpdate() override;
	bool CleanUp() override;

	void ManageDropFile(const char* file);

	bool GetKeyDown(KeyCode k)	{ return keyboard[(int)k] == KeyState::KEY_DOWN; }
	bool GetKey(KeyCode k)		{ return keyboard[(int)k] == KeyState::KEY_REPEAT; }
	bool GetKeyUP(KeyCode k)	{ return keyboard[(int)k] == KeyState::KEY_UP; }

	bool GetMouseButtonDown(MouseButton button) { return mouse[(int)button] == KeyState::KEY_DOWN; }
	bool GetMouseButton(MouseButton button) { return mouse[(int)button] == KeyState::KEY_REPEAT; }
	bool GetMouseButtonUp(MouseButton button) { return mouse[(int)button] == KeyState::KEY_UP; }
	void GetMouseMovement(float* x, float* y);
	void GetMousePosition(int* x, int* y);
	float GetMouseWheel() { return wheel_sensitivity * (float)mouse_wheel; };


private:
	KeyState* keyboard;
	KeyState* mouse;
	int mouse_x = 0;
	int mouse_y = 0;
	int mouse_wheel = 0;

public:
	float mouse_senitivity = 0.2F;
	float wheel_sensitivity = 5.0f;
};

#endif // !_MODEL_INPUT_H

