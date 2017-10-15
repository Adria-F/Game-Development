#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Audio.h"

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

	falling_right.PushBack({ 84, 33, 21, 33 });
	falling_left.PushBack({ 84, 132, 21, 33 });

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

	jump_cloud.PushBack({ 108, 60, 52, 20 });
	jump_cloud.PushBack({ 108, 40, 52, 20 });
	jump_cloud.PushBack({ 108, 20, 52, 20 });
	jump_cloud.PushBack({108, 0, 52, 20});
	jump_cloud.PushBack({ 108, 0, 0, 0 });
	jump_cloud.speed = 0.015f;
	jump_cloud.loop = false;
	

	cloud_offset.x = -16;
	cloud_offset.y = 17;

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

	if (graphics == nullptr)
		graphics = App->tex->Load("textures/Character spritesheet.png");

	if (collider == nullptr)
		collider = App->collision->AddCollider({ 0, 0, 21, 33 }, COLLIDER_PLAYER, this);

	collidingFloor = nullptr;
	colliding_bottom = false;
	colliding_left = false;
	colliding_right = false;

	animation = &idle_right;

	virtualPosition.x = position.x;
	virtualPosition.y = position.y;

	step_fx = App->audio->LoadFx("audio/fx/step.wav");
	jump_fx = App->audio->LoadFx("audio/fx/jump.wav");
	double_jump_fx = App->audio->LoadFx("audio/fx/double_jump.wav");
	landing_fx = App->audio->LoadFx("audio/fx/landing.wav");

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
		if (!double_jump)
		{
			if (state == JUMPING || state == FALLING)
			{
				double_jump = true;
				cloud_pos.x = position.x + cloud_offset.x;
				cloud_pos.y = position.y + cloud_offset.y;
				v.y = (jump_force * 2 / 3);
				if (state == FALLING)
				{
					state = JUMPING;
				}
				//Double jumping sound
				App->audio->PlayFx(double_jump_fx, 0);
			}
			else
			{
				v.y = jump_force;
				state = JUMPING;
				//Jumping sound
				App->audio->PlayFx(jump_fx, 0);

			}
		}
	}
	//Running sound
	if (v.x != 0 && colliding_bottom)
	{
		App->audio->PlayFx(step_fx, 0);
	}

	collider->SetPos(virtualPosition.x + collider_move.x, virtualPosition.y + collider_move.y);

	App->player->colliding_left = false;
	App->player->colliding_right = false;

	return true;
}

bool j1Player::PostUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && !colliding_right && v.x == 0)
	{
		v.x = speed;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && !colliding_left && v.x == 0)
	{
		v.x = -speed;
	}

	p2List_item<ImageLayer*>* image = nullptr;
	for (image = App->map->data.image_layers.start; image; image = image->next)
	{
		if (image->data->speed > 0)
		{
			if (image->data->constant_movement)
			{
				image->data->position.x -= image->data->speed;
			}
			else if (v.x > 0)
			{
				image->data->position.x -= image->data->speed;
			}
		}
	}

	position.x = virtualPosition.x;
	position.y = virtualPosition.y;
	int win_scale = App->win->GetScale();
	pos_relCam = App->player->position.x + App->render->camera.x / win_scale;

	App->render->Blit(graphics, position.x, position.y, &animation->GetCurrentFrame());

	if (double_jump)
	{
		App->render->Blit(graphics, cloud_pos.x, cloud_pos.y, &jump_cloud.GetCurrentFrame());
	}

	LOG("%d", position.x);
	LOG("%f", App->render->virtualCamPos);

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

	if (c2->type == COLLIDER_FLOOR)
	{
		if (((c2->rect.y - v.y + 1) > (c1->rect.y + (c1->rect.h)))) //The collision is from bottom
		{
			double_jump = false;
			jump_cloud.Reset();
		}
	}

	Entity_OnCollision(c1, c2);
}

bool j1Player::Load(pugi::xml_node& data)
{
	App->scene->LoadLvl(data.attribute("level").as_int());
	virtualPosition.x = data.attribute("position_x").as_int();
	virtualPosition.y = data.attribute("position_y").as_int();
	App->render->virtualCamPos = -(virtualPosition.x * (int)App->win->GetScale() - 100);
	
	return true;
}

bool j1Player::Save(pugi::xml_node& data) const
{
	data.append_attribute("position_x") = position.x;
	data.append_attribute("position_y") = position.y;
	data.append_attribute("level") = App->scene->current_lvl->data->lvl;
	
	return true;
}