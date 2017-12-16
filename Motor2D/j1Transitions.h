#ifndef __J1TRANSITIONS_H__
#define __J1TRANSITIONS_H__

#include "j1Module.h"
#include "j1UIScene.h"
#include "j1Scene.h"

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

	void menuTransition(menu_id newMenuID, float time = 0.5f);
	void sceneTransition(int newLvl, float time = 0.5f);

private:

	transition_state sceneState = UNACTIVE;
	transition_state menuState = UNACTIVE;
	bool doingMenuTransition = false;
	bool doingSceneTransition = false;
	menu_id newMenuID;
	int newLvl;
	float total_time = 0.0f;
	float alpha_value = 0;
};

#endif // !__J1TRANSITIONS_H__
