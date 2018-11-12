#include "ModuleTime.h"

#include "SDL.h"

bool ModuleTime::Init()
{
	startTime = SDL_GetTicks();
	lastTime = SDL_GetTicks();

	return true;
}

UpdateState ModuleTime::Update()
{

	deltaTime = SDL_GetTicks() - lastTime;
	lastTime = SDL_GetTicks();

	if (running)
	{
		runtime += deltaTime * timeScale;
		unscaledRuntime += deltaTime;
	}

	return UpdateState::Update_Continue;
}

void ModuleTime::StopTime()
{
	running = false;
	runtime = 0;
	unscaledRuntime = 0;
}

float ModuleTime::UnscaledDeltaTime() const
{
	if (!running) return 0.0f;

	return (float)deltaTime / 1000.f;
}

float ModuleTime::DeltaTime() const
{
	if (!running) return 0.0f;
	
	return ((float)deltaTime * timeScale) / 1000.f;
}

void ModuleTime::SetTimeScale(float timeScale)
{
	if (timeScale < 0) return;

	this->timeScale = timeScale;
}
