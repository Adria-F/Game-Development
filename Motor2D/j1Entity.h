#ifndef __ModuleEntity_H__
#define __ModuleEntity_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"

#define gravity -0.25

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

enum entity_type
{
	PLAYER,
	ENEMY1, //needs name
	ENEMY2 //needs name
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

	virtual bool specificAnimation()
	{
		return false;
	}

private:

	void setAnimation();

public:

	iPoint position; //To draw
	fPoint virtualPosition; //To move
	int pos_relCam; //Position on screen
	fPoint v; //Current velocity
	
	//Keep orientation of collision
	bool colliding_bottom;
	bool colliding_right;
	bool colliding_left;

	SDL_Texture* graphics = nullptr;
	SDL_Texture* graphics_god = nullptr;
	
	entity_state state;
	//Basic animations
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
	
	uint landing_fx;

	Collider* collider;
	Collider* collidingFloor; //Last platform collision

	float speed;
	float jump_force;
};

#endif // !__ModuleEntity_H__
