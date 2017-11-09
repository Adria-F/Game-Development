#ifndef __ModuleEntity_H__
#define __ModuleEntity_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"
#include "PugiXml/src/pugixml.hpp"

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
	CHARGER,
	BAT
};

class Entity : public j1Module
{
public:

	Entity(const char* name);

	bool Entity_Update(float dt);

	void Entity_OnCollision(Collider* c1, Collider* c2);

	virtual bool specificAnimation()
	{
		return false;
	}

	Animation LoadAnimation(const char* animationPath, const char* animationName);

private:

	void setAnimation();

public:

	iPoint position; //To draw
	fPoint virtualPosition; //To move
	int pos_relCam; //Position on screen
	fPoint v; //Current velocity
	
	//Keep orientation of collision
	bool colliding_bottom = false;
	bool colliding_right = false;
	bool colliding_left = false;

	SDL_Texture* graphics = nullptr;
	
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

	entity_type type;
};

#endif // !__ModuleEntity_H__
