#ifndef __UI_CHRONO__
#define __UI_CHRONO__
#include "UI_element.h"

class Chrono : public UI_element
{
public:
	Chrono()
	{}

	Chrono(int x, int y)
	{}

	~Chrono()
	{}

	void BlitElement();
	void CalculateTime();

public:
	uint timer = 0;
	uint time_changed = 0;
};

#endif // !__UI_CHRONO__



