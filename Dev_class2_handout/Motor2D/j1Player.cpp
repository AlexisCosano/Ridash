#include "p2Defs.h"
#include "p2Log.h"
#include "j1Player.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Scene.h"
#include "j1Colliders.h"
#include "j1Map.h"


j1Player::j1Player() : j1Module()
{
	name.create("player");

	position.SetToZero();

	speed.x = 3;
	speed.y = 60;

	dashing_speed = 10;

	dash_distance = 256.f;
	current_dash_distance = 0.f;

	gravity.x = 0;
	gravity.y = 8;

	player_rect = { 0, 0, 77, 128 };

	direction = 1; // 1 - right, -1 - left

	jumping = false;
	jump_force = 16;
	jump_distance = 256.f;
	current_jump_distance = 0.f;

	bool grounded;
}

// Destructor ---------------------------------
j1Player::~j1Player()
{
}

// Called before render is available ----------
bool j1Player::Awake(pugi::xml_node&)
{
	LOG("Init player");
	bool ret = true;
	LOG(" INITIAL Position = (%i, %i)", position.x, position.y);
	return ret;
}

// Update ------------------------------------
bool j1Player::Update(float dt)
{
	bool ret = true;

	player_rect.x = position.x;
	player_rect.y = position.y;

	if (dashing)
	{
		player_rect.x += direction * dashing_speed;
		if (CheckCollisions() == false)
		{
			position.x = player_rect.x;
		}
		else
		{
			player_rect.x = position.x;
			current_dash_distance = 0.f;
			dashing = false;
		}

		if (dash_distance <= current_dash_distance)
		{
			current_dash_distance = 0.f;
			dashing = false;
		}
		else
		{
			current_dash_distance += dashing_speed;
		}
	}
	else
	{
		// Player Controls
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			player_rect.x -= 1 * speed.x;
			if (CheckCollisions() == false)
			{
				position.x = player_rect.x;
			}
			else
			{
				player_rect.x = position.x;
			}
			direction = -1;

			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				Dash();
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			player_rect.x += 1 * speed.x;
			if (CheckCollisions() == false)
			{
				position.x = player_rect.x;
			}
			else
			{
				player_rect.x = position.x;
			}
			direction = 1;

			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				Dash();
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
		{
			jumping = true;
		}

		if (jumping)
		{
			player_rect.y -= jump_force;
			if (CheckCollisions() == false)
			{
				position.y = player_rect.y;
			}
			else
			{
				player_rect.y = position.y;
			}
			if (jump_distance <= current_jump_distance)
			{
				current_jump_distance = 0.f;
				jumping = false;
			}
			else
			{
				current_jump_distance += jump_force;
			}
		}
		else
		{
			player_rect.y += gravity.y;
			if (CheckCollisions() == false)
			{
				position.y = player_rect.y;
			}
			else
			{
				player_rect.y = position.y;
			}
		}
	}

	if (CheckDeath() == true)
	{
		position = App->map->spawn_point;
	}

	if (CheckVictory() == true)
	{
		LOG("You've won!");
	}

	return ret;
}

// Set texture
void j1Player::SetTexture(SDL_Texture* texture)
{
	this->texture = texture;

	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	player_rect.h = 128;
	player_rect.w = 77;
}

// Called before quitting --------------------
bool j1Player::CleanUp()
{
	LOG("Destroying player");

	return true;
}

// Dash -------------------------------------
void j1Player::Dash()
{
	jumping = false;
	current_jump_distance = 0.f;
	dashing = true;
}

// Collisions
bool j1Player::CheckCollisions()
{
	bool ret = false;
	p2List_item<SDL_Rect>* item = App->colliders->colliders.start;

	for (item; item != App->colliders->colliders.end; item = item->next)
	{
		ret = App->colliders->CheckCollision(player_rect, item->data);
		if (ret)
			return ret;
	}

	return ret;
}

// Death
bool j1Player::CheckDeath()
{
	bool ret = false;
	p2List_item<SDL_Rect>* item = App->colliders->death_triggers.start;

	for (item; item != App->colliders->death_triggers.end; item = item->next)
	{
		ret = App->colliders->CheckCollision(player_rect, item->data);
		if (ret)
			return ret;
	}

	return ret;
}

// Victory
bool j1Player::CheckVictory()
{
	bool ret = false;
	p2List_item<SDL_Rect>* item = App->colliders->victory_triggers.start;

	for (item; item != App->colliders->victory_triggers.end; item = item->next)
	{
		ret = App->colliders->CheckCollision(player_rect, item->data);
		if (ret)
			return ret;
	}

	return ret;
}

// Save & Load ------------------------------
bool j1Player::Save(pugi::xml_node&)
{
	bool ret = true;

	return true;
}

bool j1Player::Load(pugi::xml_node&)
{
	bool ret = true;

	return true;
}