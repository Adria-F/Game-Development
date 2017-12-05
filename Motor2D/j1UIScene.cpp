#include "j1UIScene.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "UI_element.h"
#include "UI_Button.h"
#include "UI_Image.h"
#include "UI_Slider.h"
#include "j1Input.h"
#include "j1Render.h"
#include "UI_Window.h"

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
	_TTF_Font* mid_buttons_font = App->font->Load("fonts/BMYEONSUNG.ttf", 30);
	SDL_Color yellow_color = { 229, 168, 61, 255 };
	SDL_Color dark_yellow_color = { 146, 97, 45, 255 };
	_TTF_Font* big_texts_font = App->font->Load("fonts/TCCEB.ttf", 55);
	SDL_Color black_color = { 0, 0, 0, 255 };
	_TTF_Font* mid_texts_font = App->font->Load("fonts/TCCEB.ttf", 36);
	_TTF_Font* small_texts_font = App->font->Load("fonts/TCCEB.ttf", 19);
	SDL_Texture* big_window_tex = App->tex->Load("gui/big_parchment.png");
	SDL_Texture* mid_window_tex = App->tex->Load("gui/medium_parchment.png");

	menu* mainMenu = new menu(MAIN_MENU);
	{
		//TITLE
		UI_element* title_img = App->gui->createImageFromAtlas(179 * App->gui->UI_scale, 92 * App->gui->UI_scale, { 0, 0, 664,147 }, this);

		//NEW GAME
		UI_element* new_game = App->gui->createButton(372 * App->gui->UI_scale, 341 * App->gui->UI_scale, NULL, { 0,148,281,111 }, { 281,148,281,111 }, { 562,148,281,111 }, this);
		new_game->dragable = true;

		UI_element* new_text = App->gui->createText("NEW GAME", 200, 200, big_buttons_font, yellow_color);
		new_text->setOutlined(true);
		new_game->appendChildAtCenter(new_text);

		//CONTINUE GAME
		UI_element* continue_game = App->gui->createButton(372 * App->gui->UI_scale, 469 * App->gui->UI_scale, NULL, { 0,148,281,111 }, { 281,148,281,111 }, { 562,148,281,111 }, this);
		continue_game->setDragable(true, true);

		UI_element* continue_text = App->gui->createText("CONTINUE", 200, 200, big_buttons_font, yellow_color);
		continue_text->setOutlined(true);
		continue_game->appendChildAtCenter(continue_text);

		//EXIT GAME
		UI_element* exit_game = App->gui->createButton(372 * App->gui->UI_scale, 595 * App->gui->UI_scale, NULL, { 0,148,281,111 }, { 281,148,281,111 }, { 562,148,281,111 }, this);
		exit_game->setDragable(true, true);

		UI_element* exit_text = App->gui->createText("EXIT", 200, 200, big_buttons_font, yellow_color);
		exit_text->setOutlined(true);
		exit_game->appendChildAtCenter(exit_text);

		//CREDITS
		UI_element* credits = App->gui->createButton(31 * App->gui->UI_scale, 647 * App->gui->UI_scale, NULL, { 666,0,168,66 }, { 666,67,168,66 }, { 835,0,168,66 }, this);
		credits->setDragable(true, true);

		UI_element* credits_text = App->gui->createText("CREDITS", 200, 200, mid_buttons_font, yellow_color);
		credits_text->setOutlined(true);
		credits->appendChildAtCenter(credits_text);

		//SETTINGS
		UI_element* settings = App->gui->createButton(823 * App->gui->UI_scale, 647 * App->gui->UI_scale, NULL, { 666,0,168,66 }, { 666,67,168,66 }, { 835,0,168,66 }, this);
		settings->setDragable(true, true);

		UI_element* settings_text = App->gui->createText("SETTINGS", 200, 200, mid_buttons_font, yellow_color);
		settings_text->setOutlined(true);
		settings->appendChildAtCenter(settings_text);

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
		UI_element* pause_button = App->gui->createButton(947 * App->gui->UI_scale, 12 * App->gui->UI_scale, NULL, { 666,266,60,63 }, { 726,266,60,63 }, { 786,266,60,63 }, this);
		pause_button->setDragable(true, true);
		pause_button->function = PAUSE;
		
		//WINDOW
		UI_element* pause_window = App->gui->createWindow(208 * App->gui->UI_scale, 182 * App->gui->UI_scale, mid_window_tex, { 0,0,588,404 }, this);
		pause_window->setDragable(true, true);

		//SETTING CIRCLE BUTTON
		UI_element* settings_button = App->gui->createButton(275 * App->gui->UI_scale, 414 * App->gui->UI_scale, NULL, { 876,341,119,124 }, { 876,465,119,124 }, { 876,589,119,124 }, this);
		settings_button->setDragable(true, true);
		pause_window->appendChild(67 * App->gui->UI_scale, 233 * App->gui->UI_scale, settings_button);
		
		//PLAY CIRCLE BUTTON
		UI_element* play_button = App->gui->createButton(439 * App->gui->UI_scale, 414 * App->gui->UI_scale, NULL, { 638,341,119,124 }, { 638,465,119,124 }, { 638,589,119,124 }, this);
		play_button->setDragable(true, true);
		pause_window->appendChild(231 * App->gui->UI_scale, 233 * App->gui->UI_scale, play_button);
		
		//RESTART CIRCLE BUTTON
		UI_element* restart_button = App->gui->createButton(606 * App->gui->UI_scale, 414 * App->gui->UI_scale, NULL, { 757,341,119,124 }, { 757,465,119,124 }, { 757,589,119,124 }, this);
		restart_button->setDragable(true, true);
		pause_window->appendChild(398 * App->gui->UI_scale, 233 * App->gui->UI_scale, restart_button);
		
		//TEMP SLIDER (only img)
		UI_element* slider = App->gui->createImageFromAtlas(248 * App->gui->UI_scale, 310 * App->gui->UI_scale, { 0, 321, 504, 53 }, this);
		slider->setDragable(true, true);
		pause_window->appendChild(40 * App->gui->UI_scale, 129 * App->gui->UI_scale, slider);

		pauseMenu->elements.add(pause_button);
		pauseMenu->elements.add(pause_window);
		pauseMenu->elements.add(settings_button);
		pauseMenu->elements.add(restart_button);
		pauseMenu->elements.add(play_button);
		pauseMenu->elements.add(slider);
		menus.add(pauseMenu);
	}
	menu* settingsMenu = new menu(SETTINGS_MENU);
	{
		//WINDOW
		UI_element* settings_window = App->gui->createWindow(51 * App->gui->UI_scale, 93 * App->gui->UI_scale, big_window_tex, { 0,0,923,581 }, this);
		settings_window->setDragable(true, true);

		//SOUND TXT
		UI_element* sound_txt = App->gui->createText("Sound", 0, 0, big_texts_font, black_color, this);
		settings_window->appendChild(50 * App->gui->UI_scale, 50 * App->gui->UI_scale, sound_txt);

		//MUSIC TXT
		UI_element* music_txt = App->gui->createText("Music", 0, 0, mid_texts_font, black_color, this);
		settings_window->appendChild(117 * App->gui->UI_scale, 130 * App->gui->UI_scale, music_txt);

		//MUSIC SLIDER
		Button* music_slider_butt = App->gui->createButton(0, 0, NULL, { 852, 189, 35, 42 }, { 852, 189, 35, 42 }, { 852, 189, 35, 42 }, this);
		UI_element* music_slider = App->gui->createSlider(0, 0, NULL, { 0, 259, 638, 31 }, { 0, 290, 638, 31 }, music_slider_butt, mid_texts_font, dark_yellow_color, 0, 168, 50);
		settings_window->appendChild(221 * App->gui->UI_scale, 136 * App->gui->UI_scale, music_slider);
		
		//FX TXT
		UI_element* fx_txt = App->gui->createText("Fx", 0, 0, mid_texts_font, black_color, this);
		settings_window->appendChild(164 * App->gui->UI_scale, 215 * App->gui->UI_scale, fx_txt);

		//FX SLIDER
		Button* fx_slider_butt = App->gui->createButton(0, 0, NULL, { 852, 189, 35, 42 }, { 852, 189, 35, 42 }, { 852, 189, 35, 42 }, this);
		UI_element* fx_slider = App->gui->createSlider(0, 0, NULL, { 0, 259, 638, 31 }, { 0, 290, 638, 31 }, fx_slider_butt, mid_texts_font, dark_yellow_color, 0, 168, 50);
		settings_window->appendChild(221 * App->gui->UI_scale, 220 * App->gui->UI_scale, fx_slider);

		//GRAPHICS TXT
		UI_element* graphics_txt = App->gui->createText("Graphics", 0, 0, big_texts_font, black_color, this);
		settings_window->appendChild(49 * App->gui->UI_scale, 294 * App->gui->UI_scale, graphics_txt);

		//FULLSCREEN TXT
		UI_element* fullscreen_txt = App->gui->createText("Fullscreen", 0, 0, mid_texts_font, black_color, this);
		settings_window->appendChild(60 * App->gui->UI_scale, 373 * App->gui->UI_scale, fullscreen_txt);

		//FULLSCREEN SWITCH
		Button* switchB = App->gui->createSwitch(0, 0, NULL, { 507, 329, 58, 39 }, { 507, 368, 58, 39 }, { 571, 329, 58, 39 }, { 571, 368, 58, 39 }, this);
		switchB->setDragable(true, true);
		settings_window->appendChild(222 * App->gui->UI_scale, 372 * App->gui->UI_scale, switchB);

		//APPLY
		UI_element* apply_button = App->gui->createButton(439 * App->gui->UI_scale, 414 * App->gui->UI_scale, NULL, { 666,0,168,66 }, { 666,67,168,66 }, { 835,0,168,66 }, this);
		settings_window->appendChild(248 * App->gui->UI_scale, 457 * App->gui->UI_scale, apply_button);
		apply_button->setDragable(true, true);

		UI_element* apply_text = App->gui->createText("APPLY", 200, 200, mid_texts_font, yellow_color);
		apply_text->setOutlined(true);
		apply_button->appendChildAtCenter(apply_text);

		//CANCEL
		UI_element* cancel_button = App->gui->createButton(0, 0, NULL, { 666,0,168,66 }, { 666,67,168,66 }, { 835,0,168,66 }, this);
		settings_window->appendChild(473 * App->gui->UI_scale, 457 * App->gui->UI_scale, cancel_button);
		cancel_button->setDragable(true, true);

		UI_element* cancel_text = App->gui->createText("CANCEL", 200, 200, mid_texts_font, yellow_color);
		cancel_text->setOutlined(true);
		cancel_button->appendChildAtCenter(cancel_text);

		//RESTORE
		UI_element* restore_button = App->gui->createButton(0, 0, NULL, { 849,69,133,36 }, { 849,106,133,36 }, { 849,143,133,36 }, this);
		settings_window->appendChild(729 * App->gui->UI_scale, 478 * App->gui->UI_scale, restore_button);
		restore_button->setDragable(true, true);

		UI_element* restore_text = App->gui->createText("Restore default", 200, 200, small_texts_font, yellow_color);
		restore_text->setOutlined(true);
		restore_button->appendChildAtCenter(restore_text);

		settingsMenu->elements.add(settings_window);
		settingsMenu->elements.add(music_slider_butt);
		settingsMenu->elements.add(fx_slider_butt);
		settingsMenu->elements.add(apply_button);
		settingsMenu->elements.add(cancel_button);
		settingsMenu->elements.add(restore_button);
		settingsMenu->elements.add(switchB);
		menus.add(settingsMenu);
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
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		LoadMenu(MAIN_MENU);
	else if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		LoadMenu(PAUSE_MENU);
	else if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
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

		if (element->element_type == SWITCH)
		{
			Button* tmp = (Button*)element;
			tmp->active = !tmp->active;
		}

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
