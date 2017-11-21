#include "Slider.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"
#include "p2Log.h"

bool Slider::Start()
{
	container = App->tex->Load("UI/slider_container.png");
	bar = App->tex->Load("UI/slider_progress.png");
	button = App->tex->Load("UI/slider_button.png"); //y - 5

	position = { 100, 100 };
	
	return true;
}

bool Slider::CleanUp()
{
	App->tex->UnLoad(container);
	App->tex->UnLoad(bar);
	App->tex->UnLoad(button);
	
	return true;
}

bool Slider::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		progress += 1;
	else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		progress -= 1;

	if (progress < 0)
		progress = 0;
	else if (progress > 100)
		progress = 100;

	uint width, height;
	App->tex->GetSize(bar, width, height);
	SDL_Rect bar_progress;
	bar_progress.x = 0;
	bar_progress.y = 0;
	bar_progress.w = ((width - 14) * progress)/100;
	if (bar_progress.w < 0)
		bar_progress.w = 0;
	bar_progress.h = 16;

	LOG("Volume percentage: %d", progress);

	App->render->Blit(container, position.x, position.y);
	App->render->Blit(bar, position.x, position.y, &bar_progress);
	App->render->Blit(button, position.x + bar_progress.w, position.y - 2);
	
	return true;
}

bool Slider::PostUpdate(float dt)
{
	return true;
}

void Slider::setProgress(int newProgress)
{
	progress = newProgress;
}