#include "UI_Slider.h"
#include "j1App.h"
#include "j1Render.h"
#include "UI_Button.h"

void Slider::appendChild(int x, int y, UI_element * child)
{
	child->localPosition = { x, y };
	child->parent = this;

	if (child->element_type == TEXT)
		progress_num = (Text*)child;
	else if (child->element_type == BUTTON)
		button = (Button*)child;
}

void Slider::BlitElement()
{
	iPoint globalPos = calculateAbsolutePosition();
	App->render->Blit(texture, globalPos.x, globalPos.y, &section, false, App->gui->UI_scale);

	button->localPosition.y = -2;
	if (button->localPosition.x < -6)
		button->localPosition.x = -6;
	else if (button->localPosition.x >(section.w * App->gui->UI_scale - button->section.w * App->gui->UI_scale + 10))
		button->localPosition.x = (section.w * App->gui->UI_scale - button->section.w * App->gui->UI_scale + 10);

	p2SString newText("%d", progress);
	progress_num->setText(newText);
	progress_num->localPosition.x = full.w * App->gui->UI_scale - progress_num->section.w / (2 / App->gui->UI_scale);
	progress_num->localPosition.y = -progress_num->section.h * App->gui->UI_scale;

	progress = (button->localPosition.x + button->section.w / (2 / App->gui->UI_scale)) / (bar_length * App->gui->UI_scale) * 100;
	full.w = ((bar_length)* progress) / 100;
	App->render->Blit(texture, globalPos.x, globalPos.y, &full, false, App->gui->UI_scale);

	button->BlitElement();
	progress_num->BlitElement();

}