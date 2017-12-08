#ifndef __j1INTROSCENE__
#define __j1INTROSCENE__

#include "j1Module.h"
#include "p2Point.h"
#include "j1UIScene.h"
#include "j1Scene.h"
#include "SDL\include\SDL_rect.h"

class j1IntroScene : public j1Module
{
public:
	j1IntroScene();
	// Destructor
	~j1IntroScene();

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

	// Called before quitting
	bool CleanUp();
};

#endif // !__j1INTROSCENE__

