#include "j1UIScene.h"
#include "j1Scene.h"
#include "j1App.h"
#include "j1Fonts.h"
#include "UI_element.h"
#include "UI_Button.h"
#include "UI_Image.h"
#include "UI_Slider.h"
#include "UI_PlayerInfo.h"
#include "UI_FinalInfo.h"
#include "UI_ProgressBar.h"
#include "j1Input.h"
#include "j1Render.h"
#include "UI_Window.h"
#include "UI_Chrono.h"
#include "j1EntityManager.h"
#include "j1Audio.h"
#include "j1Window.h"
#include "j1Transitions.h"
#include "p2Log.h"
#include "j1Player.h"

j1UIScene::j1UIScene()
{
	name.create("UIScene");
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
	_TTF_Font* huge_texts_font = App->font->Load("fonts/TCCEB.ttf", 80);
	_TTF_Font* big_texts_font = App->font->Load("fonts/TCCEB.ttf", 55);
	_TTF_Font* mid_texts_font = App->font->Load("fonts/TCCEB.ttf", 36);
	_TTF_Font* small_texts_font = App->font->Load("fonts/TCCEB.ttf", 19);
	_TTF_Font* tiny_texts_font = App->font->Load("fonts/TCCEB.ttf", 15);
	
	SDL_Color yellow_color = { 229, 168, 61, 255 };
	SDL_Color white_color = { 255, 255, 255, 0 };
	SDL_Color grey_color = { 190, 177, 158, 191 };
	SDL_Color dark_yellow_color = { 146, 97, 45, 255 };	
	SDL_Color black_color = { 0, 0, 0, 255 };

	float music_progress = (float)App->audio->getMusicVolume() / 128;
	float fx_progress = (float)App->audio->getFxVolume() / 128;

	menu* creditsMenu = new menu(CREDITS_MENU);
	{
		UI_element* credits_img = App->gui->createImage(0, 0, App->tex->Load("gui/credits.png"), this);
		//BACK
		UI_element* back_button = App->gui->createButton(55 * App->gui->UI_scale, 615 * App->gui->UI_scale, NULL, { 849,69,133,36 }, { 849,106,133,36 }, { 849,143,133,36 }, this);
		back_button->function = BACK;

		UI_element* back_text = App->gui->createText("BACK", 200, 200, small_texts_font, white_color);
		back_text->setOutlined(true);
		back_button->appendChildAtCenter(back_text);

		creditsMenu->elements.add(credits_img);
		creditsMenu->elements.add(back_button);
		creditsMenu->elements.add(back_text);
		menus.add(creditsMenu);
	}

	menu* startMenu = new menu(START_MENU);
	{
		App->paused = true;

		//TITLE
		UI_element* title_img = App->gui->createImageFromAtlas(179 * App->gui->UI_scale, 92 * App->gui->UI_scale, { 0, 0, 664,147 }, this);

		//NEW GAME
		UI_element* new_game = App->gui->createButton(372 * App->gui->UI_scale, 341 * App->gui->UI_scale, NULL, { 0,148,281,111 }, { 281,148,281,111 }, { 562,148,281,111 }, this);
		new_game->function = NEW_GAME;

		UI_element* new_text = App->gui->createText("NEW GAME", 200, 200, big_buttons_font, yellow_color);
		new_text->setOutlined(true);
		new_game->appendChildAtCenter(new_text);

		//CONTINUE GAME
		continueButton = App->gui->createButton(372 * App->gui->UI_scale, 469 * App->gui->UI_scale, NULL, { 0,148,281,111 }, { 281,148,281,111 }, { 562,148,281,111 }, this);
		continueButton->function = CONTINUE;

		UI_element* continue_text = App->gui->createText("CONTINUE", 200, 200, big_buttons_font, grey_color);
		continue_text->setOutlined(true);
		continueButton->appendChildAtCenter(continue_text);

		//EXIT GAME
		UI_element* exit_game = App->gui->createButton(372 * App->gui->UI_scale, 595 * App->gui->UI_scale, NULL, { 0,148,281,111 }, { 281,148,281,111 }, { 562,148,281,111 }, this);
		exit_game->function = EXIT;

		UI_element* exit_text = App->gui->createText("EXIT", 200, 200, big_buttons_font, yellow_color);
		exit_text->setOutlined(true);
		exit_game->appendChildAtCenter(exit_text);

		//CREDITS
		UI_element* credits = App->gui->createButton(31 * App->gui->UI_scale, 647 * App->gui->UI_scale, NULL, { 666,0,168,66 }, { 666,67,168,66 }, { 835,0,168,66 }, this);
		credits->function = CREDITS;

		UI_element* credits_text = App->gui->createText("CREDITS", 200, 200, mid_buttons_font, yellow_color);
		credits_text->setOutlined(true);
		credits->appendChildAtCenter(credits_text);

		//SETTINGS
		UI_element* settings_start_menu = App->gui->createButton(823 * App->gui->UI_scale, 647 * App->gui->UI_scale, NULL, { 666,0,168,66 }, { 666,67,168,66 }, { 835,0,168,66 }, this);
		settings_start_menu->function = SETTINGS;

		UI_element* settings_text = App->gui->createText("SETTINGS", 200, 200, mid_buttons_font, yellow_color);
		settings_text->setOutlined(true);
		settings_start_menu->appendChildAtCenter(settings_text);

		startMenu->elements.add(title_img);
		startMenu->elements.add(new_game);
		startMenu->elements.add(new_text);
		startMenu->elements.add(continueButton);
		startMenu->elements.add(continue_text);
		startMenu->elements.add(exit_game);
		startMenu->elements.add(exit_text);
		startMenu->elements.add(credits);
		startMenu->elements.add(credits_text);
		startMenu->elements.add(settings_start_menu);
		startMenu->elements.add(settings_text);
		menus.add(startMenu);
	}

	menu* inGameMenu = new menu(INGAME_MENU);
	{
		//PAUSE BUTTON
		UI_element* pause_button = App->gui->createButton(947 * App->gui->UI_scale, 12 * App->gui->UI_scale, NULL, { 666,266,60,63 }, { 726,266,60,63 }, { 786,266,60,63 }, this);
		pause_button->function = PAUSE;

		//LIVES TXT
		UI_element* lives_txt = App->gui->createText("LIVES: ", 25 * App->gui->UI_scale, 5 * App->gui->UI_scale, mid_texts_font, white_color, this);
		lives_txt->setOutlined(true);

		//TIME TXT
		UI_element* time_txt = App->gui->createText("TIME:", 529 * App->gui->UI_scale, 5 * App->gui->UI_scale, mid_texts_font, white_color, this);
		time_txt->setOutlined(true);

		//PLAYER INFO
		UI_element* playerInfo = App->gui->createPlayerInfo(0, 0, this);

		//CHRONO
		chrono = App->gui->createStopWatch(620 * App->gui->UI_scale, -3 * App->gui->UI_scale, tiny_texts_font, white_color, this);
		chrono->setAlarm(80);
		chrono->setAlarm(120);
		chrono->setAlarm(180);

		//TIME BAR
		Image* barImg = App->gui->createImageFromAtlas(0, 0, { 38, 652, 19, 33 });
		timeBar = App->gui->createProgressBar(610 * App->gui->UI_scale, 15 * App->gui->UI_scale, NULL, { 0, 581, 280, 33 }, { 0, 614, 280, 33 }, barImg, this);
		timeBar->type = DECREASING;
		timeBar->max_value = 193.0f;
		Image* highlight1 = App->gui->createImageFromAtlas(113 * App->gui->UI_scale, 2 * App->gui->UI_scale, { 245, 652, 7, 41 });
		Image* highlight2 = App->gui->createImageFromAtlas(172 * App->gui->UI_scale, 2 * App->gui->UI_scale, { 245, 652, 7, 41 });
		Image* highlight3 = App->gui->createImageFromAtlas(262 * App->gui->UI_scale, 2 * App->gui->UI_scale, { 245, 652, 7, 41 });
		highlight1->parent = timeBar;
		highlight2->parent = timeBar;
		highlight3->parent = timeBar;
		timeBar->highlights.add(highlight1);
		timeBar->highlights.add(highlight2);
		timeBar->highlights.add(highlight3);
		timeBar->addMarker(106 * App->gui->UI_scale, 45 * App->gui->UI_scale, App->gui->createImageFromAtlas(0, 0, { 80, 658, 22, 25 }), App->gui->createImageFromAtlas(0, 0, { 105, 658, 22, 25 }));
		timeBar->addMarker(165 * App->gui->UI_scale, 45 * App->gui->UI_scale, App->gui->createImageFromAtlas(0, 0, { 130, 658, 22, 25 }), App->gui->createImageFromAtlas(0, 0, { 155, 658, 22, 25 }));
		timeBar->addMarker(254 * App->gui->UI_scale, 45 * App->gui->UI_scale, App->gui->createImageFromAtlas(0, 0, { 180, 658, 23, 25 }), App->gui->createImageFromAtlas(0, 0, { 206, 658, 23, 25 }));
		
		inGameMenu->elements.add(pause_button);
		inGameMenu->elements.add(lives_txt);
		inGameMenu->elements.add(time_txt);
		inGameMenu->elements.add(timeBar);
		inGameMenu->elements.add(chrono);
		inGameMenu->elements.add(playerInfo);
		
		menus.add(inGameMenu);

	}

	menu* pauseMenu = new menu(PAUSE_MENU);
	{
		//HOME BUTTON
		UI_element* home_button = App->gui->createButton(947 * App->gui->UI_scale, 12 * App->gui->UI_scale, NULL, { 353,506,62,64 }, { 415,506,62,64 }, { 477,506,62,64 }, this);
		home_button->function = HOME;
		
		//WINDOW
		UI_element* pause_window = App->gui->createWindow(208 * App->gui->UI_scale, 182 * App->gui->UI_scale, App->tex->Load("gui/medium_parchment.png"), { 0,0,588,404 }, this);

		//SETTING CIRCLE BUTTON
		UI_element* settings_button = App->gui->createButton(275 * App->gui->UI_scale, 414 * App->gui->UI_scale, NULL, { 876,341,119,124 }, { 876,465,119,124 }, { 876,589,119,124 }, this);
		settings_button->function = SETTINGS;
		pause_window->appendChild(67 * App->gui->UI_scale, 233 * App->gui->UI_scale, settings_button);
		
		//PLAY CIRCLE BUTTON
		UI_element* play_button = App->gui->createButton(439 * App->gui->UI_scale, 414 * App->gui->UI_scale, NULL, { 638,341,119,124 }, { 638,465,119,124 }, { 638,589,119,124 }, this);
		play_button->function = PAUSE;
		pause_window->appendChild(231 * App->gui->UI_scale, 233 * App->gui->UI_scale, play_button);

		//NEW GAME CIRCLE BUTTON
		UI_element* newGame_pauseMenu = App->gui->createButton(606 * App->gui->UI_scale, 414 * App->gui->UI_scale, NULL, { 757,341,119,124 }, { 757,465,119,124 }, { 757,589,119,124 }, this);
		newGame_pauseMenu->function = NEW_GAME;
		pause_window->appendChild(398 * App->gui->UI_scale, 233 * App->gui->UI_scale, newGame_pauseMenu);

		//LVL BAR
		Image* head_img = App->gui->createImageFromAtlas(0, -30, { 273, 450, 48, 105 });
		lvlBar = App->gui->createProgressBar(248 * App->gui->UI_scale, 310 * App->gui->UI_scale, NULL, { 0, 321, 504, 53 }, { 0, 375, 504, 53 }, head_img, this);
		lvlBar->type = INCREASING;
		lvlBar->max_value = 2080.0f; // Map width
		Image* arrow = App->gui->createImageFromAtlas(-1, 0, { 101, 450, 50, 58 });
		arrow->parent = lvlBar;
		lvlBar->highlights.add(arrow);

		pauseMenu->elements.add(home_button);
		pauseMenu->elements.add(pause_window);
		pauseMenu->elements.add(settings_button);
		pauseMenu->elements.add(newGame_pauseMenu);
		pauseMenu->elements.add(play_button);
		pauseMenu->elements.add(lvlBar);
		menus.add(pauseMenu);
	}

	menu* settingsMenu = new menu(SETTINGS_MENU);
	{
		//WINDOW
		UI_element* settings_window = App->gui->createWindow(51 * App->gui->UI_scale, 93 * App->gui->UI_scale, App->tex->Load("gui/big_parchment.png"), { 0,0,923,581 }, this);

		//SOUND TXT
		UI_element* sound_txt = App->gui->createText("Sound", 0, 0, big_texts_font, black_color, this);
		settings_window->appendChild(50 * App->gui->UI_scale, 50 * App->gui->UI_scale, sound_txt);

		//MUSIC TXT
		UI_element* music_txt = App->gui->createText("Music", 0, 0, mid_texts_font, black_color, this);
		settings_window->appendChild(117 * App->gui->UI_scale, 130 * App->gui->UI_scale, music_txt);

		//MUSIC SLIDER
		Button* music_slider_butt = App->gui->createButton(0, 0, NULL, { 852, 189, 35, 42 }, { 852, 189, 35, 42 }, { 852, 189, 35, 42 }, this);
		Slider* music_slider = App->gui->createSlider(0, 0, NULL, { 0, 259, 638, 31 }, { 0, 290, 638, 31 }, music_slider_butt, mid_texts_font, dark_yellow_color, music_progress);
		music_slider->modify = MUSIC;
		settings_window->appendChild(221 * App->gui->UI_scale, 136 * App->gui->UI_scale, music_slider);
		
		//FX TXT
		UI_element* fx_txt = App->gui->createText("Fx", 0, 0, mid_texts_font, black_color, this);
		settings_window->appendChild(164 * App->gui->UI_scale, 215 * App->gui->UI_scale, fx_txt);

		//FX SLIDER
		Button* fx_slider_butt = App->gui->createButton(0, 0, NULL, { 852, 189, 35, 42 }, { 852, 189, 35, 42 }, { 852, 189, 35, 42 }, this);
		Slider* fx_slider = App->gui->createSlider(0, 0, NULL, { 0, 259, 638, 31 }, { 0, 290, 638, 31 }, fx_slider_butt, mid_texts_font, dark_yellow_color, fx_progress);
		fx_slider->modify = FX;
		settings_window->appendChild(221 * App->gui->UI_scale, 220 * App->gui->UI_scale, fx_slider);

		//GRAPHICS TXT
		UI_element* graphics_txt = App->gui->createText("Graphics", 0, 0, big_texts_font, black_color, this);
		settings_window->appendChild(49 * App->gui->UI_scale, 294 * App->gui->UI_scale, graphics_txt);

		//FULLSCREEN TXT
		UI_element* fullscreen_txt = App->gui->createText("Fullscreen", 0, 0, mid_texts_font, black_color, this);
		settings_window->appendChild(60 * App->gui->UI_scale, 373 * App->gui->UI_scale, fullscreen_txt);

		//FULLSCREEN SWITCH
		Button* switchB = App->gui->createSwitch(0, 0, NULL, { 507, 329, 58, 39 }, { 507, 368, 58, 39 }, { 571, 329, 58, 39 }, { 571, 368, 58, 39 }, this);
		settings_window->appendChild(222 * App->gui->UI_scale, 372 * App->gui->UI_scale, switchB);

		//APPLY
		UI_element* apply_button = App->gui->createButton(439 * App->gui->UI_scale, 414 * App->gui->UI_scale, NULL, { 666,0,168,66 }, { 666,67,168,66 }, { 835,0,168,66 }, this);
		settings_window->appendChild(248 * App->gui->UI_scale, 457 * App->gui->UI_scale, apply_button);
		apply_button->function = APPLY;

		UI_element* apply_text = App->gui->createText("APPLY", 200, 200, mid_texts_font, yellow_color);
		apply_text->setOutlined(true);
		apply_button->appendChildAtCenter(apply_text);

		//CANCEL
		UI_element* cancel_button = App->gui->createButton(0, 0, NULL, { 666,0,168,66 }, { 666,67,168,66 }, { 835,0,168,66 }, this);
		settings_window->appendChild(473 * App->gui->UI_scale, 457 * App->gui->UI_scale, cancel_button);
		cancel_button->function = CANCEL;

		UI_element* cancel_text = App->gui->createText("CANCEL", 200, 200, mid_texts_font, yellow_color);
		cancel_text->setOutlined(true);
		cancel_button->appendChildAtCenter(cancel_text);

		//RESTORE
		UI_element* restore_button = App->gui->createButton(0, 0, NULL, { 849,69,133,36 }, { 849,106,133,36 }, { 849,143,133,36 }, this);
		settings_window->appendChild(729 * App->gui->UI_scale, 478 * App->gui->UI_scale, restore_button);
		restore_button->function = RESTORE;

		UI_element* restore_text = App->gui->createText("Restore default", 200, 200, small_texts_font, yellow_color);
		restore_text->setOutlined(true);
		restore_button->appendChildAtCenter(restore_text);

		settingsMenu->elements.add(settings_window);
		settingsMenu->elements.add(music_slider_butt);
		settingsMenu->elements.add(music_slider);
		settingsMenu->elements.add(fx_slider_butt);
		settingsMenu->elements.add(fx_slider);
		settingsMenu->elements.add(apply_button);
		settingsMenu->elements.add(cancel_button);
		settingsMenu->elements.add(restore_button);
		settingsMenu->elements.add(switchB);
		menus.add(settingsMenu);
	}
	
	menu* endMenu = new menu(END_MENU);
	{
		
		UI_element* lvl_end_window = App->gui->createWindow(50 * App->gui->UI_scale, 75 * App->gui->UI_scale, App->tex->Load("gui/big_parchment.png"), { 0,0,923,581 }, this);

		//SUCCESS TXT
		UI_element* succes_txt = App->gui->createText("SUCCESS!", 0, 0, huge_texts_font, white_color, this);
		succes_txt->setOutlined(true);
		lvl_end_window->appendChildAtCenter(succes_txt);
		succes_txt->localPosition.y = 20;

		//All the info from the player (trophy, score, coins)
		UI_element* finalInfo = App->gui->createFinalInfo(0, 0, this);

		//NEW GAME
		UI_element* newGame_endMenu = App->gui->createButton(0, 0, NULL, { 757,341,119,124 }, { 757,465,119,124 }, { 757,589,119,124 }, this);
		newGame_endMenu->function = NEW_GAME;
		lvl_end_window->appendChild(502 * App->gui->UI_scale, 400 * App->gui->UI_scale, newGame_endMenu);//X = 402 FOR CENTRE

		//HOME BUTTON
		UI_element* home_button2 = App->gui->createButton(0 * App->gui->UI_scale, 0 * App->gui->UI_scale, NULL, { 638,713,119,124 }, { 757,713,119,124 }, { 876,713,119,124 }, this);
		home_button2->function = HOME;
		lvl_end_window->appendChild(302 * App->gui->UI_scale, 400 * App->gui->UI_scale, home_button2);

		endMenu->elements.add(lvl_end_window);
		endMenu->elements.add(finalInfo);
		endMenu->elements.add(newGame_endMenu);
		endMenu->elements.add(home_button2);
		menus.add(endMenu);
	}
	current_menu = startMenu;

	defaultValues.fx = fx_progress;
	defaultValues.music = music_progress;
	newValues = defaultValues;
	
	return true;
}

bool j1UIScene::PreUpdate()
{
	j1Player* player = (j1Player*)App->entityManager->getPlayer();
	timeBar->enterCurrentValue(chrono->counter.ReadSec());
	lvlBar->enterCurrentValue(player->position.x);
	return true;
}

bool j1UIScene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		App->scene->load_lvl = true;
		App->scene->newLvl = 1;
	}
	else if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		App->paused = true;
		loadMenu(PAUSE_MENU);
	}
	else if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		App->paused = true;
		loadMenu(SETTINGS_MENU);
	}
	else if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
	{
		App->paused = true;
		loadMenu(END_MENU);
	}
	
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
			newValues.fullscreen = tmp->active;
		}
		switch (element->function)
		{
		case NEW_GAME:
		case RESTART:
			App->paused = false;
			App->transitions->sceneTransition(2);
			//App->scene->load_lvl = true;
			//App->scene->newLvl = 2;
			App->entityManager->player_god_mode = false;
			App->setSaveFileLoadable(false);
			break;
		case CONTINUE:
			if (App->getSaveFileLoadability())
			{
				App->paused = false;
				App->LoadGame();
			}
			break;
		case SETTINGS:
			App->transitions->menuTransition(SETTINGS_MENU, 0.3);
			break;
		case CREDITS:
			App->transitions->menuTransition(CREDITS_MENU, 0.5);
			break;
		case EXIT:
			ret = false;
			break;
		case PAUSE:
			if (!App->paused)
			{
				App->paused = true;
				App->transitions->menuTransition(PAUSE_MENU, 0.3);
			}
			else
			{
				App->paused = false;
				App->transitions->menuTransition(INGAME_MENU, 0.3);
			}
			break;
		case APPLY:
			applySettings(newValues);
			App->transitions->menuTransition(previous_menu, 0.3);
			break;
		case CANCEL:
			newValues = startValues;
			applySettings(startValues);
			App->transitions->menuTransition(previous_menu, 0.3);
			break;
		case BACK:
			App->transitions->menuTransition(previous_menu, 0.5);
			break;
		case RESTORE:
			applySettings(defaultValues);
			App->transitions->menuTransition(previous_menu, 0.3);
			break;
		case HOME:
			if (!App->scene->current_lvl->data->default_paused)
				App->SaveGame();
			else
				App->setSaveFileLoadable(false);
			App->transitions->sceneTransition(1);
			break;
		}
	}
	else if (event_type == MOUSE_LEFT_RELEASE)
	{
		if (element->parent != nullptr && element->parent->element_type == SLIDER)
		{
			Slider* tmp = (Slider*)element->parent;
			switch (tmp->modify)
			{
			case MUSIC:
				newValues.music = tmp->progress;
				break;
			case FX:
				newValues.fx = tmp->progress;
				break;
			}
		}
		if (element->state == CLICKED)
			element->state = MOUSEOVER;
	}
	else if (event_type == MOUSE_RIGHT_CLICK)
	{
	}
	else if (event_type == MOUSE_RIGHT_RELEASE)
	{
	}
	else if (event_type == TIMER_ZERO)
	{
		LOG("Clock reached zero");
	}
	else if (event_type == STOPWATCH_ALARM)
	{
		Chrono* chrono = (Chrono*)element;
		LOG("Clock alarm at: %d", chrono->time);
		j1Player* player = (j1Player*)App->entityManager->getPlayer();
		player->trophy--;
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

bool j1UIScene::Load(pugi::xml_node& data)
{
	for (p2List_item<UI_element*>* item = getMenu(INGAME_MENU)->elements.start; item; item = item->next)
	{
		if (item->data->element_type == CHRONO)
		{
			Chrono* chrono = (Chrono*)item->data;
			chrono->counter.setAt(data.attribute("chrono").as_float());
		}
	}

	return true;
}

bool j1UIScene::Save(pugi::xml_node& data) const
{
	for (p2List_item<UI_element*>* item = getMenu(INGAME_MENU)->elements.start; item; item = item->next)
	{
		if (item->data->element_type == CHRONO)
		{
			Chrono* chrono = (Chrono*)item->data;
			data.append_attribute("chrono") = chrono->counter.Read();
		}
	}

	return true;
}

bool j1UIScene::loadMenu(menu_id id)
{
	bool ret = false;

	previous_menu = current_menu->id;
	pauseChronos();
	for (p2List_item<menu*>* item = menus.start; item; item = item->next)
	{
		if (item->data->id == id)
		{
			current_menu = item->data;
			playChronos();
			ret = true;
			if (id == SETTINGS_MENU)
			{
				for (p2List_item<UI_element*>* item2 = current_menu->elements.start; item2; item2 = item2->next)
				{
					if (item2->data->element_type == SWITCH)
					{
						Button* switchB = (Button*)item2->data;
						startValues.fullscreen = switchB->active;
					}
					if (item2->data->element_type == SLIDER)
					{
						Slider* slider = (Slider*)item2->data;
						switch (slider->modify)
						{
						case MUSIC:
							startValues.music = slider->getProgress();
							break;
						case FX:
							startValues.fx = slider->getProgress();
							break;
						}
					}
				}
			}
			break;
		}
	}

	if (current_menu->id == START_MENU)
	{
		if (App->getSaveFileLoadability())
		{
			continueButton->text->setColor({ 229, 168, 61, 255 });
		}
		else
		{
			continueButton->text->setColor({ 190, 177, 158, 191 });
		}
	}

	return ret;
}

menu* j1UIScene::getMenu(menu_id id) const
{
	for (p2List_item<menu*>* item = menus.start; item; item = item->next)
	{
		if (item->data->id == id)
			return item->data;
	}
	
	LOG("Menu with given id not found");
	return nullptr;
}

void j1UIScene::applySettings(settings_values values)
{
	Uint32 flag = 0;
	if (values.fullscreen)
		flag = SDL_WINDOW_FULLSCREEN;
	SDL_SetWindowFullscreen(App->win->window, flag);

	App->audio->setMusicVolume(values.music);
	App->audio->setFxVolume(values.fx);

	for (p2List_item<UI_element*>* item = current_menu->elements.start; item; item = item->next)
	{
		if (item->data->element_type == SWITCH)
		{
			Button* switchB = (Button*)item->data;
			switchB->active = values.fullscreen;
		}
		if (item->data->element_type == SLIDER)
		{
			Slider* slider = (Slider*)item->data;
			switch (slider->modify)
			{
			case MUSIC:
				slider->setProgress(values.music);
				break;
			case FX:
				slider->setProgress(values.fx);
				break;
			}
			slider->button->localPosition.x = ((slider->section.w * App->gui->UI_scale) - 5 - slider->button->section.w / (2 / App->gui->UI_scale)) * slider->progress;
		}
	}
}

void j1UIScene::pauseChronos()
{
	for (p2List_item<UI_element*>* item = current_menu->elements.start; item; item = item->next)
	{
		if (item->data->element_type == CHRONO)
		{
			Chrono* chrono = (Chrono*)item->data;
			if (!chrono->counter.isPaused())
				chrono->counter.Pause();
		}
	}
}

void j1UIScene::playChronos()
{
	for (p2List_item<UI_element*>* item = current_menu->elements.start; item; item = item->next)
	{
		if (item->data->element_type == CHRONO)
		{
			Chrono* chrono = (Chrono*)item->data;
			if (chrono->counter.isPaused())
				chrono->counter.Play();
		}
	}
}