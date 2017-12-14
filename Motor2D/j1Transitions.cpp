#include "j1Transitions.h"
#include "j1App.h"
#include "j1Gui.h"

Transitions::Transitions()
{
	pausable = false;
}

Transitions::~Transitions()
{
}

bool Transitions::Update(float dt)
{
	if (doingTransition)
	{
		switch (currentEffect)
		{
		case FADE:
			if (type == MENU)
			{
				float Dalpha = 255 / (total_time / dt);
				if (state == GIN)
				{
					App->gui->alpha_value -= Dalpha;
					if (App->gui->alpha_value <= 0)
					{
						App->gui->alpha_value = 0;
						state = GOUT;
						App->uiScene->loadMenu(newMenuID);
					}
				}
				else if (state == GOUT)
				{
					App->gui->alpha_value += Dalpha;
					if (App->gui->alpha_value >= 255)
					{
						App->gui->alpha_value = 255;
						state = UNACTIVE;
						doingTransition = false;
					}
				}
			}
			break;
		case DRAG:		
			break;
		}
	}
	
	return true;
}

void Transitions::menuTransition(menu_id newMenuID, transition_effect effect, float time)
{
	type = MENU;
	this->newMenuID = newMenuID;
	currentEffect = effect;
	total_time = time * 0.5f;
	doingTransition = true;
	state = GIN;
}

void Transitions::sceneTransition(int newLvl, transition_effect effect, float time)
{
	type = SCENE;
	this->newLvl = newLvl;
	currentEffect = effect;
	total_time = time * 0.5f;
	doingTransition = true;
	state = GIN;
}