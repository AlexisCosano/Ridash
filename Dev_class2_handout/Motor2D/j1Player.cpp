#include "p2Defs.h"
#include "p2Log.h"
#include "j1Player.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"


j1Player::j1Player() : j1Module()
{
	name.create("player");
	position.x = position.y = 0;
	speed.x = 5;
	speed.y = 0.155;
	grounded = true;
	affected_by_gravity = true;
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

	
	position.y += speed.y*(1.f, 60.f);
		

	return ret;
}

// Called before quitting --------------------
bool j1Player::CleanUp()
{
	LOG("Destroying player");

	return true;
}

// Player set speed -------------------------


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