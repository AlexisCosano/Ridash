#ifndef __j1APP_H__
#define __j1APP_H__

#include "p2List.h"
#include "j1Module.h"
#include "PugiXml\src\pugixml.hpp"

// Modules
class j1Window;
class j1Input;
class j1Render;
class j1Textures;
class j1Audio;
class j1Scene;

class j1App
{
public:

	// Constructor
	j1App(int argc, char* args[]);

	// Destructor
	virtual ~j1App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(j1Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;

	// Save & Load
	bool SaveFile();
	bool LoadFile();

	bool WantToSave(pugi::xml_node&);
	bool WantToLoad(pugi::xml_node&);

	// Load an XML file
	bool LoadConfigFile();
	bool LoadSaveFile();

private:

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

public:

	// Modules
	j1Window*			win;
	j1Input*			input;
	j1Render*			render;
	j1Textures*			tex;
	j1Audio*			audio;
	j1Scene*			scene;

private:

	p2List<j1Module*>	modules;
	uint				frames;
	float				dt;

public:
	pugi::xml_document  config;
	pugi::xml_document  save_file;
	pugi::xml_node      config_node;
	pugi::xml_node      save_node;

private:
	int					argc;
	char**				args;
};

extern j1App* App;

#endif