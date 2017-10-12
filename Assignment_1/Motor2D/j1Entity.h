#ifndef __ModuleEntity_H__
#define __ModuleEntity_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"

#define gravity -0.007

struct SDL_Texture;

enum entity_state
{
	IDLE,
	RIGHT,
	LEFT,
	JUMPING,
	FALLING,
	DEAD,
	DAMAGED
};

class Entity : public j1Module
{
public:

	Entity()
	{
		v.x = 0;
		v.y = 0;
	}

	bool Entity_Update();

	void Entity_OnCollision(Collider* c1, Collider* c2);

private:

	void setAnimation();

public:

	iPoint position;
	fPoint virtualPosition;
	fPoint v;
	bool colliding_bottom;
	bool colliding_right;
	bool colliding_left;
	entity_state state;
	SDL_Texture* graphics = nullptr;
	Animation* animation = nullptr;
	Animation idle_right;
	Animation idle_left;
	Animation right;
	Animation left;
	Animation jumping_left;
	Animation jumping_right;
	Animation falling_left;
	Animation falling_right;
	Animation death;
	int pos_relCam;

	Collider* collider;
	Collider* collidingFloor;

	float speed;
	float jump_force;
};

#endif // !__ModuleEntity_H__
