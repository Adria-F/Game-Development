#include "UI_FinalInfo.h"
#include "j1Fonts.h"
#include "j1Player.h"
#include "j1Gui.h"
#include "j1EntityManager.h"
#include "j1Render.h"
#include "UI_Image.h"
#include "UI_Text.h"

FinalInfo::FinalInfo(int x, int y, j1Module* callback) : UI_element(x, y, FINALINFO, { 0,0,0,0 }, callback)
{
	_TTF_Font* big_texts_font = App->font->Load("fonts/TCCEB.ttf", 55);
	SDL_Color white_color = { 255, 255, 255, 0 };
	big_coin_socket = App->gui->createImageFromAtlas(x + 160, y + 2, { 160, 457, 101, 98 });
	big_coin = App->gui->createImageFromAtlas(x + 457, y + 237, { 11, 457, 82, 82 });
	final_score = App->gui->createText("0", x + 275, y + 3, big_texts_font, white_color);
}


FinalInfo::~FinalInfo()
{}

void FinalInfo::BlitElement()
{
	iPoint big_coin_socketPos = big_coin_socket->calculateAbsolutePosition();
	iPoint big_coinPos = big_coin->calculateAbsolutePosition();

	j1Player* player = (j1Player*)App->entityManager->getPlayer();

	for (int i = 0; i < 3; i++)
	{
		if (player->coins[i])
		{
			App->render->Blit(big_coin->texture, big_coinPos.x + (i * 93), big_coinPos.y, &big_coin->section, false, App->gui->UI_scale);
		}
		App->render->Blit(big_coin_socket->texture, big_coin_socketPos.x + (i * 93), big_coin_socketPos.y, &big_coin_socket->section, false, App->gui->UI_scale);
	}
}