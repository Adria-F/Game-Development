#include "UI_Chrono.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Gui.h"

void Chrono::BlitElement()
{
	time_changed = App->gui->chrono.ReadSec();
	p2SString("%d", timer);

	

}

