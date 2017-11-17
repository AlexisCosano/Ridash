#include "p2Defs.h"
#include "p2Log.h"

#include "j1Window.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1Player.h"
#include "j1Colliders.h"
#include "j1App.h"

// Constructor -----------------------------------------
j1App::j1App(int argc, char* args[]) : argc(argc), args(args)
{
	frames = 0;
	PERF_START(ptimer);

	input = new j1Input();
	win = new j1Window();
	render = new j1Render();
	tex = new j1Textures();
	audio = new j1Audio();
	scene = new j1Scene();
	map = new j1Map();
	player = new j1Player();
	colliders = new j1Colliders();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(audio);
	AddModule(colliders);
	AddModule(scene);
	AddModule(map);
	AddModule(player);

	// render last to swap buffer
	AddModule(render);

	PERF_PEEK(ptimer);
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
	PERF_START(ptimer);
	bool				ret = false;
	pugi::xml_node		app_config;

	ret = LoadConfigFile();
	if (ret == false)
	{
		// self-config
		ret = true;
		app_config = config_node.child("app");
		title.create(app_config.child("wtitle").child_value());
		company.create(app_config.child("wcompany").child_value());
		year.create(app_config.child("wyear").child_value());

		int cap = app_config.attribute("framerate_cap").as_int(-1);

		if (cap > 0)
		{
			capped_ms = 1000 / cap;
		}
	}

	p2List_item<j1Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		ret = item->data->Awake(config_node.child(item->data->name.GetString()));
		item = item->next;
	}

	PERF_PEEK(ptimer);

	return ret;
}

// Called before the first frame -------------------
bool j1App::Start()
{
	PERF_START(ptimer);
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}

	PERF_PEEK(ptimer);

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
	frame_count++;
	last_sec_frame_count++;

	dt = frame_time.ReadSec();
	frame_time.Start();
}

// ---------------------------------------------
void j1App::FinishUpdate()
{
	// Framerate calculations --

	if (last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}

	float avg_fps = float(frame_count) / startup_time.ReadSec();
	float seconds_since_startup = startup_time.ReadSec();
	uint32 last_frame_ms = frame_time.Read();
	uint32 frames_on_last_update = prev_last_sec_frame_count;

	static char title[256];
	sprintf_s(title, 256, "Av.FPS: %.2f Last Frame Ms: %u Last sec frames: %i Last dt: %.3f Time since startup: %.3f Frame Count: %lu ",
		avg_fps, last_frame_ms, frames_on_last_update, dt, seconds_since_startup, frame_count);
	App->win->SetTitle(title);

	if (capped_ms > 0 && last_frame_ms < capped_ms)
	{
		j1PerfTimer t;
		SDL_Delay(capped_ms - last_frame_ms);
		LOG("We waited for %d milliseconds and got back in %f", capped_ms - last_frame_ms, t.ReadMs());
	}
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
	PERF_START(ptimer);
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	PERF_PEEK(ptimer);
	return ret;
}

// ---------------------------------------
int j1App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* j1App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
float j1App::GetDT() const
{
	return dt;
}

// ---------------------------------------
const char* j1App::GetCompany() const
{
	return company.GetString();
}

// ---------------------------------------
const char* j1App::GetYear() const
{
	return year.GetString();
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
	
	SaveGameFile();

	return ret;
}

// ---------------------------------------
bool j1App::LoadFile()
{
	bool ret = true;

	LoadGameFile();

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

// Save the state -----------------
bool j1App::SaveGameFile()
{
	bool ret = true;

	pugi::xml_document save_file;
	save_file.append_child("savefile");

	pugi::xml_node node = save_file.child("savefile");

	p2List_item<j1Module*>* item = modules.start;

	while (item != NULL && ret == true)
	{
		item->data->Save(node.append_child(item->data->name.GetString()));
		item = item->next;
	}
	
	

	save_file.save_file("savefile.xml");

	return true;
}

// Load the state ----------------
bool j1App::LoadGameFile()
{
	bool ret = true;

	pugi::xml_document save_file;
	pugi::xml_node node;

	pugi::xml_parse_result document_result = save_file.load_file("savefile.xml");

	if (document_result)
	{
		LOG("LOADING SAVE FILE ====================");
		LOG("The game has been loaded.");
		LOG("=====================================");
		node = save_file.child("savefile");

		p2List_item<j1Module*>* item = modules.start;

		while (item != NULL && ret == true)
		{
			item->data->Load(node.child(item->data->name.GetString()));
			item = item->next;
		}
		ret = true;
	}
	else
	{
		LOG("ERROR LOADING THE SAVE FILE ====================");
		LOG("Error description: %s", document_result.description());
		LOG("Error offset: %s", document_result.offset);
		LOG("===========================================");
		ret = false;
	}

	return ret;
}