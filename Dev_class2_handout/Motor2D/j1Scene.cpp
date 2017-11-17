#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Player.h"
#include "j1Scene.h"
#include "j1Map.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	//App->audio->PlayMusic("audio/music/music_sadpiano.ogg");	
	main_character = App->player;
	main_character->position = App->map->spawn_point;
	main_character->texture = App->tex->Load("textures/Santa.png");
	offset.x = 380;
	offset.y = 320;
	return true;
}

void j1Scene::LoadNextMap(int map)
{
	to_load = true;

	if (map == 1)
	{
		map_to_load = "level1.tmx";
	}
	else
	{
		map_to_load = "level2.tmx";
	}
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	// Save & Load
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		LoadNextMap(1);
		main_character->position = App->map->spawn_point;
		LOG("Starting from the very first level");
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		main_character->position = App->map->spawn_point;
		LOG("Starting from the beginning of the current level");
	}

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		LOG("Saving game...");
		App->WantToSave();
	}

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		LOG("Loading game...");
		App->WantToLoad();
	}
	
	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
	{
		LOG("locking/unlocking FPS cap");

	}
	App->render->camera.x = -main_character->position.x + offset.x;
	App->render->camera.y = -main_character->position.y + offset.y;

	App->map->Draw();

	App->render->Blit(main_character->texture, main_character->position.x, main_character->position.y);

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d ",
		App->map->data.width, App->map->data.height,
		App->map->data.tile_width, App->map->data.tile_height,
		App->map->data.tilesets.count(),
		map_coordinates.x, map_coordinates.y);

	//App->win->SetTitle(title.GetString());
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	if (to_load)
	{
		App->map->Unload();
		App->map->Load(map_to_load);
		main_character->position = App->map->spawn_point;
		to_load = false;
	}

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	return true;
}
