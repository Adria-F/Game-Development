#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "j1PathFinding.h"
#include "j1EntityManager.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
	
	// Add all levels to the list
	level* lvl1 = new level(1, "platformer.tmx");
	level* lvl2 = new level(2, "platformer2.tmx");

	levels.add(lvl1);
	levels.add(lvl2);

	current_lvl = levels.start;

}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	App->map->Load(levels.start->data->mapPath.GetString(), current_lvl->data->length); //hello2.tmx
	//Background music
	App->audio->PlayMusic("audio/music/bg_music.ogg");
	complete_level_fx = App->audio->LoadFx("audio/fx/level_complete.wav");
	win_fx = App->audio->LoadFx("audio/fx/win.wav");

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		load_lvl = true;
		newLvl = 1;
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		if (current_lvl->data->lvl == 2)
		{
			load_lvl = true;
			newLvl = 2;
		}
		else
		{
			load_lvl = true;
			newLvl = 1;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();

	if(App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		if (App->player->god_mode)
		{
			App->player->god_mode = false;
			App->audio->PlayFx(App->player->SSJ_off, 0);
		}
		else
		{
			App->player->god_mode = true;
			App->audio->PlayFx(App->player->SSJ_transformation, 0);
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
	{
		App->cap_frames = !App->cap_frames;
	}

	if ((App->input->GetKey(SDL_SCANCODE_F12) == KEY_DOWN))
	{
		
	}

	// Move camera with player -----------------------
	uint win_width, win_height;
	App->win->GetWindowSize(win_width, win_height);
	max_camera_pos = current_lvl->data->length + (win_width);
	max_camera_pos *= -1;
	if ((App->player->pos_relCam > (win_width / App->win->GetScale() / 2) ) && (App->render->virtualCamPos > max_camera_pos))
	{
		App->render->virtualCamPos -= App->player->speed * 2;
	}
	// ------------------------------------------------

	// Win condition
	if (App->player->position.x > current_lvl->data->length - 32 - App->player->animation->GetCurrentFrame(dt).w)
	{
		if (end_reached == 0)
		{
			App->player->won = true;
			end_reached = SDL_GetTicks();
			if (current_lvl == levels.end)
			{
				App->audio->PlayFx(win_fx, 0);
			}
			else
			{
				App->audio->PlayFx(complete_level_fx, 0);
			}
		}
	}
	if  (App->player->won && ((current_lvl == levels.end && SDL_GetTicks() > end_reached + 5000) || (current_lvl != levels.end && SDL_GetTicks() > end_reached + 500)))
	{
		end_reached = 0;
		App->player->won = false;
		load_lvl = true;
	}

	// Parallax
	p2List_item<ImageLayer*>* image = nullptr; // Parallax when player moves
	for (image = App->map->data.image_layers.start; image; image = image->next)
	{
		if (image->data->speed > 0)
		{
			if (image->data->constant_movement)
			{
				image->data->position.x -= image->data->speed;
			}
			else if (App->player->v.x > 0 && (App->player->pos_relCam > (win_width / App->win->GetScale() / 2)) && (App->render->virtualCamPos > max_camera_pos))
			{
				image->data->position.x -= image->data->speed;
			}
		}
	}

	App->map->Draw();

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate(float dt)
{
	bool ret = true;

	if (load_lvl)
	{
		LoadLvl(newLvl);
		load_lvl = false;
		newLvl = 0;
	}

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void j1Scene::LoadLvl(int num)
{
	if (num == 0)
	{
		current_lvl = current_lvl->next;
		if (current_lvl == nullptr)
		{
			current_lvl = levels.start;
		}
	}
	else
	{
		p2List_item<level*>* lvl = levels.start;
		for (int i = 1; i < num; i++)
		{
			lvl = lvl->next;
			if (lvl == nullptr)
			{
				LOG("There is no level %d to load", num);
				break;
			}
		}
		current_lvl = lvl;
	}

	if (current_lvl != nullptr)
	{
		App->map->Load(current_lvl->data->mapPath.GetString(), current_lvl->data->length);
		
		// Restart player data
		App->player->collider->to_delete = true;
		App->player->collider = nullptr; //Has to be null in order to be created
		App->player->Start();
	}
}