#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "j1Module.h"
#include "j1Entity.h"
#include "Animation.h"
#include "p2Point.h"
#include "p2DynArray.h"

struct SDL_Texture;
struct Collider;

class j1Player : public Entity
{
public:
	j1Player();
	~j1Player();

	bool Start();
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

public:

	bool jumping = false;
	bool falling = false;
	bool god_mode = false;

	uint double_jump_fx, jump_fx, step_fx, die_fx, SSJ_transformation, SSJ_off;
	float step_time = 0;

	Animation jump_cloud;
	iPoint cloud_offset;
	iPoint cloud_pos;

	SDL_Texture* graphics_god = nullptr;
	Animation SSJ_aura;
	iPoint aura_offset;

	bool double_jump = false;
	bool won = false;

	p2DynArray<iPoint> path;
	SDL_Texture* path_marker;
};

#endif