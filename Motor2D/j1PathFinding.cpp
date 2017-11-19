#include "j1PathFinding.h"
#include "j1Map.h"
#include "j1App.h"
#include "p2Defs.h"
#include "j1Collision.h"
#include "Brofiler\Brofiler.h"

void j1PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width*height];
	memcpy(map, data, width*height);
}

bool j1PathFinding::isTouchingGround(iPoint coords) const
{
	return !isWalkable({ coords.x, coords.y + 1 });
}

bool j1PathFinding::isWalkable(const iPoint& coords) const
{
	bool ret = false;

	int position = (coords.y * width) + coords.x;
	if (coords.x < width && map[position] == 1 && coords.x >= 0 && coords.y >= 0 && coords.y < height-1)
		ret = true;

	return ret;
}

void j1PathFinding::ResetPath(p2DynArray<iPoint>& path_to_reset)
{
	path_to_reset.Clear();
	frontier.Clear();
	visited.clear();
	breadcrumbs.clear();
	memset(cost_so_far, 0, sizeof(uint) * MAP_SIZE * MAP_SIZE);
	memset(falling_value, 0, sizeof(uint) * MAP_SIZE * MAP_SIZE);
}

bool j1PathFinding::getPath(Entity* entity, Entity* objective, p2DynArray<iPoint>& path_to_fill)
{
	BROFILER_CATEGORY("PathFinding algorithm", Profiler::Color::Black);
	bool ret = false;
	
	ResetPath(path_to_fill);

	//Define origin and destination coords
	iPoint origin_coords = App->map->WorldToMap(entity->position.x + entity->collider_offset.x + entity->collider->rect.w/2, entity->position.y + entity->collider_offset.y + entity->collider->rect.h/2);
	frontier.Push(origin_coords, 0);
	visited.add(origin_coords);
	breadcrumbs.add(origin_coords);
	iPoint destination_coords = App->map->WorldToMap(objective->position.x + objective->collider_offset.x + objective->collider->rect.w / 2, objective->position.y + objective->collider_offset.y + objective->collider->rect.h / 2);
	
	if (isWalkable(destination_coords) && isWalkable(origin_coords) && (entity->flying || (origin_coords.y <= destination_coords.y)))
	{	
		while (visited.find(destination_coords) == -1)
		{
			iPoint curr;
			if (frontier.Pop(curr))
			{

				iPoint neighbors[4];
				neighbors[0].create(curr.x + 0, curr.y + 1); //Down
				neighbors[1].create(curr.x - 1, curr.y + 0); //Left
				neighbors[2].create(curr.x + 1, curr.y + 0); //Right
				neighbors[3].create(curr.x + 0, curr.y - 1); //Up

				//Define which neighbors to check
				int max_neighbors = 4;
				if (!entity->flying)
				{
					max_neighbors = 3; //Don't check up
					if (!isTouchingGround(curr) && (falling_value[curr.x][curr.y] == 0 || falling_value[curr.x][curr.y] % 2 != 0))
						max_neighbors = 1; //Just check down
				}

				for (uint i = 0; i < max_neighbors; ++i)
				{
					if (!isWalkable(neighbors[i])) //If not walkable ignore
						continue;

					int distance = neighbors[i].DistanceNoSqrt(destination_coords); //h

					int newCost = cost_so_far[curr.x][curr.y] + 1; //g
					if (cost_so_far[neighbors[i].x][neighbors[i].y] == NULL || newCost < cost_so_far[neighbors[i].x][neighbors[i].y])
					{
						cost_so_far[neighbors[i].x][neighbors[i].y] = newCost;
						
						//Set falling value
						if (curr.x == neighbors[i].x)
							falling_value[neighbors[i].x][neighbors[i].y] = falling_value[curr.x][curr.y] + (falling_value[curr.x][curr.y] % 2 == 0) ? 2 : 1;
						else if (curr.y == neighbors[i].y)
							falling_value[neighbors[i].x][neighbors[i].y] = falling_value[curr.x][curr.y] + 1;
						if (isTouchingGround({ neighbors[i].x , neighbors[i].y }))
							falling_value[neighbors[i].x][neighbors[i].y] = 0;

						//Add to visited or update breadcrumb
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
			else
				break;
		}

		//Calculate final path
		if (visited.find(destination_coords) != -1)
		{
			ret = true;
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

	return ret;
}