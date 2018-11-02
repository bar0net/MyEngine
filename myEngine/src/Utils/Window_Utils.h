#ifndef _MYENGINE_SDL_UTILS_H
#define _MYENGINE_SDL_UTILS_H

#include "LogSystem.h"
#include <string>

struct SDL_Window;
struct SDL_Surface;

namespace MyEngine {
	struct WindowData 
	{
		bool valid = false;
		SDL_Window* window = nullptr;
		SDL_Surface* surface = nullptr;
		void* context = nullptr;
	};

	class WindowUtils
	{
	public:
		static WindowData* CreateWindow(const char* title, unsigned int width, unsigned int height);
		static void DestroyWindow(WindowData* data);
		static void SwapWindow(const WindowData* data);
		static void WindowSize(const WindowData* data, int* width, int* height);
		static int WindowID(const WindowData* data);

		static void Version(int& major, int& minor, int& patch);
	};
}

#endif // !_MYENGINE_SDL_UTILS_H

