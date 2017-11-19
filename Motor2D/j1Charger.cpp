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
#include "j1PathFinding.h"
#include "Brofiler\Brofiler.h"

Charger::Charger() : Entity("charger")
{
	graphics = App->tex->Load("textures/enemies/charger.png");
	scale = 0.5f;
	collider = App->collision->AddCollider({ position.x, position.y, (int)(collider_size.x*scale), (int)(collider_size.y*scale) }, COLLIDER_ENEMY, this, this);
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
	BROFILER_CATEGORY("Charger Update", Profiler::Color::Red);
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

	return true;
}
bool Charger::PostUpdate(float dt)
{

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

void Charger::followPath()
{
	iPoint next_cell;
	next_cell = *entityPath.At(1);
	next_cell = App->map->MapToWorld(next_cell.x, next_cell.y);
	next_cell = { next_cell.x + App->map->data.tile_width / 2, next_cell.y};
	iPoint map_pos(position.x + collider_offset.x + collider->rect.w / 2, position.y + collider_offset.y + collider->rect.h / 2);

	float usingSpeed = (slowerPath) ? (speed / 2) : speed;

	if (next_cell.x > map_pos.x)
	{
		v.x = usingSpeed;
		state = RIGHT;
		going_right = true;
		going_left = false;
	}
	else if (next_cell.x < map_pos.x)
	{
		v.x = -usingSpeed;
		state = LEFT;
		going_left = true;
		going_right = false;
	}
	else
	{
		v.x = 0;
		going_right = false;
		going_left = false;
	}

	if (next_cell.y > map_pos.y)
	{
		state = FALLING;
		going_down = true;
	}
}

void Charger::standardPath()
{
	iPoint curr_pos = App->map->WorldToMap(position.x + collider_offset.x + collider->rect.w / 2, position.y + collider_offset.y + collider->rect.h / 2);
	iPoint rightCell(curr_pos.x - 1, curr_pos.y);
	iPoint leftCell(curr_pos.x + 1, curr_pos.y);
	entityPath.PushBack(curr_pos);

	if (moving_right && App->pathfinding->isWalkable(rightCell) && App->pathfinding->isTouchingGround(rightCell))
		entityPath.PushBack(rightCell);
	else if (!moving_left)
	{
		entityPath.PushBack(curr_pos);
		moving_right = false;
		moving_left = true;
	}
	if (moving_left && App->pathfinding->isWalkable(leftCell) && App->pathfinding->isTouchingGround(leftCell))
		entityPath.PushBack(leftCell);
	else if (!moving_right)
	{
		entityPath.PushBack(curr_pos);
		moving_right = true;
		moving_left = false;
	}
}