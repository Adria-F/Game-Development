#ifndef __ModuleEntity_H__
#define __ModuleEntity_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Collision.h"

#define gravity -0.0025

struct SDL_Texture;

enum entity_state
{
	IDLE,
	RIGHT,
	LEFT,
	JUMPING,
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

	bool Entity_Update()
	{
		v.y += (gravity * ((colliding_bottom) ? 0 : 1));
		if (v.y < -1)
			v.y = -1;
		virtualPosition.y -= v.y;

		virtualPosition.x += v.x;

		if (v.x > 0.1)
			v.x = 0.1;
		else if (v.x < -0.1)
			v.x = -0.1;

		setAnimation();

		return true;
	}

	void Entity_OnCollision(Collider* c1, Collider* c2)
	{
		if (c2->type == COLLIDER_FLOOR)
		{
			if (((c2->rect.y - v.y + 1) > (c1->rect.y + (c1->rect.h)))) //The collision is from bottom
			{
				if (colliding_bottom == false)
				{
					v.y = 0;
					if (App->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT || v.x == 0)
					{
						v.x = 0;
						state = IDLE;
					}
					else if (v.x > 0)
					{
						state = RIGHT;
					}
					else if (v.x < 0)
					{
						state = LEFT;
					}
					colliding_bottom = true;
				}
				if (animation == &jumping_left)
					animation = &idle_left;
				else if (animation == &jumping_right)
					animation = &idle_right;
				//collidingC = c2;
			}
		}
	}

private:

	void setAnimation()
	{
		if (v.x > 0)
		{
			if (state == JUMPING)
			{
				animation = &jumping_right;
			}
			else
			{
				animation = &right;
			}
		}
		else if (v.x < 0)
		{
			if (state == JUMPING)
			{
				animation = &jumping_left;
			}
			else
			{
				animation = &left;
			}
		}
		else
		{
			if (state == IDLE)
			{
				if (animation == &left || animation == &jumping_left)
				{
					animation = &idle_left;
				}
				else if (animation == &right || animation == &jumping_right)
				{
					animation = &idle_right;
				}
			}
			else if (state == JUMPING)
			{
				if (animation == &idle_left)
				{
					animation = &jumping_left;
				}
				else if (animation == &idle_right)
				{
					animation = &jumping_right;
				}
			}
		}
	}

public:
	iPoint position;
	fPoint virtualPosition;
	fPoint v;
	bool colliding_bottom;
	bool colliding_top;
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
	Animation death;

	Collider* collider;

	float speed;
	float jump_force;
};

#endif // !__ModuleEntity_H__
