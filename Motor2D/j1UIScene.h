#ifndef __j1UISCENE_H__
#define __j1UISCENE_H__

#include "j1Module.h"

class Button;

class j1UIScene : public j1Module
{
public:

	j1UIScene();
	~j1UIScene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate(float dt);

	bool OnUIEvent(UI_element* element, event_type event_type);

	// Called before quitting
	bool CleanUp();

public:

	Button* newGame = nullptr;
	Button* continueGame = nullptr;
	Button* setting = nullptr;
	Button* credits = nullptr;
	Button* quit = nullptr;
	Button* pause = nullptr;
	Button* restart = nullptr;
};

#endif // !__j1UISCENE_H__