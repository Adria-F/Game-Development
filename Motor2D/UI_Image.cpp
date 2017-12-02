#include "UI_Image.h"
#include "j1App.h"
#include "j1Render.h"

void Image::BlitElement()
{
	iPoint globalPos = calculateAbsolutePosition();
	App->render->Blit(texture, globalPos.x, globalPos.y, &section, false, App->gui->UI_scale);
}
