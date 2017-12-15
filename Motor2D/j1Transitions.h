#ifndef __J1TRANSITIONS_H__
#define __J1TRANSITIONS_H__

#include "j1Module.h"
#include "j1UIScene.h"
#include "j1Scene.h"

enum transition_effect
{
	FADE,
	DRAG
};

enum transition_type
{
	MENU,
	SCENE
};

enum transition_state
{
	UNACTIVE,
	GIN,
	GOUT
};

class Transitions : public j1Module
{
public:

	Transitions();
	~Transitions();

	bool Update(float dt);
	bool PostUpdate(float dt);

	void menuTransition(menu_id newMenuID, transition_effect effect, float time = 0.5f);
	void sceneTransition(int newLvl, transition_effect effect, float time = 0.5f);

private:

	transition_state state = UNACTIVE;
	bool doingTransition = false;
	menu_id newMenuID;
	int newLvl;
	transition_effect currentEffect;
	transition_type type;
	float total_time = 0.0f;
	float alpha_value = 0;
};

#endif // !__J1TRANSITIONS_H__
