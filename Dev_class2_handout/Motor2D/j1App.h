#ifndef __j1APP_H__
#define __j1APP_H__

#include "p2List.h"
#include "j1Module.h"
#include "j1PerfTimer.h"
#include "j1Timer.h"
#include "PugiXml\src\pugixml.hpp"

// Modules
class j1Window;
class j1Input;
class j1Render;
class j1Textures;
class j1Audio;
class j1Scene;
class j1Map;
class j1Player;
class j1Colliders;
class j1PathFinding;

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
	const char* GetTitle() const;
	const char* GetCompany() const;
	const char* GetYear() const;
	float GetDT() const;

	// Save & Load
	bool SaveFile();
	bool LoadFile();

	void WantToSave();
	void WantToLoad();

	// Load an XML file
	bool LoadConfigFile();

	// Save state
	bool SaveGameFile();
	bool LoadGameFile();

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
	j1Map*				map;
	j1Player*           player;
	j1Colliders*        colliders;
	j1PathFinding*		pathfinding;

private:

	p2List<j1Module*>	modules;
	uint				frames;
	float				dt;

	p2SString			title;
	p2SString			company;
	p2SString			year;

	j1PerfTimer			ptimer;
	uint64				frame_count = 0;
	j1Timer				startup_time;
	j1Timer				frame_time;
	j1Timer				last_sec_frame_time;
	uint32				last_sec_frame_count = 0;
	uint32				prev_last_sec_frame_count = 0;
	int					capped_ms = -1;

public:

	pugi::xml_document  config;
	pugi::xml_node      config_node;

private:
	int					argc;
	char**				args;
};

extern j1App* App;

#endif