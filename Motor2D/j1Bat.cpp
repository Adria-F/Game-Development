#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Bat.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Audio.h"

j1Bat::j1Bat() : Entity("bat")
{
}

j1Bat::~j1Bat()
{
}

bool j1Bat::Awake(pugi::xml_node&)
{
	return true;
}
bool j1Bat::Start()
{
	return true;
}
bool j1Bat::Update(float dt)
{
	return true;
}
bool j1Bat::PostUpdate()
{
	return true;
}
bool j1Bat::CleanUp()
{
	return true;
}
void j1Bat::OnCollision(Collider* c1, Collider* c2)
{

}

bool j1Bat::Load(pugi::xml_node&)
{
	return true;
}
bool j1Bat::Save(pugi::xml_node&) const
{
	return true;
}