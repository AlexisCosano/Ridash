#pragma once
#include "j1Module.h"


class j1Player : public j1Module
{
public:

	j1Player();

	// Destructor
	virtual ~j1Player();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Save & Load
	bool Save(pugi::xml_node&);
	bool Load(pugi::xml_node&);
};