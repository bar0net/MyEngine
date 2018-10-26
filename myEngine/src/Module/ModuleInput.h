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

class ModuleInput : public Module
{
public:
	ModuleInput();
	virtual ~ModuleInput();

	bool Init();
	UpdateState PreUpdate();
	bool CleanUp();

	KeyState* keyboard;
	std::unordered_map<const char*, unsigned int> keyCode;
};

#endif // !_MODEL_INPUT_H

