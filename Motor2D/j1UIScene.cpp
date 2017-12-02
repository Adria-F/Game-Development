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
	pause = App->gui->createButton(100, 100, NULL, { 642,169,229,69 }, { 0,113,229,69 }, { 411,169,229,69 }, this);
	pause->dragable = true;
	App->gui->createText("Hello World", 200, 200, App->font->Load("fonts/open_sans/OpenSans-Regular.ttf", 25), { 255, 0, 0, 255 })->dragable = true;
	App->gui->createButton(200, 100, NULL, { 642,169,229,69 }, { 0,113,229,69 }, { 411,169,229,69 }, this)->dragable = true;

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