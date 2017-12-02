#include "UI_Text.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Render.h"

Text::~Text()
{
	if (outline != nullptr)
	{
		App->tex->UnLoad(outline);
		outline = nullptr;
	}
}

void Text::createTexture()
{
	if (texture != nullptr)
	{
		App->tex->UnLoad(texture);
		texture = nullptr;
	}
	if (outline != nullptr)
	{
		App->tex->UnLoad(outline);
		outline = nullptr;
	}

	uint outline_width, outline_height;
	if (outlined)
	{
		App->font->setFontOutline(font, 2);
		outline = App->font->Print(text.GetString(), outline_color, font); //Outlined texture
		App->tex->GetSize(outline, outline_width, outline_height);
	}

	App->font->setFontOutline(font, 0);
	texture = App->font->Print(text.GetString(), color, font); //Normal texture
	App->tex->GetSize(texture, tex_width, tex_height);
	section.w = tex_width;
	section.h = tex_height;

	if (outlined)
	{
		outline_offset.x = tex_width - outline_width;
		outline_offset.x /= 4;
		outline_offset.y = outline_offset.x;
	}

}

void Text::setColor(SDL_Color newColor)
{
	color = newColor;
	createTexture();
}

void Text::setOutlineColor(SDL_Color newColor)
{
	outline_color = newColor;
	if (outlined)
		createTexture();
}

void Text::BlitElement()
{
	iPoint globalPos = calculateAbsolutePosition();

	if (outlined)
		App->render->Blit(outline, globalPos.x + outline_offset.x, globalPos.y + outline_offset.y, NULL, false, App->gui->UI_scale);
	App->render->Blit(texture, globalPos.x, globalPos.y, NULL, false, App->gui->UI_scale);
}

void Text::setOutlined(bool isOutlined)
{
	if (isOutlined != outlined)
	{
		outlined = isOutlined;
		createTexture();
	}
}

p2SString Text::getText() const
{
	return text;
}

void Text::setText(const char * string)
{
	text = string;
	createTexture();
}

void Text::setText(const p2SString string)
{
	text = string;
	createTexture();
}

int Text::getLength() const
{
	return text.Length();
}
