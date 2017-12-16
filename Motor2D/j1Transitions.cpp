#include "j1Transitions.h"
#include "j1App.h"
#include "j1Gui.h"
#include "j1Render.h"
#include "j1Window.h"

Transitions::Transitions()
{
	pausable = false;
	alpha_value = 0;
}

Transitions::~Transitions()
{
}

bool Transitions::Update(float dt)
{
	if (doingMenuTransition)
	{
		float Dalpha = 255 / (total_time / dt);
		if (menuState == GIN)
		{
			App->gui->alpha_value -= Dalpha;
			if (App->gui->alpha_value <= 0)
			{
				App->gui->alpha_value = 0;
				menuState = GOUT;
				App->uiScene->loadMenu(newMenuID);
			}
		}
		else if (menuState == GOUT)
		{
			App->gui->alpha_value += Dalpha;
			if (App->gui->alpha_value >= 255)
			{
				App->gui->alpha_value = 255;
				menuState = UNACTIVE;
				doingMenuTransition = false;
			}
		}
	}
	
	if (doingSceneTransition)
	{
		float Dalpha = 255 / (total_time / dt);
		if (sceneState == GOUT)
		{
			alpha_value -= Dalpha;
			if (alpha_value <= 0)
			{
				alpha_value = 0;
				sceneState = UNACTIVE;
				doingSceneTransition = false;
			}
		}
		else if (sceneState == GIN)
		{
			alpha_value += Dalpha;
			if (alpha_value >= 255)
			{
				alpha_value = 255;
				sceneState = GOUT;
				App->scene->newLvl = newLvl;
				App->scene->load_lvl = true;
			}
		}
	}
	
	return true;
}

bool Transitions::PostUpdate(float dt)
{
	if (doingSceneTransition)
		App->render->DrawQuad({ 0, 0, App->win->screen_surface->w, App->win->screen_surface->h }, 0, 0, 0, alpha_value, true, false);

	return true;
}

void Transitions::menuTransition(menu_id newMenuID, float time)
{
	this->newMenuID = newMenuID;
	total_time = time * 0.5f;
	doingMenuTransition = true;
	menuState = GIN;
}

void Transitions::sceneTransition(int newLvl, float time)
{
	this->newLvl = newLvl;
	total_time = time * 0.5f;
	doingSceneTransition = true;
	sceneState = GIN;
}