#include "j1Entity.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Input.h"
#include "j1Audio.h"
#include "p2Log.h"

bool Entity::Entity_Update(float dt)
{
	v.y += (gravity * ((colliding_bottom) ? 0 : 1));
	if (v.y < -6)
		v.y = -6;
	virtualPosition.y -= v.y;

	if (pos_relCam > 2 || v.x > 0)
		virtualPosition.x += v.x;

	colliding_right = false;
	colliding_left = false;

	setAnimation();

	return true;
}

void Entity::Entity_OnCollision(Collider* c1, Collider* c2)
{
	if (c2->type == COLLIDER_FLOOR)
	{
		if ((c2->rect.y - v.y + 1) > (c1->rect.y + (c1->rect.h))) //The collision is from bottom
		{
			virtualPosition.y = c2->rect.y - animation->GetCurrentFrame().h + 1;
			if (colliding_bottom == false)
			{
				v.y = 0;
				if (App->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT || v.x == 0)
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
				colliding_bottom = true;
				App->audio->PlayFx(landing_fx, 0);
				//Touching ground sound
			}
			collidingFloor = c2;
		}
		if ((c2->rect.x + 4) >(c1->rect.x + (c1->rect.w))) // Collision is from right
		{
			if (v.x > 0)
			{
				v.x = 0;
			}
			colliding_right = true;
		}
		else if ((c2->rect.x + (c2->rect.w)) < (c1->rect.x + 5)) // Collision is from left
		{
			if (v.x < 0)
			{
				v.x = 0;
			}
			colliding_left = true;
		}
		if ((c2->rect.y + (c2->rect.h)) < (c1->rect.y + 5)) // Collision is from top
		{
			if (v.y > 0)
			{
				v.y = 0;
			}
		}
	}
	else if (c2->type == COLLIDER_JUMPABLE)
	{
		if (((c2->rect.y - v.y + 1) > (c1->rect.y + (c1->rect.h)))) //The collision is from bottom
		{
			virtualPosition.y = c2->rect.y - animation->GetCurrentFrame().h + 1;
			if (colliding_bottom == false)
			{
				v.y = 0;
				if (App->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT || v.x == 0)
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
				colliding_bottom = true;
				App->audio->PlayFx(landing_fx, 0);
				//Touching ground sound
			}
			collidingFloor = c2;
		}
	}
}

void Entity::setAnimation()
{
	if (specificAnimation())
	{ }
	else if (v.x > 0)
	{
		if (state == JUMPING)
		{
			animation = &jumping_right;
		}
		else if (state == FALLING)
		{
			animation = &falling_right;
		}
		else
		{
			animation = &right;
		}
	}
	else if (v.x < 0)
	{
		if (state == JUMPING)
		{
			animation = &jumping_left;
		}
		else if (state == FALLING)
		{
			animation = &falling_left;
		}
		else
		{
			animation = &left;
		}
	}
	else
	{
		if (state == IDLE)
		{
			if (animation == &left || animation == &jumping_left || animation == &falling_left)
			{
				animation = &idle_left;
			}
			else if (animation == &right || animation == &jumping_right || animation == &falling_right)
			{
				animation = &idle_right;
			}
		}
		else if (state == JUMPING)
		{
			if (animation == &idle_left)
			{
				animation = &jumping_left;
			}
			else if (animation == &idle_right)
			{
				animation = &jumping_right;
			}
		}
		else if (state == LEFT)
		{
			animation = &left;
		}
		else if (state == RIGHT)
		{
			animation = &right;
		}
	}
}

Animation Entity::LoadAnimation(const char* animationPath, const char* animationName)
{
	Animation ret;

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
			int x, y, h, w;
			float speed;
			for (pugi::xml_node sprite = objectGroup.child("object"); sprite; sprite = sprite.next_sibling("object"))
			{
				x = sprite.attribute("x").as_int();
				y = sprite.attribute("y").as_int();
				w = sprite.attribute("width").as_int();
				h = sprite.attribute("height").as_int();

				ret.PushBack({ x, y, w, h });

				for (pugi::xml_node property = objectGroup.child("properties").child("property"); property; property = property.next_sibling("property"))
				{
					p2SString name = property.attribute("name").as_string();
					if (name == "speed")
					{
						speed = property.attribute("value").as_float();
						ret.speed = speed;
						break;
					}
				}
			}
			break;
		}
	}

	pugi::xml_node image = animation.child("map").child("imagelayer").child("image");
	//ret.texture = App->tex->Load(image.attribute("source").as_string());

	return ret;
}