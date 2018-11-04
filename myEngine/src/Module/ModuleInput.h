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
	A = 4,
	D = 7,
	E = 8,
	I = 12,
	J = 13,
	K = 14,
	L = 15,
	O = 18,
	Q = 20,
	S = 22,
	U = 24,
	W = 26
};

class ModuleInput : public Module
{
public:
	ModuleInput();
	virtual ~ModuleInput();

	bool Init();
	UpdateState PreUpdate();
	bool CleanUp();

	void ManageDropFile(const char* file);

	bool GetKeyDown(KeyCode k)	{ return keyboard[(int)k] == KeyState::KEY_DOWN; }
	bool GetKey(KeyCode k)		{ return keyboard[(int)k] == KeyState::KEY_REPEAT; }
	bool GetKeyUP(KeyCode k)	{ return keyboard[(int)k] == KeyState::KEY_UP; }

	KeyState* keyboard;
};

#endif // !_MODEL_INPUT_H

