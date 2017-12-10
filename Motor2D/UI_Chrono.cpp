#include "UI_Chrono.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Gui.h"


void Chrono::setStartValue(int new_start_value)
{
	start_value = new_start_value;
}

void Chrono::setAlarm(int alarm)
{
	alarms.PushBack(alarm);
}

void Chrono::restartChrono()
{
	switch (this->type)
	{
	case TIMER:
		time = start_value;
		break;
	case STOPWATCH:
		time = 0;
		break;
	}
}

void Chrono::BlitElement()
{
	time_elapsed = counter.ReadSec();

	switch (type)
	{
	case STOPWATCH:
		if (time < time_elapsed)
		{
			time++;
			p2SString secs("%d", time);
			text->setText(secs);
		}
		break;
	case TIMER:
		if (start_value - time_elapsed != time)
		{
			time = start_value - time_elapsed;
			if (time < 0)
				time = 0;
			else
			{
				p2SString secs("%d", time);
				text->setText(secs);
			}
		}
		break;
	}

	text->BlitElement();
}