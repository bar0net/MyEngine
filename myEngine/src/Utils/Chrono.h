#ifndef _MYENGINE_CHRONO_H
#define _MYENGINE_CHRONO_H

#include "SDL.h"


namespace MyEngine 
{

	class Chrono
	{
	public:
		Chrono() : enabled(false), timer(0) {};
		virtual ~Chrono() {};

		void Start() 
		{ 
			enabled = true;
			timer = SDL_GetTicks();
		};

		unsigned int Read() { return (SDL_GetTicks() - timer);  };
	
		unsigned int Stop()
		{
			if (enabled)
			{
				timer = SDL_GetTicks() - timer;
				enabled = false;
			}
			return (unsigned int)timer;
		}


	private:
		bool enabled;
		unsigned long int timer;
	};


	class uChrono
	{
	public:
		uChrono() : enabled(false), counter(0)
		{
			frequency = SDL_GetPerformanceFrequency();
		};
		virtual ~uChrono() {};

		void Start()
		{
			enabled = true;
			counter = SDL_GetPerformanceCounter();
		};

		float Read() { return (SDL_GetPerformanceCounter() - counter)/(float)frequency; };

		float Stop()
		{
			if (enabled)
			{
				counter = (SDL_GetPerformanceCounter() - counter);
				enabled = false;
			}
			return (float)(1000*counter/(float)frequency);
		}


	private:
		bool enabled;
		Uint64 counter;
		Uint64 frequency;
	};
}
#endif // !_MYENGINE_CHRONO_H
