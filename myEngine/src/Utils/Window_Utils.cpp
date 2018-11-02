#include "Window_Utils.h"

#include "SDL.h"

namespace MyEngine {

	WindowData* WindowUtils::CreateWindow(const char* title, unsigned int width, unsigned int height)
	{
		WindowData* data = new WindowData; // data deleted on ModuleRenderer cleanup

		if (SDL_Init(SDL_INIT_VIDEO) != 0) return data;

		Uint32 flags = SDL_WINDOW_RESIZABLE;
		flags |= SDL_WINDOW_OPENGL;

		data->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
		data->surface = SDL_GetWindowSurface(data->window);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

		data->context = SDL_GL_CreateContext(data->window);
		data->valid = data->context && data->surface && data->window;
		
		return data;
	}

	void WindowUtils::DestroyWindow(WindowData* data)
	{
		SDL_GL_DeleteContext(data->context);
		SDL_DestroyWindow(data->window);
		SDL_Quit();
	}

	void WindowUtils::SwapWindow(const WindowData* data)
	{
		SDL_GL_SwapWindow(data->window);
	}

	void WindowUtils::WindowSize(const WindowData* data, int* width, int* height)
	{
		*(data->surface) = *SDL_GetWindowSurface(data->window); 

		SDL_GetWindowSize(data->window, width, height);
	}

	int WindowUtils::WindowID(const WindowData * data)
	{
		return SDL_GetWindowID(data->window);
	}

	void WindowUtils::Version(int & major, int & minor, int & patch)
	{
		SDL_version compiled;
		SDL_VERSION(&compiled);

		major = compiled.major;
		minor = compiled.minor;
		patch = compiled.patch;
	}
}