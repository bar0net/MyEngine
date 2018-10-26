#ifndef _MODULE_EDITOR_H
#define _MODULE_EDITOR_H

#include "Module.h"

struct ImGuiIO;
class MovingArray;

class ModuleEditor : public Module
{
public:
	bool Init();
	UpdateState Update();
	bool CleanUp();
	
	void ProcessEvent(void* event);

private:
	void FrameStart();
	void FrameEnd();

	ImGuiIO* io;
	MovingArray* fps;
};


#endif // !_MODULE_EDITOR_H

