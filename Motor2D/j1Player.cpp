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
#include "j1PathFinding.h"

#include<stdio.h>

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

j1Player::j1Player() : Entity("player")
{
	name.create("player");

	// Animations
	jump_cloud.PushBack({ 108, 60, 52, 20 });
	jump_cloud.PushBack({ 108, 40, 52, 20 });
	jump_cloud.PushBack({ 108, 20, 52, 20 });
	jump_cloud.PushBack({ 108, 0, 52, 20 });
	jump_cloud.PushBack({ 108, 0, 0, 0 });
	jump_cloud.speed = 0.17f;
	jump_cloud.loop = false;

	cloud_offset.x = -16;
	cloud_offset.y = 17;

	SSJ_aura.PushBack({ 163, 1, 36, 45 });
	SSJ_aura.PushBack({ 163, 47, 36, 45 });
	SSJ_aura.PushBack({ 163, 94, 36, 45 });
	SSJ_aura.speed = 0.1f;

	aura_offset.x = -7;
	aura_offset.y = -12;

	/*collider_offset.x = 2;
	collider_offset.y = 0;*/
}

j1Player::~j1Player()
{}

// Load assets
bool j1Player::Start()
{
	LOG("Loading player");

	if (graphics == nullptr)
		graphics = App->tex->Load("textures/character/Character spritesheet.png");

	if (graphics_god == nullptr)
		graphics_god = App->tex->Load("textures/character/godmode_spritesheet.png");

	if (collider == nullptr)
		collider = App->collision->AddCollider({ 0, 0, 21, 33 }, COLLIDER_PLAYER, this);

	collidingFloor = nullptr;
	colliding_bottom = false;
	colliding_left = false;
	colliding_right = false;

	v.x = 0;
	v.y = 0;

	animation = idle_right;

	virtualPosition.x = position.x;
	virtualPosition.y = position.y;

	if (step_fx == 0)
		step_fx = App->audio->LoadFx("audio/fx/step.wav");
	if (jump_fx == 0)
		jump_fx = App->audio->LoadFx("audio/fx/jump.wav");
	if (double_jump_fx == 0)
		double_jump_fx = App->audio->LoadFx("audio/fx/double_jump.wav");
	if (landing_fx == 0)
		landing_fx = App->audio->LoadFx("audio/fx/landing.wav");
	if (die_fx == 0)
		die_fx = App->audio->LoadFx("audio/fx/die.wav");
	if (SSJ_transformation == 0)
		SSJ_transformation = App->audio->LoadFx("audio/fx/SSJ_transformation.wav");
	if (SSJ_off == 0)
		SSJ_off = App->audio->LoadFx("audio/fx/SSJ_off.wav");

	//temp
	/*max_jump_value = 6;
	path = App->pathfinding->getPath(this, { 288, 320 });
	path_marker = App->tex->Load("assets/maps/non-walkable-tile.png");*/

	return true;
}

// Unload assets
bool j1Player::CleanUp()
{
	LOG("Unloading player");

	App->tex->UnLoad(graphics);
	App->tex->UnLoad(graphics_god);
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
	Entity_Update(dt);

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
		if (state == LEFT)
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
		if (state == RIGHT)
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
	else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
	{
		// If player release space in mid jump, the character won't reach max height
		if (!double_jump && v.y > (jump_force * 2 / 3) / 2)
		{
			v.y = (jump_force * 2 / 3) / 2;
		}
	}
	
	//Running sound
	if (v.x != 0 && colliding_bottom && SDL_GetTicks() > step_time)
	{
		App->audio->PlayFx(step_fx, 0);
		step_time = SDL_GetTicks() + (1 / right->speed) + 450;
	}

	return true;
}

bool j1Player::PostUpdate(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && !colliding_right && v.x == 0)
	{
		v.x = speed;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && !colliding_left && v.x == 0)
	{
		v.x = -speed;
	}

	int win_scale = App->win->GetScale();
	if (position.y > App->win->screen_surface->h / win_scale + 50 && !won)
	{
		App->audio->PlayFx(die_fx, 0);
		App->scene->load_lvl = true;
		App->scene->newLvl = App->scene->current_lvl->data->lvl;
	}

	//When f10 is clicked he converts into super sayan (god mode)
	if (god_mode)
	{
		App->render->Blit(graphics_god, position.x + aura_offset.x, position.y + aura_offset.y, &SSJ_aura.GetCurrentFrame(dt));
		App->render->Blit(graphics_god, position.x, position.y, &animation->GetCurrentFrame(dt));
		App->render->Blit(graphics_god, position.x + aura_offset.x, position.y + aura_offset.y, &SSJ_aura.GetCurrentFrame(dt));
	}
	else if (god_mode == false)
	{
		App->render->Blit(graphics, position.x, position.y, &animation->GetCurrentFrame(dt));
	}

	if (double_jump)
	{
		App->render->Blit(graphics, cloud_pos.x, cloud_pos.y, &jump_cloud.GetCurrentFrame(dt));
	}

	/*int i = 0;
	while(path.Count() > 0)
	{
		iPoint coords = App->map->MapToWorld(path.At(i)->x, path.At(i)->y);
		App->render->Blit(path_marker, coords.x, coords.y);
		i++;
	}*/

	return true;
}

void j1Player::OnCollision(Collider* c1, Collider* c2)
{
	if (c2->type == COLLIDER_FLOOR || c2->type == COLLIDER_JUMPABLE)
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
	App->render->virtualCamPos = -(virtualPosition.x * (int)App->win->GetScale() - 300);
	if (App->render->virtualCamPos > 0)
	{
		App->render->virtualCamPos = 0;
	}
	if (App->render->virtualCamPos < App->scene->max_camera_pos)
	{
		App->render->virtualCamPos = App->scene->max_camera_pos;
	}
	
	return true;
}

bool j1Player::Save(pugi::xml_node& data) const
{
	data.append_attribute("position_x") = position.x;

	data.append_attribute("position_y") = position.y - 5;

	data.append_attribute("level") = App->scene->current_lvl->data->lvl;
	
	return true;
}