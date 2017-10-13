#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Player.h"

#include "SDL/include/SDL.h"


j1Player::j1Player() : j1Module()
{
	name.create("player");
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

// Called before quitting --------------------
bool j1Player::CleanUp()
{
	LOG("Destroying player");

	return true;
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