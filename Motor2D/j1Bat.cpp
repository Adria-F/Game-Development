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
#include "j1Textures.h"

Bat::Bat() : Entity("bat")
{
	graphics = App->tex->Load("textures/enemies/bat.png");
	speed = 2;
	jump_force = 6;
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
	return true;
}

bool Bat::PostUpdate()
{
	return true;
}

bool Bat::CleanUp()
{
	App->tex->UnLoad(graphics);

	return true;
}

void Bat::OnCollision(Collider* c1, Collider* c2)
{

}

bool Bat::Load(pugi::xml_node&)
{
	return true;
}

bool Bat::Save(pugi::xml_node&) const
{
	return true;
}