#ifndef __ModuleEntity_H__
#define __ModuleEntity_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include "j1Timer.h"
#include "PugiXml/src/pugixml.hpp"

#define gravity -625
#define DETECTION_MARGIN 350

struct SDL_Texture;

enum entity_state
{
	IDLE = 0,
	RIGHT,
	LEFT,
	JUMPING,
	FALLING,
	DEAD,
	DAMAGED
};

enum entity_type
{
	PLAYER = 0,
	CHARGER,
	BAT
};

class Entity : public j1Module
{
public:

	Entity(const char* name);
	virtual ~Entity();

	bool Entity_Update(float dt);

	bool Calculate_Path();
	virtual void standardPath()
	{}
	virtual void followPath()
	{}

	void Entity_OnCollision(Collider* c1, Collider* c2);

	virtual bool specificAnimation()
	{
		return false;
	}

	Animation* LoadAnimation(const char* animationPath, const char* animationName);
	void LoadLogic(const char* animationPath);

	bool Collision_from_bottom	(Collider* c1, Collider* c2, int margin = 1) const;
	bool Collision_from_left	(Collider* c1, Collider* c2) const;
	bool Collision_from_right	(Collider* c1, Collider* c2) const;
	bool Collision_from_top		(Collider* c1, Collider* c2) const;

private:

	void setAnimation();

public:

	iPoint position; //To draw
	fPoint virtualPosition; //To move
	int pos_relCam; //Position on screen
	fPoint v; //Current velocity

	p2DynArray<iPoint> entityPath;
	j1Timer doStandardPath;
	bool counting = false;
	bool slowerPath = false;
	
	//Keep orientation of collision
	bool colliding_bottom = false;
	bool colliding_right = false;
	bool colliding_left = false;
	bool colliding_top = false;

	bool going_right = false;
	bool going_left = false;
	bool jumping = false;
	bool going_down = false;
	bool dead = false;

	bool sound_one_time = false;

	SDL_Texture* graphics = nullptr;
	
	entity_state state;
	//Basic animations
	Animation* animation = nullptr;			Animation* idle_right = nullptr;
	Animation* idle_left = nullptr;			Animation* right = nullptr;
	Animation* left = nullptr;				Animation* jumping_left = nullptr;
	Animation* jumping_right = nullptr;		Animation* falling_left = nullptr;
	Animation* falling_right = nullptr;		Animation* death = nullptr;
	
	uint landing_fx = 0;

	Collider* collider = nullptr;
	fPoint collider_offset;
	Collider* collidingFloor = nullptr; //Last platform collision
	Collider* old_savedCol = nullptr;

	float speed;
	float jump_force;
	bool flying = false;
	float prev_dt = 0;

	entity_type type;
	float scale = 1;
	iPoint sprite_pos, collider_pos, collider_size;
};

#endif // !__ModuleEntity_H__
