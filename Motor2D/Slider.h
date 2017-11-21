#ifndef __SLIDER_H__
#define __SLIDER_H__

#include "j1Module.h"
#include "p2Point.h"

struct SDL_Texture;

class Slider : public j1Module
{
public:

	Slider()
	{}

	~Slider()
	{}

	bool Start();
	bool Update(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();

	void setProgress(int newProgress);

public:

	SDL_Texture* container;
	SDL_Texture* bar;
	SDL_Texture* button;
	iPoint position;

private:

	int progress;
};

#endif // !__SLIDER_H__
