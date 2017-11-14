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
	virtual ~Entity();

	bool Entity_Update(float dt);

	void Entity_OnCollision(Collider* c1, Collider* c2);

	virtual bool specificAnimation()
	{
		return false;
	}

	Animation* LoadAnimation(const char* animationPath, const char* animationName);
	void LoadLogic(const char* animationPath);

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
	bool colliding_top = false;

	bool going_right = false;
	bool going_left = false;
	bool jumping = false;
	bool going_down = false;

	SDL_Texture* graphics = nullptr;
	
	entity_state state;
	//Basic animations
	Animation* animation = nullptr;
	Animation* idle_right = nullptr;
	Animation* idle_left = nullptr;
	Animation* right = nullptr;
	Animation* left = nullptr;
	Animation* jumping_left = nullptr;
	Animation* jumping_right = nullptr;
	Animation* falling_left = nullptr;
	Animation* falling_right = nullptr;
	Animation* death = nullptr;
	
	uint landing_fx = 0;

	Collider* collider = nullptr;
	fPoint collider_offset;
	Collider* collidingFloor = nullptr; //Last platform collision

	float speed;
	float jump_force;
	uint max_jump_value = 0;
	bool flying = false;

	entity_type type;
	float scale = 1;
	iPoint sprite_pos, collider_pos, collider_size;
};

#endif // !__ModuleEntity_H__
