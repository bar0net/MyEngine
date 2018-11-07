#include "ModuleInput.h"

#include "SDL.h"

#include "ModuleEditor.h"
#include "ModuleRenderer.h"
#include "ModuleScene.h"

#include "Application.h"
#include "Utils/Window_Utils.h"

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
	SDL_EventState(SDL_DROPFILE, SDL_ENABLE);

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
		
		if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED && (int)event.window.windowID == MyEngine::WindowUtils::WindowID(App->renderer->data))
				App->renderer->ResizedWindow();		

		if (event.type == SDL_DROPFILE)
		{
			char* drop_file = event.drop.file;
			LOG("Drop file detected");
			ManageDropFile(drop_file);
			SDL_free(drop_file);
		}
	}

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