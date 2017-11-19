#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Player.h"
#include "j1Collision.h"
#include "p2Log.h"
#include "j1EntityManager.h"
#include "Brofiler\Brofiler.h"

j1Collision::j1Collision()
{
	for(uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[COLLIDER_FLOOR][COLLIDER_FLOOR] = false;
	matrix[COLLIDER_FLOOR][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_FLOOR][COLLIDER_JUMPABLE] = false;
	matrix[COLLIDER_FLOOR][COLLIDER_ENEMY] = false;

	matrix[COLLIDER_PLAYER][COLLIDER_FLOOR] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_JUMPABLE] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_ENEMY] = true;

	matrix[COLLIDER_JUMPABLE][COLLIDER_FLOOR] = false;
	matrix[COLLIDER_JUMPABLE][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_JUMPABLE][COLLIDER_JUMPABLE] = false;
	matrix[COLLIDER_JUMPABLE][COLLIDER_ENEMY] = false;

	matrix[COLLIDER_ENEMY][COLLIDER_FLOOR] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_JUMPABLE] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_ENEMY] = false;

}

// Destructor
j1Collision::~j1Collision()
{}

bool j1Collision::PreUpdate()
{
	BROFILER_CATEGORY("Collision PreUpdate", Profiler::Color::Brown);
	// Remove all colliders scheduled for deletion
	for(uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if(colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

// Called before render is available
bool j1Collision::Update(float dt)
{
	BROFILER_CATEGORY("Collision Update", Profiler::Color::Brown);
	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];
		c1->Update();

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];
			c2->Update();

			if (c1->CheckCollision(c2->rect) == true)
			{
				if (matrix[c1->type][c2->type] && c1->callback)
					c1->callback->OnCollision(c1, c2);

				if (matrix[c2->type][c1->type] && c2->callback)
					c2->callback->OnCollision(c2, c1);
			}
			else if (c1->type == COLLIDER_ENEMY || c1->type == COLLIDER_PLAYER || c2->type == COLLIDER_ENEMY || c2->type == COLLIDER_PLAYER)
			{
				p2List_item<Entity*>* entity;
				for (entity = App->entityManager->entities.start; entity; entity = entity->next)
				{
					if (entity->data->collider == c1 || entity->data->collider == c2)
					{
						if ((c1 == entity->data->collidingFloor || c2 == entity->data->collidingFloor) && ((c1->type == entity->data->collider->type && (c2->type == COLLIDER_FLOOR || c2->type == COLLIDER_JUMPABLE)) || (c2->type == entity->data->collider->type && (c1->type == COLLIDER_FLOOR || c1->type == COLLIDER_JUMPABLE)))) // When there is no longer collision between entity and the previous floor or jumpable collider
						{
							entity->data->colliding_bottom = false;
							if (entity->data->state != JUMPING)
								entity->data->state = FALLING;
						}
					}
				}
			}
		}
	}

	DebugDraw();

	return true;
}

void j1Collision::DebugDraw()
{
	if(App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		debug = !debug;

	if(debug == false)
		return;

	Uint8 alpha = 80;
	for(uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if(colliders[i] == nullptr)
			continue;
		
		switch(colliders[i]->type)
		{
		case COLLIDER_NONE: // white
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case COLLIDER_FLOOR: // blue
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha);
			break;
		case COLLIDER_PLAYER: // green
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
			break;
		case COLLIDER_JUMPABLE: // cyan
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 255, alpha);
			break;
		case COLLIDER_ENEMY: // red 
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
			break;
		}
	}
}

// Called before quitting
bool j1Collision::CleanUp()
{
	LOG("Freeing all colliders");

	for(uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if(colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

Collider* j1Collision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback, Entity* entity)
{
	Collider* ret = nullptr;

	for(uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if(colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback, entity);
			break;
		}
	}

	return ret;
}

bool j1Collision::EraseCollider(Collider* collider)
{
	if(collider != nullptr)
	{
		// we still search for it in case we received a dangling pointer
		for(uint i = 0; i < MAX_COLLIDERS; ++i)
		{
			if(colliders[i] == collider)
			{
				collider->to_delete = true;
				break;
			}
		}
	}
		

	return false;
}

// -----------------------------------------------------

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	return (rect.x < r.x + r.w &&
			rect.x + rect.w > r.x &&
			rect.y < r.y + r.h &&
			rect.h + rect.y > r.y);
}

bool Collider::Update()
{

	return true;
}