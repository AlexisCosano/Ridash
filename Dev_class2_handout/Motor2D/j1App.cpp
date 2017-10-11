#include "p2Defs.h"
#include "p2Log.h"

#include "j1Window.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Scene.h"
#include "j1App.h"

// Constructor -----------------------------------------
j1App::j1App(int argc, char* args[]) : argc(argc), args(args)
{
	frames = 0;

	input = new j1Input();
	win = new j1Window();
	render = new j1Render();
	tex = new j1Textures();
	audio = new j1Audio();
	scene = new j1Scene();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(audio);
	AddModule(scene);

	// render last to swap buffer
	AddModule(render);
}

// Destructor -----------------------------------------
j1App::~j1App()
{
	// release modules
	p2List_item<j1Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.clear();
}
 // --------------------------------------------------
void j1App::AddModule(j1Module* module)
{
	module->Init();
	modules.add(module);
}

// Called before render is available -----------------
bool j1App::Awake()
{
	bool ret = true;

	bool ret2 = LoadConfigFile();
	if (ret2 == false)
		ret = false;

	bool ret3 = LoadSaveFile();
	if (ret3 == false)
		ret = false;

	p2List_item<j1Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		ret = item->data->Awake(config_node.child(item->data->name.GetString()));
		item = item->next;
	}

	return ret;
}

// Called before the first frame -------------------
bool j1App::Start()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}

	return ret;
}

// Called each loop iteration ---------------------
bool j1App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// ---------------------------------------------
void j1App::PrepareUpdate()
{
}

// ---------------------------------------------
void j1App::FinishUpdate()
{
}

// Call modules before each loop iteration
bool j1App::PreUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool j1App::DoUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool j1App::PostUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool j1App::CleanUp()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

// ---------------------------------------
int j1App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* j1App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
bool j1App::SaveFile()
{
	bool ret = true;
	
	SaveState();

	return ret;
}

// ---------------------------------------
bool j1App::LoadFile()
{
	bool ret = true;

	p2List_item<j1Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		ret = item->data->Load(save_node.child(item->data->name.GetString()));
		item = item->next;
	}

	return ret;
}

// ---------------------------------------
void j1App::WantToSave()
{
	SaveFile();
}

// ---------------------------------------
void j1App::WantToLoad()
{
	LoadFile();
}

// Load Config file ---------------------
bool j1App::LoadConfigFile()
{
	bool ret = true;
	pugi::xml_parse_result document_result = config.load_file("config.xml");

	if (document_result)
	{
		LOG("LOADING XML FILE ====================");
		LOG("The document config.xml has been loaded without any problem.");
		LOG("=====================================");
		config_node = config.child("config");
	}
	else
	{
		LOG("ERROR LOADING XML FILE ====================");
		LOG("Error description: %s", document_result.description());
		LOG("Error offset: %s", document_result.offset);
		LOG("===========================================");
		ret = false;
	}

	return ret;
}

// Load Save File ---------------------
bool j1App::LoadSaveFile()
{
	bool ret = true;
	pugi::xml_parse_result document_result = save_file.load_file("savefile.xml");

	if (document_result)
	{
		LOG("LOADING XML FILE ====================");
		LOG("The document savefile.xml has been loaded without any problem.");
		LOG("=====================================");
		save_node = save_file.child("savefile");
	}
	else
	{
		LOG("ERROR LOADING XML FILE ====================");
		LOG("Error description: %s", document_result.description());
		LOG("Error offset: %s", document_result.offset);
		LOG("===========================================");
		ret = false;
	}

	return ret;
}

// Save the state -----------------
bool j1App::SaveState()
{
	bool ret = true;

	save_file.append_child("savefile");
	save_node = save_file.child("savefile");

	p2List_item<j1Module*>* item = modules.end;

	while (item != NULL && ret == true)
	{
		ret = save_node.append_child(item->data->name.GetString());
		item->data->Save(save_node.child(item->data->name.GetString()));
		
		item = item->next;
	}

	save_file.save_file("savefile.xml");

	return true;
}