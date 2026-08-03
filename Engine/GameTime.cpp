#include "pch.h"
#include "GameTime.h"

#include <SDL3/SDL.h>

namespace nu
{
	Time::Time()
	{
		Reset();
	}

	void Time::Reset()
	{
		a_startTicks = SDL_GetTicksNS();
		a_frameTicks = SDL_GetTicksNS();
		a_currentTicks = SDL_GetTicksNS();
	}

	void Time::Tick()
	{
		a_currentTicks = SDL_GetTicksNS(); // get current ticks
		a_deltaTicks = a_currentTicks - a_frameTicks; // delta = current ticks - prev ticks
		a_frameTicks = a_currentTicks; // set prev ticks
	}
}


