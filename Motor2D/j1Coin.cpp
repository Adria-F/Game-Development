#include "j1Coin.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1EntityManager.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1UIScene.h"
#include "j1Audio.h"
#include "j1Player.h"
#include "Brofiler\Brofiler.h"

Coin::Coin(int id) : Entity("coin")
{
	graphics = App->tex->Load("textures/non-character-entities/coin.png");
	ghost_graphics = App->tex->Load("textures/non-character-entities/ghost_coin.png");
	scale = 0.2f;
	collider = App->collision->AddCollider({ position.x, position.y, (int)(collider_size.x*scale), (int)(collider_size.y*scale) }, COLLIDER_COLLECTIBLE, this, this);
	collider_offset.x *= scale;
	collider_offset.y *= scale;

	this->id = id;

	if (earn_coin_fx == 0)
		earn_coin_fx = App->audio->LoadFx("audio/fx/earn_coin.wav");

	j1Player* player = (j1Player*)App->entityManager->getPlayer();
	if (player->coins[id - 1])
	{
		SDL_Texture* tmp = graphics;
		graphics = ghost_graphics;
		ghost_graphics = tmp;
	}
}


Coin::~Coin()
{
}

bool Coin::Awake(pugi::xml_node&)
{

	return true;
}

bool Coin::Update(float dt)
{
	BROFILER_CATEGORY("Coin Update", Profiler::Color::Cyan);
	if (moving)
	{
		fPoint dPos;
		dPos.x = distanceTo.x/(0.3 / dt);
		dPos.y = distanceTo.y/(0.3 / dt);

		virtualPosition.x -= dPos.x;
		virtualPosition.y -= dPos.y;
		if (pos_relCam <= goingTo.x && virtualPosition.y <= goingTo.y)
		{
			j1Player* player = (j1Player*)App->entityManager->getPlayer();
			player->score += 1000;
			player->coins[id - 1] = true;
			moving = false;
			App->entityManager->DeleteEntity(this);
		}
	}

	return true;
}

bool Coin::PostUpdate(float dt)
{

	return true;
}

bool Coin::CleanUp()
{
	App->tex->UnLoad(ghost_graphics);
	ghost_graphics = nullptr;

	return true;
}
void Coin::OnCollision(Collider* c1, Collider* c2)
{
	if (c2->type == COLLIDER_PLAYER && !App->entityManager->getPlayer()->dead && !moving)
	{
		App->audio->PlayFx(earn_coin_fx, 0);

		j1Player* player = (j1Player*)App->entityManager->getPlayer();
		if (!player->coins[id - 1])
		{
			moveTo(player->coins_pos[id - 1].x, player->coins_pos[id - 1].y);
		}
		else
		{
			App->entityManager->DeleteEntity(this);
		}
	}
}

void Coin::moveTo(int x, int y)
{
	goingTo = { x,y };
	distanceTo.x = pos_relCam - x;
	distanceTo.y = position.y - y;
	moving = true;
}

bool Coin::Load(pugi::xml_node&)
{
	return true;
}
bool Coin::Save(pugi::xml_node&) const
{
	return true;
}