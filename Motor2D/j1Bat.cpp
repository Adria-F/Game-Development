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
#include "j1entityManager.h"
#include "j1Textures.h"
#include "j1Player.h"
#include "j1PathFinding.h"
#include "j1Player.h"
#include "Brofiler\Brofiler.h"
#include <time.h>

Bat::Bat() : Entity("bat")
{
	graphics = App->tex->Load("textures/enemies/bat.png");
	scale = 0.2f;
	collider = App->collision->AddCollider({ position.x, position.y, (int)(collider_size.x*scale), (int)(collider_size.y*scale) }, COLLIDER_ENEMY, this, this);
	collider_offset.x *= scale;
	collider_offset.y *= scale;

	if (die_fx == 0)
		die_fx = App->audio->LoadFx("audio/fx/bat_die.wav");
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
	BROFILER_CATEGORY("Bat Update", Profiler::Color::Red);
	if (!dead)
	{
		if (!Calculate_Path())
		{
			if (!counting)
			{
				doStandardPath.Start();
				counting = true;
			}
			if (doStandardPath.Read() >= 1500)
			{
				standardPath();
				slowerPath = true;
			}
		}
		else
		{
			counting = false;
			slowerPath = false;
		}

		if (entityPath.Count() > 1)
			followPath();
	}

	if (death->Finished())
		App->entityManager->DeleteEntity(this);

	return true;
}

bool Bat::PostUpdate(float dt)
{

	return true;
}

bool Bat::CleanUp()
{
	return true;
}

void Bat::OnCollision(Collider* c1, Collider* c2)
{
	if (c2->type == COLLIDER_PLAYER && dead && !App->entityManager->getPlayer()->dead && Collision_from_top(c1, c2))
	{
		App->audio->PlayFx(die_fx, 0);
		v = { 0,0 };
	}
}

bool Bat::Load(pugi::xml_node&)
{
	return true;
}

bool Bat::Save(pugi::xml_node&) const
{
	return true;
}

void Bat::followPath()
{
	iPoint curr_cell;
	iPoint next_cell;
	curr_cell = *entityPath.At(1);
	if (entityPath.Count() > 2)
		next_cell = *entityPath.At(2);
	iPoint map_pos(position.x + collider_offset.x + collider->rect.w / 2, position.y + collider_offset.y + collider->rect.h / 2);
	iPoint map_cell = App->map->WorldToMap(map_pos.x, map_pos.y);

	iPoint curr_pos = App->map->MapToWorld(curr_cell.x, curr_cell.y);
	curr_pos = { curr_pos.x + App->map->data.tile_width / 2, curr_pos.y + App->map->data.tile_height / 2 };
	iPoint next_pos = App->map->MapToWorld(next_cell.x, next_cell.y);
	next_pos = { next_pos.x + App->map->data.tile_width / 2, next_pos.y + App->map->data.tile_height / 2 };

	float usingSpeed = (slowerPath) ? (speed / 2) : speed;

	if (curr_pos.x > map_pos.x)
	{
		v.x = usingSpeed;
		if (curr_cell.x > map_cell.x)
			state = RIGHT;
	}
	else if (curr_pos.x < map_pos.x)
	{
		v.x = -usingSpeed;
		if (curr_cell.x < map_cell.x)
			state = LEFT;
	}
	else
		v.x = 0;

	if (curr_pos.y > map_pos.y || (next_pos.y > map_pos.y && !App->pathfinding->isTouchingGround({ map_cell.x, map_cell.y + 1 })))
	{
		v.y = -usingSpeed;
	}
	else if (curr_pos.y < map_pos.y || (next_pos.y < map_pos.y && !App->pathfinding->isTouchingGround({ map_cell.x, map_cell.y - 2 })))
	{
		v.y = usingSpeed;
	}
	else
		v.y = 0;
}

void Bat::standardPath()
{
	srand(time(NULL));

	int newPosX = rand() % 10;
	newPosX = (newPosX < 5) ? 1 : -1;
	int newPosY = rand() % 10;
	newPosY = (newPosY < 5) ? 1 : -1;

	iPoint curr_pos = App->map->WorldToMap(position.x + collider_offset.x + collider->rect.w / 2, position.y + collider_offset.y + collider->rect.h / 2);

	curr_pos = { curr_pos.x + newPosX, curr_pos.y + newPosY }; // 1
	if (App->pathfinding->isWalkable(curr_pos))
		entityPath.PushBack(curr_pos);

	curr_pos = { curr_pos.x + newPosX, curr_pos.y + newPosY }; // 2
	if (App->pathfinding->isWalkable(curr_pos))
		entityPath.PushBack(curr_pos);
}