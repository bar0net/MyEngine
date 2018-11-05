#ifndef _MODULE_TIME_H
#define _MODULE_TIME_H

#include "Module.h"

class ModuleTime : public Module
{
public:
	bool Init();
	UpdateState Update();

	float Time() { return (float)(lastTime - startTime) * timeScale / 1000.0F; };
	float DeltaTime() { return (float)deltaTime * timeScale / 1000.f; };

	float UnscaledTime() { return (float)(lastTime - startTime) / 1000.0F; }
	float UnscaledDeltaTime() { return (float)deltaTime/1000.f; }

	unsigned int UnscaledDeltaTimeMS() { return deltaTime; }
	float FPS() { return 1000.0f / (float)deltaTime; }

private:
	unsigned long long int frameCount = 0;
	unsigned int startTime = 0;
	unsigned int lastTime = 0;
	unsigned int deltaTime = 0;
	float timeScale = 1.0F;
};


#endif // !_MODULE_TIME_H


