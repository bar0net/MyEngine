#ifndef _MODULE_TIME_H
#define _MODULE_TIME_H

#include "Module.h"

class ModuleTime : public Module
{
public:
	bool Init() override;
	UpdateState Update() override;

	float DeltaTime() { return (float)deltaTime/1000.f; }
	unsigned int DeltaTimeMS() { return deltaTime; }
	float FPS() { return 1000.0f / (float)deltaTime; }

private:
	unsigned int startTime = 0;
	unsigned int lastTime = 0;
	unsigned int deltaTime = 0;
};


#endif // !_MODULE_TIME_H


