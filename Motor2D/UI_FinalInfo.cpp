#include "UI_FinalInfo.h"
#include "j1Fonts.h"
#include "j1Player.h"
#include "j1Gui.h"
#include "j1EntityManager.h"
#include "j1Render.h"
#include "UI_Image.h"
#include "UI_Text.h"
#include "j1UIScene.h"
#include "UI_ProgressBar.h"
#include "UI_Chrono.h"

FinalInfo::FinalInfo(int x, int y, j1Module* callback) : UI_element(x, y, FINALINFO, { 0,0,0,0 }, callback)
{
	_TTF_Font* big_texts_font = App->font->Load("fonts/TCCEB.ttf", 55);
	SDL_Color white_color = { 255, 255, 255, 0 };
	big_coin_socket = App->gui->createImageFromAtlas(x + 457 * App->gui->UI_scale, y + 237 * App->gui->UI_scale, { 160, 457, 101, 98 });
	big_coin = App->gui->createImageFromAtlas(x + 461 * App->gui->UI_scale, y + 242 * App->gui->UI_scale, { 11, 457, 82, 82 });
	final_score = App->gui->createText("0", x + 515 * App->gui->UI_scale, y + 377 * App->gui->UI_scale, big_texts_font, white_color);
	no_trophy =  App->gui->createImageFromAtlas(x + 265 * App->gui->UI_scale, y + 226 * App->gui->UI_scale, { 336, 701, 110, 120 });
	brown_trophy = App->gui->createImageFromAtlas(x + 265 * App->gui->UI_scale, y + 226 * App->gui->UI_scale, { 6, 701, 110, 120 });
	silver_trophy = App->gui->createImageFromAtlas(x + 265 * App->gui->UI_scale, y + 226 * App->gui->UI_scale, { 116, 701, 110, 120 });
	gold_trophy = App->gui->createImageFromAtlas(x + 265 * App->gui->UI_scale, y + 226 * App->gui->UI_scale, { 226, 701, 110, 120 });
}


FinalInfo::~FinalInfo()
{}

void FinalInfo::BlitElement()
{
	iPoint big_coin_socketPos = big_coin_socket->calculateAbsolutePosition();
	iPoint big_coinPos = big_coin->calculateAbsolutePosition();
	iPoint trophyPos = no_trophy->calculateAbsolutePosition();//Same pos for all the trophys because it has to be only 1.

	j1Player* player = (j1Player*)App->entityManager->getPlayer();
	p2SString score_str("%d", player->score);
	final_score->setText(score_str);
	final_score->setOutlined(true);
	final_score->BlitElement();

	//NEEDS TO CHOOSE THE TROPHY:
	if (player->trophy == 3)
		App->render->Blit(gold_trophy->texture, trophyPos.x, trophyPos.y, &gold_trophy->section, false, App->gui->UI_scale);

	else if (player->trophy == 2)
		App->render->Blit(silver_trophy->texture, trophyPos.x, trophyPos.y, &silver_trophy->section, false, App->gui->UI_scale);

	else if (player->trophy == 1)
		App->render->Blit(brown_trophy->texture, trophyPos.x, trophyPos.y, &brown_trophy->section, false, App->gui->UI_scale);

	else
		App->render->Blit(no_trophy->texture, trophyPos.x, trophyPos.y, &no_trophy->section, false, App->gui->UI_scale);

	for (int i = 0; i < 3; i++)
	{
		if (player->coins[i])
		{
			App->render->Blit(big_coin->texture, big_coinPos.x + (i * 50 ), big_coinPos.y, &big_coin->section, false, App->gui->UI_scale);
		}
		App->render->Blit(big_coin_socket->texture, big_coin_socketPos.x + (i * 50), big_coin_socketPos.y, &big_coin_socket->section, false, App->gui->UI_scale);
	}
}