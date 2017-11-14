#include "j1EntityManager.h"
#include "j1Charger.h"
#include "j1Bat.h"
#include "p2Log.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1App.h"
#include "j1Collision.h"

j1EntityManager::j1EntityManager()
{
	name.create("entityManager");
}

j1EntityManager::~j1EntityManager()
{}

bool j1EntityManager::Awake(pugi::xml_node& config)
{
	this->config = config;
	for (p2List_item<Entity*>* entity = entities.start; entity; entity = entity->next)
	{
		entity->data->Awake(config.child(entity->data->name.GetString()));
	}

	return true;
}

bool j1EntityManager::Start()
{


	return true;
}

bool j1EntityManager::Update(float dt)
{
	for (p2List_item<Entity*>* entity = entities.start; entity; entity = entity->next)
	{
		entity->data->Entity_Update(dt);
		entity->data->Update(dt);
	}

	return true;
}

bool j1EntityManager::PostUpdate(float dt)
{
	for (p2List_item<Entity*>* entity = entities.start; entity; entity = entity->next)
	{
		entity->data->PostUpdate(dt);
		App->render->Blit(entity->data->graphics, entity->data->position.x, entity->data->position.y, &entity->data->animation->GetCurrentFrame(dt), entity->data->scale);
	}

	return true;
}

bool j1EntityManager::CleanUp()
{
	p2List_item<Entity*>* item;
	item = entities.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	entities.clear();

	return true;
}

Entity* j1EntityManager::createEntity(entity_type type, int x, int y)
{
	Entity* ret = nullptr;
	
	switch (type)
	{
	case CHARGER:
		ret = new Charger();
		break;
	case BAT:
		ret = new Bat();
		ret->flying = true;
		break;
	}
	ret->type = type;
	ret->virtualPosition.x = ret->position.x = x;
	ret->virtualPosition.y = ret->position.y = y;
	ret->animation = ret->idle_left;

	float time = ret->jump_force / -gravity;
	int max_height = (ret->jump_force * time) + ((gravity / 2) * time * time);
	ret->max_jump_value = (max_height / App->map->data.tile_height) * 2;

	entities.add(ret);

	return ret;
}