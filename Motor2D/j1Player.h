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
	void OnCollision(Collider* c1, Collider* c2);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

public:

	uint double_jump_fx = 0, jump_fx = 0, step_fx = 0, die_fx = 0, SSJ_transformation = 0, SSJ_off = 0, killed_fx = 0;
	float step_time = 0;
	float killed_finished = 0;
	bool sound_one_time = false;

	uint lives = 0;
	uint score = 0;

	Animation* jump_cloud = nullptr;
	iPoint cloud_offset;
	iPoint cloud_pos;

	bool coins[3];
	iPoint coins_pos[3];
	int trophy = 3; //3 - gold|2 - silver|1 - bronze|0 - none

	SDL_Texture* graphics_god = nullptr;
	Animation* SSJ_aura = nullptr;
	iPoint aura_offset;

	bool double_jump = false;
	bool won = false;
	bool loading = false;
	float end_reached = 0;	
};

#endif