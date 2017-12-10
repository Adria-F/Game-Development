#include "UI_PlayerInfo.h"
#include "j1Gui.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "UI_element.h"
#include "UI_Image.h"
#include "j1Player.h"
#include "j1EntityManager.h"

PlayerInfo::PlayerInfo(int x, int y, j1Module* callback): UI_element(x, y, PLAYERINFO, {0,0,0,0}, callback)
{
	coin_socket = App->gui->createImageFromAtlas(x + 150, y + 2, { 948, 189, 48, 46 });
	coin = App->gui->createImageFromAtlas(x + 152, y + 3, { 895, 189, 41, 41 });
	live = App->gui->createImageFromAtlas(x + 60, y + 3, {900, 240, 19, 33});
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

	for (int i = 0; i < player->lives; i++)
		App->render->Blit(live->texture, livePos.x + (i * 20), livePos.y, &live->section, false, App->gui->UI_scale);
	
	for (int i = 0; i < 3; i++)
	{
		if (player->coins[i])
		{
			App->render->Blit(coin->texture, coinPos.x + (i * 25), coinPos.y, &coin->section, false, App->gui->UI_scale);
		}
		App->render->Blit(coin_socket->texture, coin_socketPos.x + (i * 25), coin_socketPos.y, &coin_socket->section, false, App->gui->UI_scale);
	}
}