#ifndef __PATH_FINDING_H__
#define __PATH_FINDING_H__

#include "j1Module.h"
#include "p2Point.h"
#include "p2List.h"
#include "j1Entity.h"
#include "p2DynArray.h"
#include "p2PQueue.h"

#define MAP_SIZE 100

class j1PathFinding : public j1Module
{
public:

	j1PathFinding()
	{}
	~j1PathFinding()
	{}
	
	bool getPath(Entity* entity, Entity* objective, p2DynArray<iPoint>& path_to_fill);
	void SetMap(uint width, uint height, uchar* data);
	bool isWalkable(const iPoint& coords) const;
	void ResetPath(p2DynArray<iPoint>& path_to_reset);
	bool isTouchingGround(iPoint coords) const;

private:

	uint width = 0, height = 0;
	uchar* map = nullptr;

	p2PQueue<iPoint>	frontier;
	p2List<iPoint>		visited;
	p2List<iPoint>		breadcrumbs;
	uint				cost_so_far[MAP_SIZE][MAP_SIZE];
	uint				falling_value[MAP_SIZE][MAP_SIZE];
};

#endif // !__PATH_FINDING_H__
