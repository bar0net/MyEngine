#include "ModuleInput.h"

#include "SDL.h"

#include "ModuleEditor.h"
#include "ModuleRenderer.h"
#include "ModuleScene.h"

#include "Application.h"
#include "Utils/Window_Utils.h"

#define MAX_KEYS 300
#define MOUSE_KEYS 3

ModuleInput::ModuleInput() : Module()
{
	keyboard = new KeyState[MAX_KEYS];
	mouse = new KeyState[MOUSE_KEYS];

	memset(keyboard, 0, MAX_KEYS * sizeof(KeyState));
	memset(mouse, 0, MOUSE_KEYS * sizeof(KeyState));
}

ModuleInput::~ModuleInput()
{
	delete[] keyboard;
	delete[] mouse;
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
	SDL_EventState(SDL_DROPFILE, SDL_ENABLE);

	return true;
}

UpdateState ModuleInput::PreUpdate()
{
	static SDL_Event event;

	mouse_wheel = 0;
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

	for (unsigned int i = 0; i < MOUSE_KEYS; ++i) 
	{
		if (mouse[i] == KeyState::KEY_DOWN)		
			mouse[i] = KeyState::KEY_REPEAT;
		else if (mouse[i] == KeyState::KEY_UP)	
			mouse[i] = KeyState::KEY_IDLE;
	}

	while (SDL_PollEvent(&event) != 0)
	{
		App->editor->ProcessEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			return UpdateState::Update_End;
			break;

		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED && (int)event.window.windowID == MyEngine::WindowUtils::WindowID(App->renderer->data))
				App->renderer->ResizedWindow();
			break;

		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)			mouse[0] = KeyState::KEY_DOWN;
			else if (event.button.button == SDL_BUTTON_RIGHT)	mouse[1] = KeyState::KEY_DOWN;
			else if (event.button.button == SDL_BUTTON_MIDDLE)	mouse[2] = KeyState::KEY_DOWN;
			break;

		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT)			mouse[0] = KeyState::KEY_UP;
			else if (event.button.button == SDL_BUTTON_RIGHT)	mouse[1] = KeyState::KEY_UP;
			else if (event.button.button == SDL_BUTTON_MIDDLE)	mouse[2] = KeyState::KEY_UP;
			break;

		case SDL_MOUSEWHEEL:
			mouse_wheel = event.wheel.y;
			break;

		case SDL_DROPFILE:
			char* drop_file = event.drop.file;
			LOG("Drop file detected");
			ManageDropFile(drop_file);
			SDL_free(drop_file);
			break;
		}
	}

	return UpdateState::Update_Continue;
}

UpdateState ModuleInput::PostUpdate()
{
	SDL_GetMouseState(&mouse_x, &mouse_y);


	return UpdateState::Update_Continue;
}

bool ModuleInput::CleanUp()
{
	LOGINFO("Close input system.");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

void ModuleInput::ManageDropFile(const char* file)
{
	unsigned int len = strlen(file);

	if (file[len - 4] != '.') return;
	if (file[len - 3] != 'f') return;
	if (file[len - 2] != 'b') return;
	if (file[len - 1] != 'x') return;

	LOGDEBUG("FBX file Droped!");

	App->scene->NewModel(file);
}

void ModuleInput::GetMouseMovement(float * x, float * y)
{
	int curr_x;
	int curr_y;

	SDL_GetMouseState(&curr_x, &curr_y);
	*x = ((curr_x - mouse_x) * mouse_senitivity);
	*y = ((curr_y - mouse_y) * mouse_senitivity);
}

void ModuleInput::GetMousePosition(int * x, int * y)
{
	SDL_GetMouseState(x, y);
}
