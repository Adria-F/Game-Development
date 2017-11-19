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
#include "j1entityManager.h"
#include "j1PathFinding.h"
#include "Brofiler\Brofiler.h"

#include<stdio.h>

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

j1Player::j1Player() : Entity("player")
{
	name.create("player");

	// Animations
	jump_cloud = LoadAnimation("animations/player.tmx", "jump_cloud");
	cloud_offset.x = -16;
	cloud_offset.y = 17;

	SSJ_aura = LoadAnimation("animations/player.tmx", "SSJ_aura");
	aura_offset.x = -7;
	aura_offset.y = -12;

	type = PLAYER;
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
		collider = App->collision->AddCollider({ 0, 0, 15, 29 }, COLLIDER_PLAYER, this);

	collider_offset.x = 3;
	collider_offset.y = 2;

	collidingFloor = nullptr;
	colliding_bottom = false;
	colliding_left = false;
	colliding_right = false;

	going_right = false;
	going_left = false;
	going_down = false;
	jumping = false;

	dead = false;
	sound_one_time = false;

	old_savedCol = nullptr;
	App->SaveGame(true);

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
	if (killed_fx == 0)
		killed_fx = App->audio->LoadFx("audio/fx/killed_by_enemy.wav");

	return true;
}

// Unload assets
bool j1Player::CleanUp()
{
	LOG("Unloading player");

	App->tex->UnLoad(graphics);
	graphics = nullptr;
	App->tex->UnLoad(graphics_god);
	graphics_god = nullptr;

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
	BROFILER_CATEGORY("Player Update", Profiler::Color::Red);
	if (!dead)
	{
		if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		{
			if (App->entityManager->player_god_mode)
			{
				App->entityManager->player_god_mode = false;
				App->audio->PlayFx(SSJ_off, 0);
			}
			else
			{
				App->entityManager->player_god_mode = true;
				App->audio->PlayFx(SSJ_transformation, 0);
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
		{
			v.x = -speed;
			going_left = true;
			if (state != JUMPING && state != DEAD)
			{
				state = LEFT;
			}
		}
		else if (App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
		{
			going_left = false;
			if (state == LEFT)
			{
				v.x = 0;
				state = IDLE;
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
		{
			v.x = speed;
			going_right = true;
			if (state != JUMPING && state != DEAD)
			{
				state = RIGHT;
			}
		}
		else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
		{
			going_right = false;
			if (state == RIGHT)
			{
				v.x = 0;
				state = IDLE;
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			jumping = true;
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
					App->audio->PlayFx(double_jump_fx, 0);
				}
				else
				{
					v.y = jump_force;
					state = JUMPING;
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
	}
	if (v.x != 0 && colliding_bottom && SDL_GetTicks() > step_time)
	{
		App->audio->PlayFx(step_fx, 0);
		step_time = SDL_GetTicks() + (1 / right->speed) + 450;
	}

	return true;
}

bool j1Player::PostUpdate(float dt)
{	
	if (!dead)
	{
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && !colliding_right && v.x == 0)
		{
			v.x = speed;
			state = RIGHT;
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && !colliding_left && v.x == 0)
		{
			v.x = -speed;
			state = LEFT;
		}
	}
	
	// Win condition
	if (((collider->rect.x + collider->rect.w) > App->scene->current_lvl->data->end.x) && (position.y + collider->rect.h) < (App->scene->current_lvl->data->end.y + App->scene->current_lvl->data->end.h))
	{
		if (end_reached == 0)
		{
			won = true;
			end_reached = SDL_GetTicks();
			if (App->scene->current_lvl == App->scene->levels.end)
			{
				App->audio->PlayFx(App->scene->win_fx, 0);
			}
			else
			{
				App->audio->PlayFx(App->scene->complete_level_fx, 0);
			}
		}
	}
	if (won && ((App->scene->current_lvl == App->scene->levels.end && SDL_GetTicks() > end_reached + 5000) || (App->scene->current_lvl != App->scene->levels.end && SDL_GetTicks() > end_reached + 500)))
	{
		end_reached = 0;
		won = false;
		App->scene->load_lvl = true;
	}

	// Lose condition
	//By enemyy
	if (dead && SDL_GetTicks() > killed_finished + 1500)
	{
		killed_finished = 0;
		App->scene->load_lvl = true;
		App->scene->newLvl = App->scene->current_lvl->data->lvl;
		dead = false;
	}
	//By falling
	int win_scale = App->win->GetScale();
	if (position.y > App->win->screen_surface->h / win_scale + 50 && !won && !dead)
	{
		if (App->entityManager->player_god_mode)
		{ 
			App->LoadGame(true);
		}
		else
		{
			App->audio->PlayFx(die_fx, 0);
			App->scene->load_lvl = true;
			App->scene->newLvl = App->scene->current_lvl->data->lvl;
		}
	}

	//When f10 is clicked he converts into super sayan (god mode)
	if (App->entityManager->player_god_mode)
	{
		App->render->Blit(graphics_god, position.x + aura_offset.x, position.y + aura_offset.y, &SSJ_aura->GetCurrentFrame(dt));
		App->render->Blit(graphics_god, position.x, position.y, &animation->GetCurrentFrame(dt));
		App->render->Blit(graphics_god, position.x + aura_offset.x, position.y + aura_offset.y, &SSJ_aura->GetCurrentFrame(dt));
	}
	else if (App->entityManager->player_god_mode == false)
	{
		App->render->Blit(graphics, position.x, position.y, &animation->GetCurrentFrame(dt));
	}

	if (double_jump)
	{
		App->render->Blit(graphics, cloud_pos.x, cloud_pos.y, &jump_cloud->GetCurrentFrame(dt));
	}

	return true;
}

void j1Player::OnCollision(Collider* c1, Collider* c2)
{
	if (c2->type == COLLIDER_FLOOR || c2->type == COLLIDER_JUMPABLE)
	{
		if (Collision_from_bottom(c1, c2))
		{
			double_jump = false;
			jump_cloud->Reset();
		}
	}

	//If player touches the charger he must die, if the player hits the bat from the top the bat must die
	if (!dead && c2->type == COLLIDER_ENEMY)
	{
		p2SString c2_name = c2->callback->name.GetString();
		if (c2_name == "bat" && Collision_from_bottom(c1, c2, 3) && v.y < 0)
		{
			v.y = (jump_force * 2 / 3);
			c2->entity->dead = true;
			c2->to_delete = true;
		}
		else if (!App->entityManager->player_god_mode && (c2_name == "charger" || (c2_name == "bat" && !c2->entity->dead && !Collision_from_bottom(c1, c2, 3))))
		{
			v.x = 0;
			dead = true;
			if (!sound_one_time && killed_finished == 0)
			{
   				killed_finished = SDL_GetTicks();
				App->audio->PlayFx(killed_fx, 0);
				sound_one_time = true;
			}
		}
	}

	Entity_OnCollision(c1, c2);
}

bool j1Player::Load(pugi::xml_node& data)
{
	App->scene->LoadLvl(data.attribute("level").as_int(), true);
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