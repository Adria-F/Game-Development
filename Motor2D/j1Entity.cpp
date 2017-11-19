#include "j1Entity.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Input.h"
#include "j1Audio.h"
#include "p2Log.h"
#include "j1EntityManager.h"
#include "j1Window.h"
#include "j1Textures.h"
#include "j1PathFinding.h"
#include "j1Player.h"
#include "Brofiler\Brofiler.h"

Entity::Entity(const char* name)
{
	v.x = 0;
	v.y = 0;
	this->name.create(name);

	p2SString path("animations/%s.tmx", name);
	LoadLogic(path.GetString());
	idle_right = LoadAnimation(path.GetString(), "idle_right");
	idle_left = LoadAnimation(path.GetString(), "idle_left");
	right = LoadAnimation(path.GetString(), "right");
	left = LoadAnimation(path.GetString(), "left");
	jumping_right = LoadAnimation(path.GetString(), "jumping_right");
	jumping_left = LoadAnimation(path.GetString(), "jumping_left");
	falling_right = LoadAnimation(path.GetString(), "falling_right");
	falling_left = LoadAnimation(path.GetString(), "falling_left");
	death = LoadAnimation(path.GetString(), "death");
}

Entity::~Entity()
{
	App->tex->UnLoad(graphics);
	graphics = nullptr;
	if (collider != nullptr)
	collider->to_delete = true;
}

bool Entity::Entity_Update(float dt)
{
	float oldVy = v.y;
	v.y += (gravity * ((colliding_bottom || flying) ? 0 : 1)) * dt; //*dt
	if (oldVy > 0 && v.y <= 0)
	{
		state = FALLING;
		jumping = false;
		going_down = true;
	}

	if (v.y < -jump_force)
		v.y = -jump_force;

	virtualPosition.y -= v.y * dt; 

	if (pos_relCam > 2 || v.x > 0 || type != PLAYER)
		virtualPosition.x += v.x * dt;

	colliding_right = false;
	colliding_left = false;

	if (collider != nullptr)
		collider->SetPos(virtualPosition.x + collider_offset.x, virtualPosition.y + collider_offset.y);

	position.x = virtualPosition.x;
	position.y = virtualPosition.y;

	int win_scale = App->win->GetScale();
	pos_relCam = position.x + App->render->camera.x / win_scale;

	setAnimation();

	prev_dt = dt;

	return true;
}

bool Entity::Calculate_Path()
{
	bool ret = false;

	if (!App->entityManager->getPlayer()->dead && position.DistanceTo(App->entityManager->getPlayer()->position) < DETECTION_MARGIN)
	{
		ret = App->pathfinding->getPath(this, App->entityManager->getPlayer(), entityPath);
	}
	else
	{
		App->pathfinding->ResetPath(entityPath);
	}

	if (!ret)
	{
		v.x = 0;
		if (this->type == BAT)
			v.y = 0;
		going_right = false;
		going_left = false;
		going_down = false;
		state = IDLE;
	}

	return ret;
}

void Entity::Entity_OnCollision(Collider* c1, Collider* c2)
{
	if (c2->type == COLLIDER_FLOOR)
	{
		if (Collision_from_bottom(c1, c2))
		{
			virtualPosition.y = c2->rect.y - collider->rect.h - collider_offset.y + 1;
			collidingFloor = c2;
			if (colliding_bottom == false)
			{
				v.y = 0;
				going_down = false;
				if (!going_left && !going_right || v.x == 0)
				{
					v.x = 0;
					state = IDLE;
				}
				else if (v.x > 0)
				{
					state = RIGHT;
				}
				else if (v.x < 0)
				{
					state = LEFT;
				}
				App->audio->PlayFx(landing_fx, 0);
				if (c1->type == COLLIDER_PLAYER && App->entityManager->player_god_mode)
				{
					if (App->entityManager->getPlayer()->old_savedCol != nullptr && App->entityManager->getPlayer()->old_savedCol->rect.x + App->entityManager->getPlayer()->old_savedCol->rect.w < c2->rect.x + c2->rect.w)
					{
						App->entityManager->getPlayer()->old_savedCol = c2;
						App->SaveGame(true);
					}
					else if (App->entityManager->getPlayer()->old_savedCol == nullptr)
						App->entityManager->getPlayer()->old_savedCol = c2;
				}
			}
			colliding_bottom = true;
		}
		else if (Collision_from_right(c1, c2))
		{
			if (v.x > 0)
			{
				v.x = 0;
			}
			colliding_right = true;
		}
		else if (Collision_from_left(c1, c2))
		{
			if (v.x < 0)
			{
				v.x = 0;
			}
			colliding_left = true;
		}
		if (Collision_from_top(c1, c2))
		{
			if (v.y > 0)
			{
				v.y = 0;
			}
		}
	}
	else if (c2->type == COLLIDER_JUMPABLE)
	{
		if (Collision_from_bottom(c1, c2))
		{
			virtualPosition.y = c2->rect.y - collider->rect.h - collider_offset.y + 1;
			collidingFloor = c2;
			if (colliding_bottom == false)
			{
				v.y = 0;
				going_down = false;
				if (!going_left && !going_right || v.x == 0)
				{
					v.x = 0;
					state = IDLE;
				}
				else if (v.x > 0)
				{
					state = RIGHT;
				}
				else if (v.x < 0)
				{
					state = LEFT;
				}
				App->audio->PlayFx(landing_fx, 0);
				if (c1->type == COLLIDER_PLAYER && App->entityManager->player_god_mode)
				{
					if (App->entityManager->getPlayer()->old_savedCol != nullptr && App->entityManager->getPlayer()->old_savedCol->rect.x + App->entityManager->getPlayer()->old_savedCol->rect.w < c2->rect.x + c2->rect.w)
					{
						App->entityManager->getPlayer()->old_savedCol = c2;
						App->SaveGame(true);
					}
					else if (App->entityManager->getPlayer()->old_savedCol == nullptr)
						App->entityManager->getPlayer()->old_savedCol = c2;
				}
			}
			colliding_bottom = true;
		}
	}
}

void Entity::setAnimation()
{
	if (specificAnimation())
	{ }
	else if (dead && death != nullptr)
	{
		animation = death;
	}

	else if (v.x > 0)
	{
		if (state == JUMPING && jumping_right != nullptr)
		{
			animation = jumping_right;
		}
		else if (state == FALLING && falling_right != nullptr)
		{
			animation = falling_right;
		}
		else if (right != nullptr)
		{
			animation = right;
		}
	}
	else if (v.x < 0)
	{
		if (state == JUMPING && jumping_left != nullptr)
		{
			animation = jumping_left;
		}
		else if (state == FALLING && falling_left != nullptr)
		{
			animation = falling_left;
		}
		else if (left != nullptr)
		{
			animation = left;
		}
	}
	else
	{
		if (state == IDLE)
		{
			if ((animation == left || animation == jumping_left || animation == falling_left) && idle_left != nullptr)
			{
				animation = idle_left;
			}
			else if ((animation == right || animation == jumping_right || animation == falling_right) && idle_right != nullptr)
			{
				animation = idle_right;
			}
		}
		else if (state == JUMPING)
		{
			if (animation == idle_left && jumping_left != nullptr)
			{
				animation = jumping_left;
			}
			else if (animation == idle_right && jumping_right != nullptr)
			{
				animation = jumping_right;
			}
		}
		else if (state == FALLING)
		{
			if (animation == jumping_left && falling_left != nullptr)
			{
				animation = falling_left;
			}
			else if (animation == jumping_right && falling_right != nullptr)
			{
				animation = falling_right;
			}
		}
		else if (state == LEFT && left != nullptr)
		{
			animation = left;
		}
		else if (state == RIGHT && right != nullptr)
		{
			animation = right;
		}
	}
}

Animation* Entity::LoadAnimation(const char* animationPath, const char* animationName)
{
	BROFILER_CATEGORY("Entity LoadAnimation", Profiler::Color::Red);
	Animation* ret = new Animation();

	bool animationFound = false;

	pugi::xml_document animation;
	pugi::xml_parse_result result = animation.load_file(animationPath);
	if (result == NULL)
	{
		LOG("Could not load animation xml file %s. pugi error: %s", animationName, result.description());
	}

	pugi::xml_node objectGroup;
	for (objectGroup = animation.child("map").child("objectgroup"); objectGroup; objectGroup = objectGroup.next_sibling("objectgroup"))
	{
		p2SString objectname = objectGroup.attribute("name").as_string();
		if (objectname == animationName)
		{
			animationFound = true;
			int x, y, h, w;
			float speed;
			bool loop;

			for (pugi::xml_node property = objectGroup.child("properties").child("property"); property; property = property.next_sibling("property"))
			{
				p2SString name = property.attribute("name").as_string();
				if (name == "speed")
				{
					speed = property.attribute("value").as_float();
					ret->speed = speed;
				}
				if (name == "loop")
				{
					loop = property.attribute("value").as_bool();
					ret->loop = loop;
				}
			}

			for (pugi::xml_node sprite = objectGroup.child("object"); sprite; sprite = sprite.next_sibling("object"))
			{
				x = sprite.attribute("x").as_int();
				y = sprite.attribute("y").as_int();
				w = sprite.attribute("width").as_int();
				h = sprite.attribute("height").as_int();

				ret->PushBack({ x, y, w, h });
			}
			break;
		}
	}

	if (animationFound)
		return ret;
	else
		return nullptr;
}

void Entity::LoadLogic(const char* animationPath)
{
	pugi::xml_document animation;
	pugi::xml_parse_result result = animation.load_file(animationPath);
	if (result == NULL)
	{
		LOG("Could not load; %s. pugi error: %s", animationPath, result.description());
	}
	else
	{
		pugi::xml_node objectGroup;
		for (objectGroup = animation.child("map").child("objectgroup"); objectGroup; objectGroup = objectGroup.next_sibling("objectgroup"))
		{
			p2SString objectname = objectGroup.attribute("name").as_string();
			if (objectname == "Logic")
			{
				for (pugi::xml_node property = objectGroup.child("properties").child("property"); property; property = property.next_sibling("property"))
				{
					p2SString name = property.attribute("name").as_string();
					if (name == "speed")
						speed = property.attribute("value").as_float();
					else if (name == "jump_force")
						jump_force = property.attribute("value").as_float();
				}
				for (pugi::xml_node object = objectGroup.child("object"); object; object = object.next_sibling("object"))
				{				
					p2SString name = object.attribute("name").as_string();
					if (name == "sprite")
					{
						sprite_pos.x = object.attribute("x").as_int();
						sprite_pos.y = object.attribute("y").as_int();
					}
					else if (name == "collider")
					{
						collider_pos.x = object.attribute("x").as_int();
						collider_pos.y = object.attribute("y").as_int();
						collider_size.x = object.attribute("width").as_int();
						collider_size.y = object.attribute("height").as_int();
					}
				}
				collider_offset.x = collider_pos.x - sprite_pos.x;
				collider_offset.y = collider_pos.y - sprite_pos.y;
			}
		}
	}
}

bool Entity::Collision_from_right(Collider* c1, Collider* c2) const
{
	return ((c2->rect.x + (v.x*prev_dt) + 4) > (c1->rect.x + (c1->rect.w)));
}

bool Entity::Collision_from_bottom(Collider* c1, Collider* c2, int margin) const
{
	return ((c2->rect.y - (v.y*prev_dt) + margin) >= (c1->rect.y + (c1->rect.h)));
}

bool Entity::Collision_from_left(Collider* c1, Collider* c2) const
{
	return ((c2->rect.x + (c2->rect.w)) < (c1->rect.x - (v.x*prev_dt) + 5));
}

bool Entity::Collision_from_top(Collider* c1, Collider* c2) const
{
	return ((c2->rect.y + (c2->rect.h)) < (c1->rect.y + (v.y*prev_dt) + 10));
}



