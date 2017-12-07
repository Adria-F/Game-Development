#include "UI_Chrono.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Gui.h"


void Chrono::BlitElement()
{
	if (timer < time_changed)
	{
		timer++;
		p2SString secs = p2SString("%d", timer);
		text->setText(secs);
	}
	time_changed = App->gui->chrono.ReadSec();
	text->BlitElement();
}

