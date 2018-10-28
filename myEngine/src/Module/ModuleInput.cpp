#include "ModuleInput.h"

#include "SDL.h"
#include "../Application.h"
#include "ModuleEditor.h"
#include "ModuleRenderer.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput() : Module()
{
	keyboard = new KeyState[MAX_KEYS];
	memset(keyboard, 0, MAX_KEYS * sizeof(KeyState));
}

ModuleInput::~ModuleInput()
{
	delete[]keyboard;
}

bool ModuleInput::Init()
{
	LOGINFO("Initialize input system.");
	SDL_Init(0);
	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOGERROR("Input System could not initialize: %s", SDL_GetError());
		return false;
	}

	return true;
}

UpdateState ModuleInput::PreUpdate()
{
	static SDL_Event event;

	const Uint8* keys = SDL_GetKeyboardState(nullptr);
	for (unsigned int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KeyState::KEY_IDLE) keyboard[i] = KeyState::KEY_DOWN;
			else keyboard[i] = KeyState::KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KeyState::KEY_REPEAT || keyboard[i] == KeyState::KEY_DOWN)
				keyboard[i] = KeyState::KEY_UP;
			else
				keyboard[i] = KeyState::KEY_IDLE;
		}
	}

	while (SDL_PollEvent(&event) != 0)
	{
		App->editor->ProcessEvent(&event);
		if (event.type == SDL_QUIT)
			return UpdateState::Update_End;
		
		if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				App->renderer->ResizedWindow();		
	}

	return UpdateState::Update_Continue;
}

bool ModuleInput::CleanUp()
{
	LOGINFO("Close input system.");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}