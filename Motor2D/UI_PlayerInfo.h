#ifndef __UI_PLAYERINFO__
#define __UI_PLAYERINFO__

#include "UI_element.h"

class Image;

class PlayerInfo : public UI_element
{
public:

	PlayerInfo(int x, int y, j1Module* callback);
	~PlayerInfo();

	void BlitElement();

public:

	Image* coin_socket = nullptr;
	Image* coin = nullptr;
	Image* live = nullptr;
	Image* god_live = nullptr;
	Image* empty_live = nullptr;
	Text* score = nullptr;
};

#endif // !__UI_PLAYERINFO__