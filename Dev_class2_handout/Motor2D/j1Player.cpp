#include "p2Defs.h"
#include "p2Log.h"
#include "j1Player.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Scene.h"


j1Player::j1Player() : j1Module()
{
	name.create("player");

	position.x = 0;
	position.y = 0;

	speed.x = 3;
	speed.y = 60;

	gravity.x = 0;
	gravity.y = 8;

	direction = 1; // 1 - right, 0 - left

	grounded = true;
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

	// Player Controls
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		position.x -= 1 * speed.x;
		direction = 0;

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			Dash(direction);
	}
		
		
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		position.x += 1 * speed.x;
		direction = 1;

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			Dash(direction);
	}
		
	
	position += gravity;

	return ret;
}

// Called before quitting --------------------
bool j1Player::CleanUp()
{
	LOG("Destroying player");

	return true;
}

// Dash -------------------------------------
void j1Player::Dash(int direction)
{
	if (direction == 1)
	{
		for (int i = 0; i <= 15; ++i)
		{
			position.x += i;
			App->render->Blit(App->scene->main_character->texture, position.x, position.y);
		}
	}
	else
	{
		for (int i = 0; i <= 15; ++i)
		{
			position.x -= i;
			App->render->Blit(App->scene->main_character->texture, position.x, position.y);
		}
	}
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