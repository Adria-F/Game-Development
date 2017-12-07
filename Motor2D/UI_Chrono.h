#ifndef __UI_CHRONO__
#define __UI_CHRONO__
#include "UI_element.h"
#include "UI_Text.h"

class Chrono : public UI_element
{
public:
	Chrono()
	{}

	Chrono(int x, int y, j1Module* callback): UI_element(x, y, element_type::CHRONO, {0, 0, 0, 0}, callback, nullptr)
	{}

	~Chrono()
	{}

	void BlitElement();

public:
	uint timer = 0;
	uint time_changed = 0;
	Text* text;
};

#endif // !__UI_CHRONO__



