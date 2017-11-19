#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "j1App.h"
#include "j1Textures.h"

struct MapLayer
{
	p2SString name;
	int width;
	int height;
	uint* tiles = nullptr;
	int size;
	inline uint Get(int x, int y) const
	{
		return (x + y * width);
	}

	~MapLayer()
	{
		if (tiles != NULL)
		{
			delete[] tiles;
			tiles = NULL;
		}
	}

};

struct ImageLayer
{
	p2SString name;
	SDL_Texture* texture = nullptr;
	int offset_x;
	int offset_y;
	int width;
	int height;
	fPoint position;
	float speed = 0;
	bool constant_movement = false;

	ImageLayer()
	{}

	ImageLayer(const ImageLayer& copy)
	{
		name = copy.name;
		texture = copy.texture;
		offset_x = copy.offset_x;
		offset_y = copy.offset_y;
		width = copy.width;
		height = copy.height;
		position = copy.position;
		speed = copy.speed;
		constant_movement = copy.constant_movement;
	}

	~ImageLayer()
	{
		if (texture != nullptr)
		{
			App->tex->UnLoad(texture);
			texture = nullptr;
		}
	}

};

struct TileSet
{
	SDL_Rect GetTileRect(int id) const;

	p2SString			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture = nullptr;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;

	~TileSet()
	{
		if (texture != nullptr)
		{
			App->tex->UnLoad(texture);
			texture = nullptr;
		}
	}

};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	MapTypes			type;
	p2List<TileSet*>	tilesets;
	p2List<ImageLayer*> image_layers;
	p2List<MapLayer*>	layers;
	p2List<Collider*>	colliders;
};

class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path, int& map_length, SDL_Rect& end, bool fromSaveData = false);

	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadImageLayer(pugi::xml_node& node, ImageLayer* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadColliders(pugi::xml_node& node);
	bool LoadLogic(pugi::xml_node& node, int& map_length, SDL_Rect& end, bool fromSaveData = false);
	bool LoadWalkabilityMap(pugi::xml_node& node, int& width, int& height, uchar** buffer) const;

public:

	MapData data;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded = false;
};

#endif // __j1MAP_H__