#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "p2Log.h"

#include<stdio.h>

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

j1Player::j1Player()
{
	name.create("player");
	
	// ----- Animations -----
	idle_right.PushBack({ 0, 66, 21, 33 });
	idle_left.PushBack({ 84, 66, 21, 33 });

	jumping_right.PushBack({ 63, 33, 21, 33 });
	jumping_left.PushBack({ 63, 132, 21, 33 });

	right.PushBack({ 0, 0, 21, 33 });
	right.PushBack({ 21, 0, 21, 33 });
	right.PushBack({ 42, 0, 21, 33 });
	right.PushBack({ 63, 0, 21, 33 });
	right.PushBack({ 84, 0, 21, 33 });
	right.PushBack({ 0, 33, 21, 33 });
	right.PushBack({ 21, 33, 21, 33 });
	right.PushBack({ 42, 33, 21, 33 });
	right.speed = 0.007f;

	left.PushBack({ 0, 99, 21, 33 });
	left.PushBack({ 21, 99, 21, 33 });
	left.PushBack({ 42, 99, 21, 33 });
	left.PushBack({ 63, 99, 21, 33 });
	left.PushBack({ 84, 99, 21, 33 });
	left.PushBack({ 0, 132, 21, 33 });
	left.PushBack({ 21, 132, 21, 33 });
	left.PushBack({ 42, 132, 21, 33 });
	left.speed = 0.007f;

	/*collider_move.x = 2;
	collider_move.y = 0;*/
}

j1Player::~j1Player()
{}

bool j1Player::Awake(pugi::xml_node& config)
{
	speed = config.attribute("speed").as_float();
	jump_force = config.attribute("jump_force").as_float();

	return true;
}

// Load assets
bool j1Player::Start()
{
	LOG("Loading player");

	graphics = App->tex->Load("textures/Character spritesheet.png");

	if (collider == nullptr)
		collider = App->collision->AddCollider({ 0, 0, 21, 33 }, COLLIDER_PLAYER, this);

	animation = &idle_right;

	position.x = 50;
	position.y = 150;

	virtualPosition.x = position.x;
	virtualPosition.y = position.y;

	dead = false;

	return true;
}

// Unload assets
bool j1Player::CleanUp()
{
	LOG("Unloading player");

	App->tex->UnLoad(graphics);
	if (collider != nullptr)
	{
		collider->to_delete = true;
		collider = nullptr;
	}

	return true;
}

// Update: draw background
bool j1Player::Update(float dt)
{
	Entity_Update();

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		v.x = -speed;
		if (state != JUMPING && state != DEAD)
		{
			state = LEFT;
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
	{
		if (v.x < 0 && state != JUMPING)
		{
			v.x = 0;
			state = IDLE;
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		v.x = speed;
		if (state != JUMPING && state != DEAD)
		{
			state = RIGHT;
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
	{
		if (v.x > 0 && state != JUMPING)
		{
			v.x = 0;
			state = IDLE;
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		if (state != JUMPING)
		{
			v.y = jump_force;
			state = JUMPING;
		}
	}

	collider->SetPos(virtualPosition.x + collider_move.x + (App->render->camera.x / 2), virtualPosition.y + collider_move.y);

	App->player->colliding_left = false;
	App->player->colliding_right = false;

	return true;
}

bool j1Player::PostUpdate()
{
	position.x = virtualPosition.x;
	position.y = virtualPosition.y;

	App->render->Blit(graphics, position.x, position.y, &animation->GetCurrentFrame(), 1.0, false);
	LOG("Blited Player");

	return true;
}

void j1Player::OnCollision(Collider* c1, Collider* c2)
{
	/*if ((c2->rect.x + 4) > (c1->rect.x + (c1->rect.w)))
	{ 
		//LOG("Collision from right");
		//going_right = false;
		if (v.x > 0)
		{
			v.x = 0;
		}
		colliding_right = true;
	}
	else if ((c2->rect.x + (c2->rect.w)) < (c1->rect.x + 5))
	{ 
		//LOG("Collision from left");
		//going_left = false;
		if (v.x < 0)
		{
			v.x = 0;
		}
		colliding_left = true;
	}
	if ((c2->rect.y + (c2->rect.h)) < (c1->rect.y + 4))
	{ 
		//LOG("Collision from top");
		if (v.y > 0)
		{
			v.y = 0;
		}
		//position.y += 1;
		colliding_top = true;
		collidingC = c2;
	}*/
	/*if (((c2->rect.y - v.y + 1) > (c1->rect.y + (c1->rect.h))))
	{ 
		//LOG("Collision from bottom");
		if (colliding_bottom == false)
		{
			v.y = 0;
			state = IDLE;
			colliding_bottom = true;
		}
		if (animation == &jumping_left)
			animation = &idle_left;
		else if (animation == &jumping_right)
			animation = &idle_right;
		//collidingC = c2;
	}*/

	Entity_OnCollision(c1, c2);
}

bool j1Player::Load(pugi::xml_node& data)
{
	virtualPosition.x = data.attribute("position_x").as_int();
	virtualPosition.y = data.attribute("position_y").as_int();
	
	return true;
}

bool j1Player::Save(pugi::xml_node& data) const
{
	data.append_attribute("position_x") = position.x;
	data.append_attribute("position_y") = position.y;
	
	return true;
}