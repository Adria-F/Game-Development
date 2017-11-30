#ifndef __UI_INPUTBOX__
#define __UI_INPUTBOX__

#include "UI_element.h"
#include "UI_Text.h"

class InputBox : public UI_element
{
public:

	InputBox(_TTF_Font* font, SDL_Color color, int x, int y, SDL_Texture* texture, SDL_Rect section, j1Module* callback) : UI_element(x, y, element_type::INPUTTEXT, section, callback, texture)
	{
		this->text = new Text("", localPosition.x, localPosition.y, font, color, nullptr);
	}

	~InputBox();

	void readInput();
	void setOutlined(bool isOutlined);
	void BlitElement();

public:

	Text* text = nullptr;
	bool reading = false;
};

#endif // !__UI_INPUTBOX__