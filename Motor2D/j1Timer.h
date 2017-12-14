#ifndef __j1TIMER_H__
#define __j1TIMER_H__

#include "p2Defs.h"

class j1Timer
{
public:

	// Constructor
	j1Timer();

	void Start();
	uint32 Read() const;
	float ReadSec() const;
	void Pause();
	void Play();
	bool isPaused() const;
	void setAt(float miliseconds);

private:
	uint32	started_at;
	uint32 paused_at;
	bool paused = false;
};

#endif //__j1TIMER_H__