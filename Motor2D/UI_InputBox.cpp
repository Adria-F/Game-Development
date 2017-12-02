#include "UI_InputBox.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "SDL\include\SDL_events.h"

InputBox::~InputBox()
{
	if (text != nullptr)
	{
		delete text;
		text = nullptr;
	}
}

void InputBox::readInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_KEYDOWN)
		{
			if (e.key.keysym.sym == SDLK_BACKSPACE && text->getLength() > 0)
			{
				//text->text.Cut(0, text->text.Length() - 1);
			}
		}
		else if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			int x, y;
			App->input->GetMousePosition(x, y);
			if (!(x > localPosition.x && x < localPosition.x + section.w && y > localPosition.y && y <localPosition.y + section.h)) //This event should be able to be detected in other modules
			{
				reading = false;
				SDL_StopTextInput();
			}
		}
		else if (e.type == SDL_TEXTINPUT)
		{
			p2SString newText = text->getText();
			newText += e.text.text;
			text->setText(newText);
		}
	}
}

void InputBox::setOutlined(bool isOutlined)
{
	if (text != nullptr)
		text->setOutlined(isOutlined);
}

void InputBox::BlitElement()
{
	iPoint globalPos = calculateAbsolutePosition();
	App->render->Blit(texture, globalPos.x, globalPos.y, &section, false, App->gui->UI_scale);

	if (text != nullptr)
	{		
		text->BlitElement();
	}
}
