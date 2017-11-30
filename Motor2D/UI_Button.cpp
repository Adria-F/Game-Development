#include "UI_Button.h"
#include "j1App.h"
#include "j1Render.h"

void Button::setOutlined(bool isOutlined)
{
	if (text != nullptr)
		text->setOutlined(isOutlined);
}

void Button::appendChild(int x, int y, UI_element * child)
{
	child->localPosition = { x, y };
	child->parent = this;
	text = (Text*)child;
}

void Button::appendChildAtCenter(UI_element * child)
{
	iPoint child_pos(section.w / 2, section.h / 2);
	child_pos.x -= child->section.w / 2;
	child_pos.y -= child->section.h / 2;
	child->localPosition = { child_pos.x, child_pos.y };
	child->parent = this;
	text = (Text*)child;
}

void Button::BlitElement()
{
	iPoint globalPos = calculateAbsolutePosition();
	switch (state)
	{
	case STANDBY:
		App->render->Blit(texture, globalPos.x, globalPos.y, &section, false, 0.5f);
		break;
	case MOUSEOVER:
		App->render->Blit(texture, globalPos.x, globalPos.y, &OnMouse, false, 0.5f);
		break;
	case CLICKED:
		App->render->Blit(texture, globalPos.x, globalPos.y, &OnClick, false, 0.5f);
		break;
	}

	if (type == CHECKBOX && tick)
	{
		App->render->Blit(texture, globalPos.x, globalPos.y, &Tick, false, 0.5f);
	}
	else if (type == LINK && text != nullptr)
	{		
		text->BlitElement();
	}
}
