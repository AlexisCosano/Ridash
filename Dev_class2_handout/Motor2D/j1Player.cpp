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

	position.SetToZero();

	speed.x = 3;
	speed.y = 60;

	gravity.x = 0;
	gravity.y = 8;

	grounded = true;
	dash = false;
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
		position.x -= 1 * speed.x;
		
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		position.x += 1 * speed.x;

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		dash = true;

	if (dash == true)
	{
		for (int i = 0; i <= 15; ++i)
		{
			position.x += i;
			App->render->Blit(App->scene->main_character->texture, position.x, position.y);
		}

		dash = false;
	}
	else
		App->render->Blit(App->scene->main_character->texture, position.x, position.y);
	
	return ret;
}

// Called before quitting --------------------
bool j1Player::CleanUp()
{
	LOG("Destroying player");

	return true;
}

// Dash -------------------------------------

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