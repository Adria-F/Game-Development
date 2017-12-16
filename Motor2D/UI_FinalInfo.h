#ifndef __UI_FINALINFO
#define __UI_FINALINFO

#include "UI_element.h"

class Image;

class FinalInfo : public UI_element
{
public:
	FinalInfo(int x, int y, j1Module* callback);
	~FinalInfo();

	void BlitElement();

public:

	Image* big_coin_socket = nullptr;
	Image* big_coin = nullptr;
	Image* no_trophy = nullptr;
	Image* brown_trophy = nullptr;
	Image* silver_trophy = nullptr;
	Image* gold_trophy = nullptr;
	Text* total_score = nullptr;
	Text* productxTrophy = nullptr;

	bool score_multiplicated = false;
};


#endif // !__UI_FINALINFO
