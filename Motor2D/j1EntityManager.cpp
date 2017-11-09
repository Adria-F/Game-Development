#include "j1EntityManager.h"

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

	return true;
}

bool j1EntityManager::Update(float dt)
{

	return true;
}

bool j1EntityManager::PostUpdate()
{

	return true;
}

bool j1EntityManager::CleanUp()
{

	return true;
}