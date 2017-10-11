#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "j1Module.h"
#include "j1Entity.h"
#include "Animation.h"
#include "p2Point.h"

struct SDL_Texture;
struct Collider;

class j1Player : public Entity
{
public:
	j1Player();
	~j1Player();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

public:

	bool jumping;
	bool dead;
	bool falling;

	uint points = 0;
	int lives = 3;
	int num = 0;
	fPoint collider_move;
};

#endif