#include "j1UIScene.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "UI_element.h"
#include "UI_Button.h"
#include "UI_Image.h"
#include "j1Input.h"
#include "j1Render.h"

j1UIScene::j1UIScene()
{
	pausable = false;
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
	_TTF_Font* big_buttons_font = App->font->Load("fonts/BMYEONSUNG.ttf", 50);
	SDL_Color big_buttons_color = { 229, 168, 61, 255 };
	_TTF_Font* mid_buttons_font = App->font->Load("fonts/BMYEONSUNG.ttf", 30);
	SDL_Color mid_buttons_color = { 229, 168, 61, 255 };

	menu* mainMenu = new menu(INGAME_MENU);
	{
		//TITLE
		UI_element* title_img = App->gui->createImageFromAtlas(179 * App->gui->UI_scale, 92 * App->gui->UI_scale, { 0, 0, 664,147 }, this);

		//NEW GAME
		UI_element* new_game = App->gui->createButton(372 * App->gui->UI_scale, 341 * App->gui->UI_scale, NULL, { 0,148,281,111 }, { 281,148,281,111 }, { 562,148,281,111 }, this);
		new_game->function = PAUSE;
		new_game->dragable = true;

		UI_element* new_text = App->gui->createText("NEW GAME", 200, 200, big_buttons_font, big_buttons_color);
		new_text->setOutlined(true);
		new_game->appendChildAtCenter(new_text);

		//CONTINUE GAME
		UI_element* continue_game = App->gui->createButton(372 * App->gui->UI_scale, 469 * App->gui->UI_scale, NULL, { 0,148,281,111 }, { 281,148,281,111 }, { 562,148,281,111 }, this);
		continue_game->dragable = true;

		UI_element* continue_text = App->gui->createText("CONTINUE", 200, 200, big_buttons_font, big_buttons_color);
		continue_text->setOutlined(true);
		continue_game->appendChildAtCenter(continue_text);

		//EXIT GAME
		UI_element* exit_game = App->gui->createButton(372 * App->gui->UI_scale, 595 * App->gui->UI_scale, NULL, { 0,148,281,111 }, { 281,148,281,111 }, { 562,148,281,111 }, this);
		exit_game->dragable = true;

		UI_element* exit_text = App->gui->createText("EXIT", 200, 200, big_buttons_font, big_buttons_color);
		exit_text->setOutlined(true);
		exit_game->appendChildAtCenter(exit_text);

		//CREDITS
		UI_element* credits = App->gui->createButton(31 * App->gui->UI_scale, 647 * App->gui->UI_scale, NULL, { 666,0,168,66 }, { 666,67,168,66 }, { 835,0,168,66 }, this);
		credits->dragable = true;

		UI_element* credits_text = App->gui->createText("CREDITS", 200, 200, mid_buttons_font, mid_buttons_color);
		credits_text->setOutlined(true);
		credits->appendChildAtCenter(credits_text);

		//SETTINGS
		UI_element* settings = App->gui->createButton(823 * App->gui->UI_scale, 647 * App->gui->UI_scale, NULL, { 666,0,168,66 }, { 666,67,168,66 }, { 835,0,168,66 }, this);
		settings->dragable = true;

		UI_element* settings_text = App->gui->createText("SETTINGS", 200, 200, mid_buttons_font, mid_buttons_color);
		settings_text->setOutlined(true);
		settings->appendChildAtCenter(settings_text);


		/*new_text = App->gui->createText("IN GAME", 0, 0, big_buttons_font, big_buttons_color);
		new_text->setOutlined(true);*/

		mainMenu->elements.add(title_img);
		mainMenu->elements.add(new_game);
		mainMenu->elements.add(new_text);
		mainMenu->elements.add(continue_game);
		mainMenu->elements.add(continue_text);
		mainMenu->elements.add(exit_game);
		mainMenu->elements.add(exit_text);
		mainMenu->elements.add(credits);
		mainMenu->elements.add(credits_text);
		mainMenu->elements.add(settings);
		mainMenu->elements.add(settings_text);
		menus.add(mainMenu);
	}

	menu* pauseMenu = new menu(PAUSE_MENU);
	{
		//PAUSE BUTTON
		UI_element* pause_button = App->gui->createButton(947 * App->gui->UI_scale, 12 * App->gui->UI_scale, NULL, { 665,266,61,65 }, { 725,266,61,65 }, { 785,148,61,65 }, this);
		pause_button->dragable = true;
		pause_button->function = PAUSE;
		
		//WINDOW
		SDL_Texture* mid_window_tex = App->tex->Load("gui/medium_parchment.png");
		UI_element* pause_window = App->gui->createImage(208 * App->gui->UI_scale, 182 * App->gui->UI_scale, mid_window_tex, this);
		pause_window->dragable = true;

		//SETTING CIRCLE BUTTON
		UI_element* settings_button = App->gui->createButton(275 * App->gui->UI_scale, 414 * App->gui->UI_scale, NULL, { 876,341,120,123 }, { 876,465,120,123 }, { 876,589,120,123 }, this);
		settings_button->dragable = true;
		pause_window->appendChild(275 * App->gui->UI_scale, 414 * App->gui->UI_scale, settings_button);
		
		//PLAY CIRCLE BUTTON
		UI_element* play_button = App->gui->createButton(439 * App->gui->UI_scale, 414 * App->gui->UI_scale, NULL, { 638,341,120,123 }, { 638,465,120,123 }, { 638,589,120,123 }, this);
		play_button->dragable = true;
		pause_window->appendChild(439 * App->gui->UI_scale, 414 * App->gui->UI_scale, play_button);
		
		//RESTART CIRCLE BUTTON
		UI_element* restart_button = App->gui->createButton(606 * App->gui->UI_scale, 414 * App->gui->UI_scale, NULL, { 757,341,120,123 }, { 757,465,120,123 }, { 757,589,120,123 }, this);
		restart_button->dragable = true;
		pause_window->appendChild(606 * App->gui->UI_scale, 414 * App->gui->UI_scale, restart_button);
		
		//SLIDER
		UI_element* slider = App->gui->createImageFromAtlas(248 * App->gui->UI_scale, 310 * App->gui->UI_scale, { 0, 321, 504, 53 }, this);
		slider->dragable = true;
		
		//test = App->gui->createText("PAUSE MENU", 0, 0, big_buttons_font, big_buttons_color);
		//test->setOutlined(true);

		pauseMenu->elements.add(pause_button);
		pauseMenu->elements.add(pause_window);
		pauseMenu->elements.add(settings_button);
		pauseMenu->elements.add(restart_button);
		pauseMenu->elements.add(play_button);
		pauseMenu->elements.add(slider);
		menus.add(pauseMenu);
	}

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
		LoadMenu(PAUSE_MENU);
	
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
			if (App->paused)
			{ 
				App->paused = false;
				LoadMenu(INGAME_MENU);
			}
			else
			{
				App->paused = true;
				LoadMenu(PAUSE_MENU);
			}
			
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
