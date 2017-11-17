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

Bat::Bat() : Entity("bat")
{
	graphics = App->tex->Load("textures/enemies/bat.png");
	scale = 0.2f;
	collider = App->collision->AddCollider({ 0, 0, (int)(collider_size.x*scale), (int)(collider_size.y*scale) }, COLLIDER_ENEMY, this, this);
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
	if (!dead && Calculate_Path())
	{
		iPoint curr_cell;
		iPoint* next_cell = nullptr;
		curr_cell = *path_to_player.At(1);
		if (path_to_player.Count() > 1)
			next_cell = path_to_player.At(2);
		iPoint map_pos = App->map->WorldToMap(position.x + collider_offset.x + collider->rect.w / 2, position.y + collider_offset.y + collider->rect.h / 2);

		if (curr_cell.x > map_pos.x)
		{
			v.x = speed;
			state = RIGHT;
		}
		else if (curr_cell.x < map_pos.x)
		{
			v.x = -speed;
			state = LEFT;
		}
		else
			v.x = 0;

		if (curr_cell.y > map_pos.y || (next_cell != nullptr && next_cell->y > map_pos.y))
		{
			v.y = -speed;
		}
		else if (curr_cell.y < map_pos.y || (next_cell != nullptr && next_cell->y < map_pos.y))
		{
			v.y = speed;
		}
		else
			v.y = 0;
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
	if (c2->type == COLLIDER_PLAYER && !App->player->dead && Collision_from_top(c1, c2))
	{
		App->audio->PlayFx(die_fx, 0);
		v = { 0,0 };
		c1->to_delete;
		dead = true;
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