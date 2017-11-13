#ifndef __ModuleCharger__
#define __ModuleCharger__

#include "j1Module.h"
#include "j1Entity.h"
#include "Animation.h"
#include "p2Point.h"

class Charger : public Entity
{
public:
	Charger();
	~Charger();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;
};

#endif // !__ModuleCharger__

