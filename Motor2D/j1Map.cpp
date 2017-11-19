#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Collision.h"
#include "j1Window.h"
#include "j1PathFinding.h"
#include "j1EntityManager.h"
#include <math.h>
#include "Brofiler\Brofiler.h"

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

void j1Map::Draw()
{
	if(map_loaded == false)
		return;

	//Draw all image layers
	p2List_item<ImageLayer*>* image = nullptr;
	for (image = data.image_layers.start; image; image = image->next)
	{
		SDL_Texture* texture = image->data->texture;
		SDL_Rect section = { 0, 0, image->data->width, image->data->height };
		if (image->data->position.x < -image->data->width)
		{
			image->data->position.x = image->data->width;
		}
		App->render->Blit(texture, image->data->position.x, image->data->position.y, &section);
	}

	p2List_item<MapLayer*>* item = nullptr;
	MapLayer* layer = nullptr;
	uint tile_id;
	p2List_item<TileSet*>* tileSet = nullptr;
	for (item = data.layers.start; item; item = item->next)
	{
		layer = item->data;
		
		//Each layer
		for (int j = 0; j < data.height; j++)
		{
			for (int i = 0; i < data.width; i++)
			{
				tile_id = layer->tiles[layer->Get(i, j)];
				if (tile_id != 0)
				{
					for (tileSet = data.tilesets.start; tileSet; tileSet = tileSet->next)
					{
						if (tile_id >= tileSet->data->firstgid && ((!tileSet->next) || (tileSet->next && tile_id < tileSet->next->data->firstgid)))
						{
							break;
						}
					}
					
					SDL_Texture* texture = tileSet->data->texture;
					iPoint position = MapToWorld(i, j);
					SDL_Rect* section = &tileSet->data->GetTileRect(tile_id);
					App->render->Blit(texture, position.x, position.y, section);
				}
			}
		}
	}
}


iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	ret.x = x * data.tile_width;
	ret.y = y * data.tile_height;

	return ret;
}

iPoint j1Map::WorldToMap(int x, int y) const
{
	iPoint ret;

	ret.x = x / data.tile_width;
	ret.y = y / data.tile_height;

	return ret;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	p2List_item<ImageLayer*>* item_imageLayer;
	item_imageLayer = data.image_layers.start;

	while (item_imageLayer != NULL)
	{
		RELEASE(item_imageLayer->data);
		item_imageLayer = item_imageLayer->next;
	}
	data.image_layers.clear();

	p2List_item<MapLayer*>* item_layer;
	item_layer = data.layers.start;

	while (item_layer != NULL)
	{
		RELEASE(item_layer->data);
		item_layer = item_layer->next;
	}
	data.layers.clear();

	//Clean colliders
	p2List_item<Collider*>* item_collider;
	item_collider = data.colliders.start;

	while (item_collider != NULL)
	{
		item_collider->data->to_delete = true;		
		item_collider = item_collider->next;
	}
	data.colliders.clear();

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name, int& map_length, SDL_Rect& end, bool fromSaveData)
{
	bool ret = true;
	BROFILER_CATEGORY("Load Map", Profiler::Color::Pink);
	//Clean previous map before loading another one
	CleanUp();
	if (!fromSaveData && App->entityManager->entities.start->next != nullptr)
		App->entityManager->CleanUp(); //Clean entities

	// Restart player data
	App->entityManager->getPlayer()->Start();

	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}

	pugi::xml_node image_layer;
	for (image_layer = map_file.child("map").child("imagelayer"); image_layer && ret; image_layer = image_layer.next_sibling("imagelayer"))
	{
		ImageLayer* set = new ImageLayer();

		if (ret == true)
		{
			ret = LoadImageLayer(image_layer, set);
		}
		ImageLayer* set2 = new ImageLayer(*set);
		data.image_layers.add(set);
		set2->position.x += set2->width;
		data.image_layers.add(set2);
	}

	// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for (layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		p2SString name = layer.attribute("name").as_string();
		if (name == "Walkability")
		{
			int w, h;
			uchar* data = NULL;
			if (LoadWalkabilityMap(layer, w, h, &data))
				App->pathfinding->SetMap(w, h, data);
		}
		else
		{
			MapLayer* set = new MapLayer();

			if (ret == true)
			{
				ret = LoadLayer(layer, set);
			}

			data.layers.add(set);
		}
	}

	pugi::xml_node object;
	p2SString object_name;
	for (object = map_file.child("map").child("objectgroup"); object && ret; object = object.next_sibling("objectgroup"))
	{
		object_name = object.attribute("name").as_string();
		if (object_name == "Collision")
		{
			LoadColliders(object);
		}
		else if (object_name == "Logic")
		{
			LoadLogic(object, map_length, end, fromSaveData);
		}
	}

	// Log info ----------------------
	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while(item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

		p2List_item<MapLayer*>* item_layer = data.layers.start;
		while(item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}
	}

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

bool j1Map::LoadImageLayer(pugi::xml_node& node, ImageLayer* set)
{
	bool ret = true;

	set->name = node.attribute("name").as_string();
	set->position.x = set->offset_x = node.attribute("offsetx").as_int();
	set->position.y = set->offset_y = node.attribute("offsety").as_int();

	pugi::xml_node image = node.child("image");
	set->width = image.attribute("width").as_int();
	set->height = image.attribute("height").as_int();
	set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));

	pugi::xml_node property;
	for (property = node.child("properties").child("property"); property; property = property.next_sibling("property"))
	{
		p2SString name = property.attribute("name").as_string();
		if (name == "speed")
		{
			set->speed = property.attribute("value").as_float();
		}
		if (name == "constant_movement")
		{
			set->constant_movement = property.attribute("value").as_bool();
		}
	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();
	layer->size = (layer->width * layer->height);

	layer->tiles = new uint[layer->size];
	memset(layer->tiles, 0, sizeof(uint) * layer->size);

	pugi::xml_node tile;
	int i = 0;
	for (tile = node.child("data").child("tile"); tile; tile = tile.next_sibling("tile"))
	{
		layer->tiles[i] = tile.attribute("gid").as_uint();
		i++;
	}

	return ret;
}

bool j1Map::LoadColliders(pugi::xml_node& node)
{
	bool ret = true;

	pugi::xml_node object;
	COLLIDER_TYPE collider_type;
	p2SString type;
	for (object = node.child("object"); object; object = object.next_sibling("object"))
	{
		type = object.attribute("type").as_string();
		if (type == "floor")
		{
			collider_type = COLLIDER_FLOOR;
		}
		else if (type == "jumpable")
		{
			collider_type = COLLIDER_JUMPABLE;
		}
		else
		{
			LOG("Collider type undefined");
			continue;
		}

		SDL_Rect shape;
		shape.x = object.attribute("x").as_int();
		shape.y = object.attribute("y").as_int();
		shape.w = object.attribute("width").as_int();
		shape.h = object.attribute("height").as_int();

		data.colliders.add(App->collision->AddCollider(shape, collider_type));
	}

	return ret;
}

bool j1Map::LoadLogic(pugi::xml_node& node, int& map_length, SDL_Rect& end, bool fromSaveData)
{
	bool ret = true;

	pugi::xml_node object;
	p2SString name;
	p2SString type;
	for (object = node.child("object"); object; object = object.next_sibling("object"))
	{
		name = object.attribute("name").as_string();
		type = object.attribute("type").as_string();
		if (name == "player_start_pos")
		{
			App->entityManager->getPlayer()->virtualPosition.x = App->entityManager->getPlayer()->position.x = object.attribute("x").as_int();
			App->entityManager->getPlayer()->virtualPosition.y = App->entityManager->getPlayer()->position.y = object.attribute("y").as_int();

			App->render->virtualCamPos = -(App->entityManager->getPlayer()->position.x * (int)App->win->GetScale() - 100);
			if (App->render->virtualCamPos > 0)
			{
				App->render->virtualCamPos = 0;
			}
		}
		if (name == "end")
		{
			end.x = object.attribute("x").as_int();
			end.y = object.attribute("y").as_int();
			end.w = object.attribute("width").as_int();
			end.h = object.attribute("height").as_int();
		}
		if (type == "enemy" && !fromSaveData)
		{
			int x, y;
			x = object.attribute("x").as_int();
			y = object.attribute("y").as_int();
			if (name == "charger")
			{
				App->entityManager->createEntity(CHARGER, x, y);
			}
			else if (name == "bat")
			{
				App->entityManager->createEntity(BAT, x, y);
			}
		}
	}

	pugi::xml_node property;
	for (property = node.child("properties").child("property"); property; property = property.next_sibling("property"))
	{
		p2SString name = property.attribute("name").as_string();
		if (name == "map_length")
		{
			map_length = property.attribute("value").as_int();
		}
	}

	return ret;
}

bool j1Map::LoadWalkabilityMap(pugi::xml_node& node, int & width, int & height, uchar ** buffer) const
{
	
	width = node.attribute("width").as_int();
	height = node.attribute("height").as_int();

	uchar* map = new uchar[width*height];
	memset(map, 1, width*height);

	int i = 0;
	for (pugi::xml_node tile = node.child("data").child("tile"); tile; tile = tile.next_sibling("tile"))
	{
		int tile_value = tile.attribute("gid").as_int();
		map[i] = (tile_value > 0) ? 0 : 1;
		i++;
	}
	
	*buffer = map;

	return true;
}
