#ifndef _MODULE_TIME_H
#define _MODULE_TIME_H

#include "Module.h"

class ModuleTime : public Module
{
public:
	bool Init() override;
	UpdateState Update() override;

	void StartTime() { running = true;  }
	void PauseTime() { running = false; }
	void StopTime();

	float UnscaledTime() const { return (float)unscaledRuntime / 1000.F; }
	float UnscaledDeltaTime() const;

	float Time() const { return (float)runtime / 1000.F; }
	float DeltaTime() const;

	float RealTime() const { return (float)(lastTime - startTime) / 1000.F; }
	float RealDeltaTime() const { return (float)deltaTime/1000.F; }
	
	unsigned int RealDeltaTimeMS() const { return deltaTime; }
	float FPS() const { return 1000.0f / (float)deltaTime; }

	void SetTimeScale(float timeScale);
	float GetTimeScale() const { return timeScale; }

private:
	bool running = false;
	float timeScale = 1.0F;

	unsigned int startTime = 0;
	unsigned int lastTime = 0;
	unsigned int deltaTime = 0;

	unsigned int runtime = 0;
	unsigned int unscaledRuntime = 0;
};


#endif // !_MODULE_TIME_H


