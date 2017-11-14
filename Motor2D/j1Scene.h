#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "p2Point.h"

struct SDL_Texture;

struct level
{
	level(int num, char* mapPath)
	{
		this->mapPath.create(mapPath);
		lvl = num;
	}

	p2SString mapPath;
	int lvl;
	int length;
	iPoint player_start_pos;
	iPoint end;
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
	p2List_item<level*>* current_lvl;
	uint complete_level_fx, win_fx;
	float end_reached = 0;
	int max_camera_pos = 0;

	bool load_lvl = false;
	int newLvl = 0;

};

#endif // __j1SCENE_H__