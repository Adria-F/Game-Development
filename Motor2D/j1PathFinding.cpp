#include "j1PathFinding.h"
#include "j1App.h"

PathNode::PathNode() : g(-1), h(-1), jump_value(0), coords(-1, -1), parent(NULL)
{}

PathNode::PathNode(int g, int h, int jump_value, const iPoint& pos, const PathNode* parent) : g(g), h(h), jump_value(jump_value), coords(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), jump_value(node.jump_value), coords(node.coords), parent(node.parent)
{}

uint PathNode::FindWalkableAdjacents(PathList& list_to_fill, const uint& max_jump_value) const
{
	iPoint cell;

	if (jump_value == 0 || jump_value % 2 != 0)
	{
		if (jump_value < max_jump_value)
		{
			//Cell up
			cell.create(coords.x, coords.y - 1);
			if (App->pathfinding->isWalkable(cell))
				list_to_fill.list.add(PathNode(-1, -1, -1, cell, this));
		}
		//Cel down
		cell.create(coords.x, coords.y + 1);
		if (App->pathfinding->isWalkable(cell))
			list_to_fill.list.add(PathNode(-1, -1, -1, cell, this));
	}
	if (jump_value == 0 || jump_value % 2 == 0)
	{
		//Cell right
		cell.create(coords.x + 1, coords.y);
		if (App->pathfinding->isWalkable(cell))
			list_to_fill.list.add(PathNode(-1, -1, -1, cell, this));
		//Cell left
		cell.create(coords.x - 1, coords.y);
		if (App->pathfinding->isWalkable(cell))
			list_to_fill.list.add(PathNode(-1, -1, -1, cell, this));
	}

	return list_to_fill.list.count();
}

bool PathNode::touchingGround() const
{
	return !App->pathfinding->isWalkable({ this->coords.x, this->coords.y + 1 });
}

void PathNode::calculateJumpValue(const uint& max_jump_value, bool flying)
{
	if (this->touchingGround() || flying)
		jump_value = 0;
	else
	{
		if(parent->coords.x == coords.x) //Moved vertically
		{
			if (parent->coords.y > coords.y) //Moved up
			{
				if (parent->touchingGround())
				{
					jump_value = 3;
				}
				else
				{
					if (parent->jump_value % 2 != 0)
						jump_value = parent->jump_value + 1;
					else
						jump_value = parent->jump_value + 2;
				}
			}
			else if (parent->coords.y < coords.y) //Moved down
			{
				if (parent->jump_value < max_jump_value)
				{
					jump_value = max_jump_value;
				}
				else
				{
					if (parent->jump_value % 2 != 0)
						jump_value = parent->jump_value + 1;
					else
						jump_value = parent->jump_value + 2;
				}
			}
		}
		else if(parent->coords.y == coords.y) //Moved horizontally
		{ 
			jump_value = parent->jump_value + 1;
		}
	}
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
		if (item->data.F == min.F)
		{
			if (item->data.jump_value < min.jump_value)
			{
				min = item->data;
			}
		}
		else if (item->data.F < min.F)
		{
			min = item->data;
			ret = item;
		}
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

PathList j1PathFinding::getPath(Entity* entity, const iPoint& destination) const
{
	PathList path;	
	////add origin to open list
	//PathNode item;
	//p2List_item <PathNode*>* open_list;
	//for (open_list = path.list.add.start(entity->position); open_list; open_list = open_list->next)
	//{
	//	item.FindWalkableAdjacents(path, 6);
	//	path.FindLowestValue();
	//}
	//// LOOP (pop the node with lowest score from open list)
	////fill a patList with walkable adjacent neighbors
	////calculate the values of neighbors and add them to open list
	////repeat loop

	////max_jump_value has to be entity max jumpable cells * 2

	return path;
}