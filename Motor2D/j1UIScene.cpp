#include "j1UIScene.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "UI_element.h"
#include "UI_Button.h"
#include "j1Input.h"
#include "j1Render.h"

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

	menu* mainMenu = new menu(MAIN_MENU);
	UI_element* pause = App->gui->createButton(100, 100, NULL, { 0,148,281,111 }, { 281,148,281,111 }, { 562,148,281,111 }, this);
	pause->function = PAUSE;
	pause->dragable = true;
	UI_element* text = App->gui->createText("NEW GAME", 200, 200, text_font, { 229, 168, 61, 255 }, this);
	text->setOutlined(true);
	pause->appendChildAtCenter(text);
	mainMenu->elements.add(pause);
	menus.add(mainMenu);

	menu* testMenu = new menu(SETTINGS_MENU);
	UI_element* button = App->gui->createButton(300, 300, NULL, { 0,148,281,111 }, { 281,148,281,111 }, { 562,148,281,111 }, this);
	button->dragable = true;
	button->function = QUIT;
	UI_element* test = App->gui->createText("QUIT", 200, 200, text_font, { 229, 168, 61, 255 }, this);
	test->setOutlined(true);
	button->appendChildAtCenter(test);
	testMenu->elements.add(button);
	menus.add(testMenu);

	current_menu = mainMenu;

	return true;
}

bool j1UIScene::PreUpdate()
{
	return true;
}

bool j1UIScene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
		LoadMenu(SETTINGS_MENU);
	
	return true;
}

bool j1UIScene::PostUpdate(float dt)
{
	return true;
}

bool j1UIScene::OnUIEvent(UI_element* element, event_type event_type)
{
	bool ret = true;

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

		switch (element->function)
		{
		case NEW_GAME:
			break;
		case CONTINUE:
			break;
		case SETTINGS:
			LoadMenu(SETTINGS_MENU);
			break;
		case CREDITS:
			LoadMenu(CREDITS_MENU);
			break;
		case QUIT:
			ret = false;
			break;
		case PAUSE:
			App->paused = !App->paused;
			break;
		case RESTART:
			break;
		}
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

	return ret;
}

bool j1UIScene::CleanUp()
{
	p2List_item<menu*>* item;
	item = menus.start;
	while (item)
	{
		delete item->data;
		item = item->next;
	}
	menus.clear();
	current_menu = nullptr;

	return true;
}

bool j1UIScene::LoadMenu(menu_id id)
{
	bool ret = false;
	
	for (p2List_item<menu*>* item = menus.start; item; item = item->next)
	{
		if (item->data->id == id)
		{
			current_menu = item->data;
			ret = true;
			break;
		}
	}

	return ret;
}
