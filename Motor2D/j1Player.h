#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "j1Module.h"
#include "j1Entity.h"
#include "Animation.h"
#include "p2Point.h"

struct SDL_Texture;
struct Collider;

class j1Player : public Entity
{
public:
	j1Player();
	~j1Player();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

public:

	bool jumping;
	bool falling;
	bool god_mode = false;

	fPoint collider_move;

	uint double_jump_fx, jump_fx, step_fx, die_fx, SSJ_transformation, SSJ_off;
	float step_time = 0;

	Animation jump_cloud;
	iPoint cloud_offset;
	iPoint cloud_pos;

	Animation SSJ_aura;
	iPoint aura_offset;

	bool double_jump;
	bool won;
};

#endif