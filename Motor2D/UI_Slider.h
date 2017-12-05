#ifndef __UI_SLIDER__
#define __UI_SLIDER__

#include "UI_element.h"
#include "UI_Text.h"

class Button;

class Slider : public UI_element
{
public:

	Slider()
	{}

	Slider(int x, int y, SDL_Texture* texture, SDL_Rect empty, SDL_Rect full, int default_progress, j1Module* callback) : UI_element(x, y, SLIDER, empty, callback, texture),
		full(full),
		bar_length(full.w),
		progress(default_progress)
	{	}

	~Slider()
	{}

	void appendChild(int x, int y, UI_element* child);
	float getProgress() const;
	void BlitElement();

public:

	SDL_Rect full;
	Button* button = nullptr;
	Text* progress_num = nullptr;
	float progress;
	int bar_length;
};

#endif // !__UI_SLIDER__