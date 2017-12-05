#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "p2Point.h"
#include "j1UIScene.h"
#include "SDL\include\SDL_rect.h"

struct SDL_Texture;

struct level
{
	level(int num, char* mapPath, menu_id default_menu)
	{
		this->mapPath.create(mapPath);
		lvl = num;
		this->default_menu = default_menu;
	}

	p2SString mapPath;
	int lvl;
	int length;
	iPoint player_start_pos;
	SDL_Rect end_rect;
	menu_id default_menu;
};

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	// If num is 0 it will load the next level or the first one if there is no next level
	void LoadLvl(int num = 0);

public:

	p2List<level*> levels;
	p2List_item<level*>* current_lvl = nullptr;
	uint complete_level_fx = 0, win_fx = 0;
	int max_camera_pos = 0;

	bool load_lvl = false;
	int newLvl = 0;
	bool respawn_enemies = true;

};

#endif // __j1SCENE_H__