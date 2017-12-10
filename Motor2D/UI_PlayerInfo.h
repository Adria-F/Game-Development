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

	Image* coin_socket;
	Image* coin;
	Image* live;
	Text* score;
};

#endif // !__UI_PLAYERINFO__