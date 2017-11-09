#ifndef __ENTITY_MANAGER_H__
#define __ENTITY_MANAGER_H__

#include "j1Module.h"
#include "j1Entity.h"
#include "p2List.h"

class j1EntityManager : public j1Module
{
public:

	j1EntityManager();
	~j1EntityManager();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	Entity* createEntity(entity_type type);

private:

	p2List<Entity*> entities;
	pugi::xml_node config;
};

#endif // !__ENTITY_MANAGER_H__
