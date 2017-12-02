#include "j1UIScene.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "UI_element.h"
#include "UI_Button.h"

j1UIScene::j1UIScene()
{
}

j1UIScene::~j1UIScene()
{
}

bool j1UIScene::Awake()
{
	return true;
}

bool j1UIScene::Start()
{
	_TTF_Font* text_font = App->font->Load("fonts/BMYEONSUNG.ttf", 50);

	pause = App->gui->createButton(100, 100, NULL, { 0,148,281,111 }, { 281,148,281,111 }, { 562,148,281,111 }, this);
	pause->dragable = true;

	UI_element* text = App->gui->createText("NEW GAME", 200, 200, text_font, { 229, 168, 61, 255 }, this);
	text->setOutlined(true);
	pause->appendChildAtCenter(text);

	return true;
}

bool j1UIScene::PreUpdate()
{
	return true;
}

bool j1UIScene::Update(float dt)
{
	return true;
}

bool j1UIScene::PostUpdate(float dt)
{
	return true;
}

bool j1UIScene::OnUIEvent(UI_element* element, event_type event_type)
{
	if (event_type == MOUSE_ENTER)
	{
		element->state = MOUSEOVER;

	}
	else if (event_type == MOUSE_LEAVE)
	{
		element->state = STANDBY;

	}
	else if (event_type == MOUSE_LEFT_CLICK)
	{
		element->state = CLICKED;

		if (element == pause)
			App->paused = !App->paused;
	}
	else if (event_type == MOUSE_LEFT_RELEASE)
	{
		if (element->state == CLICKED)
			element->state = MOUSEOVER;
	}
	else if (event_type == MOUSE_RIGHT_CLICK)
	{
	}
	else if (event_type == MOUSE_RIGHT_RELEASE)
	{
	}

	return true;
}

bool j1UIScene::CleanUp()
{
	return true;
}