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
	Image* trophy = nullptr;
	Text* final_score = nullptr;
};


#endif // !__UI_FINALINFO
