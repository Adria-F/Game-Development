#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Charger.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Audio.h"

Charger::Charger() : Entity("charger")
{
	graphics = App->tex->Load("textures/enemies/charger.png");
	speed = 2;
	jump_force = 6;
	scale = 0.5f;
	collider = App->collision->AddCollider({ 0, 0, (int)(collider_size.x*scale), (int)(collider_size.y*scale) }, COLLIDER_ENEMY, this);
	collider_offset.x *= scale;
	collider_offset.y *= scale;
}

Charger::~Charger()
{

}

bool Charger::Awake(pugi::xml_node&)
{
	return true;
}
bool Charger::Start()
{
	return true;
}
bool Charger::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		v.x = -speed;
		if (state != JUMPING && state != DEAD)
		{
			state = LEFT;
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
	{
		if (state == LEFT)
		{
			v.x = 0;
			state = IDLE;
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		v.x = speed;
		if (state != JUMPING && state != DEAD)
		{
			state = RIGHT;
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
	{
		if (state == RIGHT)
		{
			v.x = 0;
			state = IDLE;
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
	{
		if (state != JUMPING && state != FALLING)
		{
			v.y = jump_force;
			state = JUMPING;
		}
	}

	return true;
}
bool Charger::PostUpdate(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && !colliding_left && v.x == 0)
	{
		v.x = -speed;
		state = LEFT;
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && !colliding_right && v.x == 0)
	{
		v.x = speed;
		state = RIGHT;
	}

	return true;
}
bool Charger::CleanUp()
{
	return true;
}
void Charger::OnCollision(Collider* c1, Collider* c2)
{
	Entity_OnCollision(c1, c2);
}

bool Charger::Load(pugi::xml_node&)
{
	return true;
}
bool Charger::Save(pugi::xml_node&) const
{
	return true;
}