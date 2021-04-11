#include "timer.h"

#include <SDL.h>

Timer::Timer() : countStart(0), countEnd(0), deltaTime(0.0f) {}

float Timer::update()
{
	// get the current cycle count
	countEnd = SDL_GetPerformanceCounter();

	// calculate the time since last update in milliseconds
	deltaTime = 1000.0f * (countEnd - countStart) / SDL_GetPerformanceFrequency();

	countStart = countEnd;

	return deltaTime;
}

float Timer::getDeltaTime() const
{
	return deltaTime;
}