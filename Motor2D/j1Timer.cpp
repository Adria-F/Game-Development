// ----------------------------------------------------
// j1Timer.cpp
// Fast timer with milisecons precision
// ----------------------------------------------------

#include "j1Timer.h"
#include "SDL\include\SDL_timer.h"

// ---------------------------------------------
j1Timer::j1Timer()
{
	Start();
}

// ---------------------------------------------
void j1Timer::Start()
{
	started_at = SDL_GetTicks();
}

// ---------------------------------------------
uint32 j1Timer::Read() const
{
	if (paused)
		return paused_at;
	else
		return SDL_GetTicks() - started_at;
}

// ---------------------------------------------
float j1Timer::ReadSec() const
{
	if (paused)
		return float(paused_at) / 1000.0f;
	else
		return float(SDL_GetTicks() - started_at) / 1000.0f;
}

void j1Timer::Pause()
{
	if (!paused)
	{
		paused_at = SDL_GetTicks() - started_at;
		paused = true;
	}
}

void j1Timer::Play()
{
	if (paused)
	{
		started_at = SDL_GetTicks() - paused_at;
		paused = false;
	}
}

bool j1Timer::isPaused() const
{
	return paused;
}