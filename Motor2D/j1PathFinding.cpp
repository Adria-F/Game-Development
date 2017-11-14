#include "j1PathFinding.h"
#include "j1Map.h"
#include "j1App.h"

PathNode::PathNode() : g(-1), h(-1), coords(-1, -1), parent(NULL)
{}

PathNode::PathNode(int g, int h, const iPoint& pos, const PathNode* parent) : g(g), h(h), coords(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), F(node.F), coords(node.coords), parent(node.parent)
{}

uint PathNode::FindWalkableAdjacents(p2List<PathNode>& list_to_fill) const
{
	PathNode up(-1, -1, { coords.x, coords.y - 1 }, this);
	PathNode down(-1, -1, { coords.x, coords.y + 1 }, this); 
	PathNode left(-1, -1, { coords.x - 1, coords.y }, this);
	PathNode right(-1, -1, { coords.x + 1, coords.y }, this);
	
	if (App->pathfinding->isWalkable(up.coords))
		list_to_fill.add(up);

	if (App->pathfinding->isWalkable(down.coords))
		list_to_fill.add(down);

	if (App->pathfinding->isWalkable(left.coords))
		list_to_fill.add(left);

	if (App->pathfinding->isWalkable(right.coords))
		list_to_fill.add(right);
	
	return list_to_fill.count();
}

bool PathNode::touchingGround() const
{
	return !App->pathfinding->isWalkable({ this->coords.x, this->coords.y + 1 });
}

int PathNode::calculateF(const iPoint& destination)
{
	g = parent->g + 1;
	h = coords.DistanceTo(destination);

	return g + h;
}

p2List_item<PathNode>* PathList::Find(const iPoint& point) const
{
	p2List_item<PathNode>* item = list.start;
	while (item)
	{
		if (item->data.coords == point)
			return item;
		item = item->next;
	}
	return NULL;
}

p2List_item<PathNode>* PathList::FindLowestValue() const
{
	p2List_item<PathNode>* ret = NULL;
	PathNode min;
	min.F = 65535;

	p2List_item<PathNode>* item = list.end;
	while (item)
	{
		if (item->data.F < min.F)
		{
			min = item->data;
			ret = item;
		}
		if (item == list.start)
			break;
		item = item->prev;
	}

	return ret;
}

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
	if (position <= width*height && map[position] == 1)
		ret = true;

	return ret;
}

p2DynArray<iPoint> j1PathFinding::getPath(Entity* entity, const iPoint& destination) const
{
	p2DynArray<iPoint> path;

	PathList open, close;
	iPoint origin_coords = App->map->WorldToMap(entity->position.x, entity->position.y);
	iPoint destination_coords = App->map->WorldToMap(destination.x, destination.y);
	PathNode origin;
	origin.coords = origin_coords;
	open.list.add(origin);

	while (open.list.count() > 0)
	{
		p2List_item<PathNode>* lowestPathNode = open.FindLowestValue();
		close.list.add(lowestPathNode->data);
		
		if (lowestPathNode->data.coords == destination_coords)
		{
			const PathNode* curr = &close.list.end->data;
			while (curr != NULL)
			{
				path.PushBack(curr->coords);
				curr = curr->parent;
			}
			break;
		}

		p2List<PathNode> neighbors;
		lowestPathNode->data.FindWalkableAdjacents(neighbors);

		for (p2List_item<PathNode>* curr_neighbor = neighbors.start; curr_neighbor; curr_neighbor = curr_neighbor->next)
		{
			if (close.Find(curr_neighbor->data.coords) == NULL)
			{
				p2List_item<PathNode>* old_instance = open.Find(curr_neighbor->data.coords);
				curr_neighbor->data.F = curr_neighbor->data.calculateF(destination_coords);

				if (old_instance != NULL && curr_neighbor->data.g < old_instance->data.g)
				{
					*old_instance = *curr_neighbor;
				}

				else if (old_instance == NULL)
				{
					open.list.add(curr_neighbor->data);
				}			
			}
		}
		open.list.del(lowestPathNode);
	}

	return path;
}