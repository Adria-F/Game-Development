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
	coin_socket = App->gui->createImageFromAtlas(x+150, y + 2, { 948, 189, 48, 46 });
	coin = App->gui->createImageFromAtlas(x+151, y+3, { 895, 189, 41, 41 });
}


PlayerInfo::~PlayerInfo()
{
}

void PlayerInfo::BlitElement()
{
	iPoint coin_socketPos = coin_socket->calculateAbsolutePosition();
	iPoint coinPos = coin->calculateAbsolutePosition();

	j1Player* player = (j1Player*)App->entityManager->getPlayer();

	for (int i = 0; i < 3; i++)
	{
		if (player->coins[i])
		{
			App->render->Blit(coin->texture, coinPos.x + (i * 25), coinPos.y, &coin->section, false, App->gui->UI_scale);
		}
		App->render->Blit(coin_socket->texture, coin_socketPos.x + (i * 25), coin_socketPos.y, &coin_socket->section, false, App->gui->UI_scale);
	}
}