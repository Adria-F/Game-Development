#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "UI_element.h"
#include "UI_Image.h"
#include "UI_Text.h"
#include "UI_Button.h"
#include "UI_InputBox.h"
#include "UI_Window.h"
#include "j1Window.h"
#include "j1UIScene.h"
#include "UI_Slider.h"
#include "UI_Chrono.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
	pausable = false;
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());
	UI_scale = App->win->GetScale();
	UI_scale = 1 / UI_scale;

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	bool ret = true;
	
	int x, y;
	App->input->GetMousePosition(x, y);
	int scale = App->win->GetScale();
	UI_element* element = nullptr;

	//Get element to interact with
	if (draggingElement != nullptr)
		element = draggingElement;
	else
	{
		if (App->uiScene->current_menu != nullptr)
		{
			for (p2List_item<UI_element*>* item = App->uiScene->current_menu->elements.end; item; item = item->prev)
			{
				iPoint globalPos = item->data->calculateAbsolutePosition();
				if (x > globalPos.x && x < globalPos.x + item->data->section.w / scale && y > globalPos.y && y < globalPos.y + item->data->section.h / scale && element == nullptr && item->data->solid)
				{
					element = item->data;
				}
				else if (item->data->hovering)
				{
					item->data->hovering = false;
					if (item->data->callback != nullptr)
						item->data->callback->OnUIEvent(item->data, MOUSE_LEAVE);
				}
			}
		}
	}

	//Send events related to UI elements
	if (element != nullptr)
	{
		if (!element->hovering)
		{
			element->hovering = true;
			if (element->callback != nullptr)
				element->callback->OnUIEvent(element, MOUSE_ENTER);
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			if (element->callback != nullptr)
			{
				ret = element->callback->OnUIEvent(element, MOUSE_LEFT_CLICK);
			}
			if (element->dragable)
			{
				element->Start_Drag();
				draggingElement = element;
			}
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
		{
			if (element->callback != nullptr)
			{
				element->callback->OnUIEvent(element, MOUSE_LEFT_RELEASE);
			}
			if (element->dragable)
			{
				element->End_Drag();
				draggingElement = nullptr;
			}
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
		{
			if (element->callback != nullptr)
				ret = element->callback->OnUIEvent(element, MOUSE_RIGHT_CLICK);
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP)
		{
			if (element->callback != nullptr)
				element->callback->OnUIEvent(element, MOUSE_RIGHT_RELEASE);
		}
	}

	return ret;
}

bool j1Gui::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		UI_Debug = !UI_Debug;

	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate(float dt)
{
	if (App->uiScene->current_menu != nullptr)
	{
		for (p2List_item<UI_element*>* item = App->uiScene->current_menu->elements.start; item; item = item->next)
		{
			if (item->data->moving)
			{
				item->data->Mouse_Drag();
				if (item->data->parent != nullptr && item->data->parent->element_type == SLIDER)
				{
					Slider* parent = (Slider*)item->data->parent;
					parent->setProgress((parent->button->localPosition.x + parent->button->section.w / (2 / App->gui->UI_scale)) / ((parent->bar_length) * App->gui->UI_scale));
				}
				item->data->state = CLICKED;
			}

			if (item->data->parent == nullptr)
				item->data->BlitElement();
		}
	}
	if (UI_Debug)
		UIDebugDraw();
	/*for (p2List_item<inputBox*>* item = inputTexts.start; item; item = item->next) //Input Text reading
	{
	if (item->data->reading)
	{
	item->data->readInput();
	break;
	}
	}*/

	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	p2List_item<UI_element*>* item;
	item = UI_elements.start;
	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	UI_elements.clear();

	return true;
}

void j1Gui::UIDebugDraw()
{
	for (p2List_item<UI_element*>* item = App->uiScene->current_menu->elements.start; item; item = item->next)
	{
		SDL_Rect box;
		int scale = App->win->GetScale();
		box.x = item->data->calculateAbsolutePosition().x * scale;
		box.y = item->data->calculateAbsolutePosition().y * scale;
		box.w = item->data->section.w;
		box.h = item->data->section.h;
		App->render->DrawQuad(box, 255, 0, 0, 255, false, false);
	}

}
// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

Text* j1Gui::createText(char* text, int x, int y, _TTF_Font* font, SDL_Color color, j1Module* callback)
{
	Text* ret = new Text(text, x, y, font, color, callback);
	ret->solid = false;
	UI_elements.add(ret);

	return ret;
}

Image* j1Gui::createImage(int x, int y, SDL_Texture* texture, j1Module* callback)
{
	uint tex_width, tex_height;
	App->tex->GetSize(texture, tex_width, tex_height);
	Image* ret = new Image(texture, x, y, {0, 0, (int)tex_width, (int)tex_height }, callback);
	UI_elements.add(ret);

	return ret;
}

Image* j1Gui::createImageFromAtlas(int x, int y, SDL_Rect section, j1Module* callback)
{
	Image* ret = new Image(atlas, x, y, section, callback);
	UI_elements.add(ret);

	return ret;
}

Button* j1Gui::createSwitch(int x, int y, SDL_Texture* texture, SDL_Rect standbyUnactive, SDL_Rect OnMouseUnactive, SDL_Rect standbyActive, SDL_Rect OnMouseActive, j1Module* callback)
{
	SDL_Texture* usingTexture = (texture) ? texture : atlas;

	Button* ret = new Button(x, y, usingTexture, standbyUnactive, OnMouseUnactive, standbyActive, OnMouseActive, callback);
	UI_elements.add(ret);

	return ret;
}

InputBox* j1Gui::createInputBox(_TTF_Font* font, SDL_Color color, int x, int y, SDL_Texture * texture, SDL_Rect section, j1Module* callback)
{
	SDL_Texture* usingTexture = (texture) ? texture : atlas;

	InputBox* ret = new InputBox(font, color, x, y, usingTexture, section, callback);
	UI_elements.add(ret);

	return ret;
}

Window* j1Gui::createWindow(int x, int y, SDL_Texture * texture, SDL_Rect section, j1Module * callback)
{
	SDL_Texture* usingTexture = (texture) ? texture : atlas;

	Window* ret = new Window(usingTexture, x, y, section, callback);
	UI_elements.add(ret);

	return ret;
}

Slider * j1Gui::createSlider(int x, int y, SDL_Texture * texture, SDL_Rect empty, SDL_Rect full, Button* button, _TTF_Font* text_font, SDL_Color text_color, float default_progress, j1Module * callback, char* text)
{
	SDL_Texture* usingTexture = (texture) ? texture : atlas;

	Slider* ret = new Slider(x, y, usingTexture, empty, full,  default_progress, callback);
	ret->solid = false;

	if (full.w > full.h)
	{
		button->setDragable(true, false);
		button->setLimits(empty.w / (2 / UI_scale), empty.w / (2 / UI_scale), -1, -1);
	}
	else
	{
		button->setDragable(false, true);
		button->setLimits(-1, -1, empty.h / (2 / UI_scale), empty.h / (2 / UI_scale));
	}

	ret->appendChild(((empty.w * UI_scale) - 5 - button->section.w / (2 / UI_scale)) * default_progress, y, button);
	button->setOriginalPos(((empty.w * UI_scale) - 7 - button->section.w / (2 / UI_scale)) * 0.5f, y);

	ret->appendChild(x, y, createText(text, x, y, text_font, text_color));

	UI_elements.add(ret);

	return ret;
}

Button* j1Gui::createButton(int x, int y, SDL_Texture* texture, SDL_Rect standby, SDL_Rect OnMouse, SDL_Rect OnClick, j1Module* callback)
{
	SDL_Texture* usingTexture = (texture) ? texture : atlas;

	Button* ret = new Button(x, y, usingTexture, standby, OnMouse, OnClick, callback);
	UI_elements.add(ret);

	return ret;
}

Chrono* j1Gui::createChrono(int x, int y, _TTF_Font* font, SDL_Color color, j1Module* callback)
{
	Chrono* ret = new Chrono(x, y,font,color, callback);
	ret->solid = false;
	UI_elements.add(ret);

	return ret;
}