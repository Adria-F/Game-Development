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
//temp
#include "j1PathFinding.h"
#include "j1Player.h"

Charger::Charger() : Entity("charger")
{
	graphics = App->tex->Load("textures/enemies/charger.png");
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
	if (!dead && Calculate_Path())
	{
		iPoint next_cell;
		next_cell = *path_to_player.At(1);
		iPoint map_pos = App->map->WorldToMap(position.x + collider_offset.x + collider->rect.w / 2, position.y + collider_offset.y + collider->rect.h / 2);

		if (next_cell.x > map_pos.x)
		{
			v.x = speed;
			state = RIGHT;
			going_right = true;
			going_left = false;
		}
		else if (next_cell.x < map_pos.x)
		{
			v.x = -speed;
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
			v.y = -speed;
			state = FALLING;
			going_down = true;
		}
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