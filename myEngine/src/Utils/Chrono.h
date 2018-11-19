#ifndef _MYENGINE_CHRONO_H
#define _MYENGINE_CHRONO_H

#include "SDL.h"

namespace MyEngine
{
	class Chrono
	{
	public:
		Chrono() { SDL_Init(SDL_INIT_TIMER);  }
		virtual ~Chrono() {}

		void Start() 
		{ 
			end_time = 0U;
			start_time = SDL_GetTicks(); 
		}

		unsigned int Pause() { return SDL_GetTicks() - start_time; }

		unsigned int Stop()
		{
			if (end_time == 0U) end_time = SDL_GetTicks();
			return (end_time - start_time);
		}

		unsigned int start_time = 0U;
		unsigned int end_time = 0U;
	};


	static Uint64 frequency;
	class uChrono
	{
	public:
		uChrono() { SDL_Init(SDL_INIT_TIMER); frequency = SDL_GetPerformanceFrequency(); }
		virtual ~uChrono() {}

		void Start()
		{
			end_time = 0U;
			start_time = SDL_GetPerformanceCounter();
		}

		float Pause() { return (float)(SDL_GetPerformanceCounter() - start_time) * 1000.0F / (float)frequency;
		}

		float Stop()
		{
			if (end_time == 0) end_time = SDL_GetPerformanceCounter();
			return (float)(end_time - start_time) * 1000.0F / (float)frequency;
		}

		Uint64 start_time = 0U;
		Uint64 end_time = 0U;

	};
}

#endif // !_MYENGINE_CHRONO_H

