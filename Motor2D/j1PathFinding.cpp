#include "j1PathFinding.h"
#include "j1Map.h"
#include "j1App.h"
#include "p2Defs.h"
#include "j1Collision.h"

void j1PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width*height];
	memcpy(map, data, width*height);
}

bool j1PathFinding::isWalkable(const iPoint& coords) const
{
	bool ret = false;

	int position = (coords.y * width) + coords.x;
	if (position <= width*height && map[position] == 1 && coords.x >= 0 && coords.y >= 0)
		ret = true;

	return ret;
}

void j1PathFinding::ResetPath(p2DynArray<iPoint>& path_to_reset)
{
	path_to_reset.Clear();
	frontier.Clear();
	visited.clear();
	breadcrumbs.clear();
	memset(cost_so_far, 0, sizeof(uint) * 65 * 13);
}

void j1PathFinding::getPath(Entity* entity, Entity* objective, p2DynArray<iPoint>& path_to_fill)
{
	ResetPath(path_to_fill);

	iPoint origin_coords = App->map->WorldToMap(entity->position.x + entity->collider_offset.x + entity->collider->rect.w/2, entity->position.y + entity->collider_offset.y + entity->collider->rect.h/2);
	frontier.Push(origin_coords, 0);
	visited.add(origin_coords);
	breadcrumbs.add(origin_coords);
	iPoint destination_coords = App->map->WorldToMap(objective->position.x + objective->collider_offset.x + objective->collider->rect.w / 2, objective->position.y + objective->collider_offset.y + objective->collider->rect.h / 2);
	
	if (isWalkable(destination_coords) && isWalkable(origin_coords))
	{	
		while (visited.find(destination_coords) == -1)
		{
			iPoint curr;
			if (frontier.Pop(curr))
			{
				iPoint neighbors[4];
				neighbors[0].create(curr.x + 1, curr.y + 0);
				neighbors[1].create(curr.x + 0, curr.y + 1);
				neighbors[2].create(curr.x - 1, curr.y + 0);
				neighbors[3].create(curr.x + 0, curr.y - 1);

				for (uint i = 0; i < 4; ++i)
				{
					int distance = neighbors[i].DistanceNoSqrt(destination_coords);
					int d2 = pow(neighbors[i].x - destination_coords.x, 2.0) + pow(neighbors[i].y - destination_coords.y, 2.0);

					int newCost = cost_so_far[curr.x][curr.y] + 1;
					if (cost_so_far[neighbors[i].x][neighbors[i].y] == NULL || newCost < cost_so_far[neighbors[i].x][neighbors[i].y])
					{
						if (isWalkable(neighbors[i]))
						{
							cost_so_far[neighbors[i].x][neighbors[i].y] = newCost;

							if (visited.find(neighbors[i]) == -1)
							{
								breadcrumbs.add(curr);
								visited.add(neighbors[i]);
								if (neighbors[i] != destination_coords)
								{
									frontier.Push(neighbors[i], newCost + (distance * 10));
								}
							}
							else
							{
								breadcrumbs.At(visited.find(neighbors[i]))->data = curr;
							}
						}
					}
				}
			}
		}

		if (visited.find(destination_coords) != -1)
		{
			path_to_fill.PushBack(destination_coords);
			iPoint cameFrom = breadcrumbs.At(visited.find(destination_coords))->data;
			path_to_fill.PushBack(cameFrom);
			while (cameFrom != visited.start->data)
			{
				cameFrom = breadcrumbs.At(visited.find(cameFrom))->data;
				path_to_fill.PushBack(cameFrom);
			}
		}
		path_to_fill.Flip();
	}
}