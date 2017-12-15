#include "UI_PlayerInfo.h"
#include "j1Gui.h"
#include "j1Render.h"
#include "p2DynArray.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "UI_Image.h"
#include "UI_Text.h"
#include "j1Player.h"
#include "j1EntityManager.h"

PlayerInfo::PlayerInfo(int x, int y, j1Module* callback): UI_element(x, y, PLAYERINFO, {0,0,0,0}, callback)
{
	_TTF_Font* mid_texts_font = App->font->Load("fonts/TCCEB.ttf", 36);
	SDL_Color white_color = { 255, 255, 255, 0 };
	coin_socket = App->gui->createImageFromAtlas(x + 160, y + 2, { 948, 189, 48, 46 });
	j1Player* player = (j1Player*)App->entityManager->getPlayer();
	player->coins_pos[0] = { x + 160, y + 2 };
	player->coins_pos[1] = { x + 185, y + 2 };
	player->coins_pos[2] = { x + 210, y + 2 };
	coin = App->gui->createImageFromAtlas(x + 161, y + 3, { 895, 189, 41, 41 });
	live = App->gui->createImageFromAtlas(x + 62, y + 3, {380, 449, 51, 48});
	empty_live = App->gui->createImageFromAtlas(x + 62, y + 3, { 431, 449, 51, 48 });
}


PlayerInfo::~PlayerInfo()
{
}

void PlayerInfo::BlitElement()
{
	iPoint coin_socketPos = coin_socket->calculateAbsolutePosition();
	iPoint coinPos = coin->calculateAbsolutePosition();
	iPoint livePos = live->calculateAbsolutePosition();

	j1Player* player = (j1Player*)App->entityManager->getPlayer();

	for (int i = 0; i < 3; i++)
	{
		if (i < player->lives)
			App->render->Blit(live->texture, livePos.x + (i * 28), livePos.y, &live->section, false, App->gui->UI_scale);
		else
			App->render->Blit(empty_live->texture, livePos.x + (i * 28), livePos.y, &empty_live->section, false, App->gui->UI_scale);
	}
	
	for (int i = 0; i < 3; i++)
	{
		if (player->coins[i])
		{
			App->render->Blit(coin->texture, coinPos.x + (i * 25), coinPos.y, &coin->section, false, App->gui->UI_scale);
		}
		App->render->Blit(coin_socket->texture, coin_socketPos.x + (i * 25), coin_socketPos.y, &coin_socket->section, false, App->gui->UI_scale);
	}
}