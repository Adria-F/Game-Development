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

j1Charger::j1Charger() : Entity("charger")
{
}

j1Charger::~j1Charger()
{
}

bool j1Charger::Awake(pugi::xml_node&)
{
	return true;
}
bool j1Charger::Start()
{
	return true;
}
bool j1Charger::Update(float dt)
{
	return true;
}
bool j1Charger::PostUpdate()
{
	return true;
}
bool j1Charger::CleanUp()
{
	return true;
}
void j1Charger::OnCollision(Collider* c1, Collider* c2)
{

}

bool j1Charger::Load(pugi::xml_node&)
{
	return true;
}
bool j1Charger::Save(pugi::xml_node&) const
{
	return true;
}