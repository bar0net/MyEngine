#ifndef _MYENGINE_APPLICATION_H
#define _MYENGINE_APPLICATION_H

#include <list>

#include "Module/Module.h"

class ModuleRenderer;
class ModuleInput;

class Application
{
public:
	Application();
	virtual ~Application();

	bool Start();
	bool Update();
	bool End();

	ModuleRenderer* renderer;
	ModuleInput* input;

private:
	std::list<Module *> modules;

};

#endif // !_MYENGINE_APPLICATION_H