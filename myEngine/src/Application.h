#ifndef _MYENGINE_APPLICATION_H
#define _MYENGINE_APPLICATION_H

#include <list>

#include "Module/Module.h"

class ModuleRenderer;
class ModuleInput;
class ModuleEditor;
class ModuleTime;

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
	ModuleEditor* editor;
	ModuleTime* time;

private:
	std::list<Module *> modules;
};

extern Application* App;

#endif // !_MYENGINE_APPLICATION_H