#include "UI_ProgressBar.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "UI_Image.h"

ProgressBar::ProgressBar(int x, int y, SDL_Texture* texture, SDL_Rect empty, SDL_Rect full, Image* head, j1Module* callback) : UI_element(x, y, PROGRESSBAR, empty, callback, texture),
full(full),
head(head)
{
	head->parent = this;
}

void ProgressBar::addMarker(int x, int y, Image* active, Image* unactive)
{
	marker* newMarker = new marker();
	newMarker->position = { x, y };
	active->localPosition = { x, y };
	active->parent = this;
	unactive->localPosition = { x, y };
	unactive->parent = this;
	newMarker->active = active;
	newMarker->unactive = unactive;
	markers.add(newMarker);
}

void ProgressBar::setProgress(float newProgress)
{
	progress = newProgress;
}

float ProgressBar::getProgress() const
{
	return progress;
}

void ProgressBar::enterCurrentValue(float current_value)
{
	if (current_value == 0)
		progress = 0.0f;
	else if (current_value <= max_value)
		progress = current_value / max_value;
}

void ProgressBar::BlitElement()
{
	iPoint globalPos = calculateAbsolutePosition();
	App->render->Blit(texture, globalPos.x, globalPos.y, &section, false, App->gui->UI_scale);
	
	float bar_start;
	float bar_end;
	switch (type)
	{
	case INCREASING:
		full.w = section.w*progress;
		bar_start = 0;
		bar_end = full.w*App->gui->UI_scale;
		App->render->Blit(texture, globalPos.x, globalPos.y, &full, false, App->gui->UI_scale);
		head->localPosition = { (int)(full.w*App->gui->UI_scale) - (int)(head->section.w*App->gui->UI_scale) / 2, 0 };
		head->BlitElement();
		break;
	case DECREASING:
		full.w = section.w*(1.0f-progress);
		full.x = section.w - full.w;
		bar_start = (section.w*App->gui->UI_scale) - (full.w*App->gui->UI_scale);
		bar_end = section.w;
		App->render->Blit(texture, globalPos.x + (section.w*App->gui->UI_scale) - (full.w*App->gui->UI_scale), globalPos.y, &full, false, App->gui->UI_scale);
		head->localPosition = { (int)(section.w*App->gui->UI_scale) - (int)(full.w*App->gui->UI_scale) - 3, 0 };
		if (head->localPosition.x < (section.w*App->gui->UI_scale) - head->section.w/1.5f)
			head->BlitElement();
		break;
	}

	for (p2List_item<Image*>* item = highlights.start; item; item = item->next)
	{
		item->data->BlitElement();
	}

	for (p2List_item<marker*>* item = markers.start; item; item = item->next)
	{
		if ((item->data->position.x + item->data->active->section.w/(2/App->gui->UI_scale)) > bar_start && (item->data->position.x + item->data->active->section.w / (2/App->gui->UI_scale)) < bar_end) //In the full-bar area
		{
			item->data->active->BlitElement();
		}
		else
		{
			item->data->unactive->BlitElement();
		}
	}
	
}
