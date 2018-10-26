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

	return UpdateState::Update_Continue;
}