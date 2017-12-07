#include "j1Collectible.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1UIScene.h"
#include "j1Audio.h"
#include "Brofiler\Brofiler.h"

Collectible::Collectible() : Entity ("collectible")
{
	graphics = App->tex->Load("textures/non-character-entities/coin.png");
	scale = 0.2f;//Needs to be changed
	collider = App->collision->AddCollider({ position.x, position.y, (int)(collider_size.x*scale), (int)(collider_size.y*scale) }, COLLIDER_COLLECTIBLE, this, this);
	collider_offset.x *= scale;
	collider_offset.y *= scale;

	if (earn_coin_fx == 0)
		earn_coin_fx = App->audio->LoadFx("audio/fx/earn_coin.wav");
}


Collectible::~Collectible()
{
}

bool Collectible::Awake(pugi::xml_node&)
{
	return true;
}

bool Collectible::Update(float dt)
{
	BROFILER_CATEGORY("Collectible Update", Profiler::Color::Cyan);
	return true;
}

bool Collectible::PostUpdate(float dt)
{

	return true;
}

bool Collectible::CleanUp()
{
	return true;
}
void Collectible::OnCollision(Collider* c1, Collider* c2)
{
	Entity_OnCollision(c1, c2);
}

bool Collectible::Load(pugi::xml_node&)
{
	return true;
}
bool Collectible::Save(pugi::xml_node&) const
{
	return true;
}