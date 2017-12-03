#ifndef __UI_ELEMENT__
#define __UI_ELEMENT__

#include "j1Module.h"
#include "p2Point.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Gui.h"
#include "j1Input.h"

struct SDL_Texture;

enum element_type
{
	TEXT,
	IMAGE,
	BUTTON,
	//SLIDER,
	INPUTTEXT,
	WINDOW
};

enum element_function
{
	NONE,

	NEW_GAME,
	CONTINUE,
	SETTINGS,
	CREDITS,
	QUIT,
	PAUSE,
	RESTART
};

enum element_state
{
	STANDBY,
	MOUSEOVER,
	CLICKED
};

class UI_element
{
public:

	UI_element()
	{}

	UI_element(int x, int y, element_type type, SDL_Rect section, j1Module* callback, SDL_Texture* texture = nullptr) : localPosition({ x, y }), element_type(type), section(section), callback(callback), texture(texture)
	{}

	virtual ~UI_element()
	{
		if (texture != nullptr && texture != App->gui->GetAtlas())
		{
			App->tex->UnLoad(texture);
			texture = nullptr;
		}
	}

	virtual void setOutlined(bool isOutlined)
	{}

	iPoint calculateAbsolutePosition() const
	{
		if (parent != nullptr)
			return (localPosition + parent->calculateAbsolutePosition());
		else
			return localPosition;
	}

	virtual void appendChild(int x, int y, UI_element* child)
	{}

	virtual void appendChildAtCenter(UI_element* child)
	{}

	virtual void BlitElement()
	{}

	void Mouse_Drag()
	{
		iPoint Mouse_Movement;
		App->input->GetMousePosition(Mouse_Movement.x, Mouse_Movement.y);
		localPosition.x += (Mouse_Movement.x - Click_Pos.x);
		localPosition.y += (Mouse_Movement.y - Click_Pos.y);

		Click_Pos = Mouse_Movement;
	}

	void Start_Drag()
	{
		iPoint Mouse_Movement;
		App->input->GetMousePosition(Mouse_Movement.x, Mouse_Movement.y);
		Click_Pos = Mouse_Movement;

		moving = true;
	}

	void End_Drag()
	{
		Click_Pos = { 0,0 };
		moving = false;
	}

public:

	SDL_Texture* texture = nullptr;
	iPoint localPosition;
	SDL_Rect section;
	element_type element_type;
	element_state state = STANDBY;
	element_function function = NONE;
	j1Module* callback = nullptr;
	UI_element* parent = nullptr;
	bool hovering = false;
	bool moving = false;
	bool dragable = false;

protected:
	iPoint Click_Pos{ 0,0 };
	iPoint Displace{ 0,0 };
};

#endif // !__UI_ELEMENT__