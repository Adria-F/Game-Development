#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Bat.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Audio.h"
#include "j1Textures.h"

Bat::Bat() : Entity("bat")
{
	graphics = App->tex->Load("textures/enemies/bat.png");
	speed = 2;
	jump_force = 6;
	scale = 0.2f;
	collider = App->collision->AddCollider({ 0, 0, (int)(collider_size.x*scale), (int)(collider_size.y*scale) }, COLLIDER_ENEMY, this);
	collider_offset.x *= scale;
	collider_offset.y *= scale;
}

Bat::~Bat()
{
}

bool Bat::Awake(pugi::xml_node&)
{
	return true;
}

bool Bat::Update(float dt)
{
	/*if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		v.x = -speed;
		state = LEFT;
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
		state = RIGHT;
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
		v.y = speed;
	}
	else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_UP)
	{
		v.y = 0;
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
	{
		v.y = -speed;
	}
	else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
	{
		v.y = 0;
	}*/
	
	return true;
}

bool Bat::PostUpdate(float dt)
{
	/*if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && !colliding_left && v.x == 0)
	{
		v.x = -speed;
		state = LEFT;
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && !colliding_right && v.x == 0)
	{
		v.x = speed;
		state = RIGHT;
	}*/
	
	return true;
}

bool Bat::CleanUp()
{
	return true;
}

void Bat::OnCollision(Collider* c1, Collider* c2)
{

}

bool Bat::Load(pugi::xml_node&)
{
	return true;
}

bool Bat::Save(pugi::xml_node&) const
{
	return true;
}