#ifndef __UI_FINALINFO
#define __UI_FINALINFO

#include "UI_element.h"

class Image;

class FinalInfo : public UI_element
{
public:
	FinalInfo(int x, int y, j1Module* callback);
	~FinalInfo();
};


#endif // !__UI_FINALINFO
