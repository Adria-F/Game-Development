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
#include "Brofiler\Brofiler.h"

Coin::Coin() : Entity ("coin")
{
	graphics = App->tex->Load("textures/non-character-entities/coin.png");
	scale = 0.2f;//Needs to be changed
	collider = App->collision->AddCollider({ position.x, position.y, (int)(collider_size.x*scale), (int)(collider_size.y*scale) }, COLLIDER_COLLECTIBLE, this, this);
	collider_offset.x *= scale;
	collider_offset.y *= scale;

	if (earn_coin_fx == 0)
		earn_coin_fx = App->audio->LoadFx("audio/fx/earn_coin.wav");
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
	return true;
}

bool Coin::PostUpdate(float dt)
{

	return true;
}

bool Coin::CleanUp()
{
	return true;
}
void Coin::OnCollision(Collider* c1, Collider* c2)
{
	if (c2->type == COLLIDER_PLAYER && !App->entityManager->getPlayer()->dead)
	{
		App->audio->PlayFx(earn_coin_fx, 0);
		App->entityManager->DeleteEntity(this);
	}
}

bool Coin::Load(pugi::xml_node&)
{
	return true;
}
bool Coin::Save(pugi::xml_node&) const
{
	return true;
}