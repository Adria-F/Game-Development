#include "j1EntityManager.h"
#include "j1Charger.h"
#include "j1Bat.h"
#include "p2Log.h"

j1EntityManager::j1EntityManager()
{}

j1EntityManager::~j1EntityManager()
{}

bool j1EntityManager::Awake(pugi::xml_node& config)
{
	this->config = config;

	return true;
}

bool j1EntityManager::Start()
{
	createEntity(entity_type::BAT);

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

bool j1EntityManager::PostUpdate()
{
	for (p2List_item<Entity*>* entity = entities.start; entity; entity = entity->next)
	{
		entity->data->position.x = entity->data->virtualPosition.x;
		entity->data->position.y = entity->data->virtualPosition.y;
	}

	return true;
}

bool j1EntityManager::CleanUp()
{

	return true;
}

Entity* j1EntityManager::createEntity(entity_type type)
{
	Entity* ret = nullptr;
	
	switch (type)
	{
	case CHARGER:
		ret = new Charger();	
		break;
	case BAT:
		ret = new Bat();
		break;
	}
	ret->type = type;
	entities.add(ret);

	return ret;
}