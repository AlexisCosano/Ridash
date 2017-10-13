#pragma once
#include "j1Module.h"
#include "p2Point.h"

struct SDL_Texture;

class j1Player : public j1Module
{
public:

	j1Player();

	// Destructor
	virtual ~j1Player();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Update
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Save & Load
	bool Save(pugi::xml_node&);
	bool Load(pugi::xml_node&);

	void Dash(int direction);

public:

	p2Point<int> position;
	p2Point<int> speed;
	p2Point<int> gravity;
	SDL_Texture* texture = nullptr;
	int direction;
	bool grounded;
};