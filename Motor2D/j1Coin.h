#ifndef __j1Coin__
#define __j1Coin__

#include "j1Module.h"
#include "j1Entity.h"
#include "Animation.h"
#include "p2Point.h"

class Coin : public Entity
{
public:
	Coin(int id);
	~Coin();

	bool Awake(pugi::xml_node&);
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

public:
	uint earn_coin_fx = 0;
	int id = 0;
	SDL_Texture* ghost_graphics = nullptr;
};


#endif // !__j1Coin__

