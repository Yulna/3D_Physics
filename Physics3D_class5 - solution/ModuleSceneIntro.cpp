#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));


	p_pointscube.size.x = 5;
	p_pointscube.size.y = 5;
	p_pointscube.size.z = 5;
	pointscube = App->physics->AddBody(p_pointscube, 10);
	pointscube->SetPos(3, 10, 0);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");


	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	pointscube->GetTransform(&p_pointscube.transform);
	p_pointscube.Render();


	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
		App->player->AddPoints(50);

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body1 == pointscube || body2 == pointscube)
	{
		App->player->AddPoints(20);
	}



}

