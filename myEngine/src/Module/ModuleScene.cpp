#include "ModuleScene.h"

#include <vector>

#include "../Application.h"
#include "ModuleRenderer.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Components/MeshRenderer.h"
#include "../GameObject/Components/Camera.h"
//#include "../Utils/Shader.h"

bool ModuleScene::Init() 
{
	object = new GameObject();

	std::vector<float> triangle = {
		 0.0f,  1.0f,  0.0f, //0
		-0.5f, -0.5f,  0.5f, //1
		 0.5f, -0.5f,  0.5f,  //2
		-0.5f, -0.5f,  -0.5f, //3
		 0.5f, -0.5f,  -0.5f //4
	};

	std::vector<unsigned int> indices = {
		0, 1, 2,
		0, 3, 1,
		0, 4, 3,
		0, 2, 4
	};

	App->renderer->CreateShader("default", "default.vs", "default.fs");
	
	object->AddComponent(new MeshRenderer(&triangle, &indices, App->renderer->GetShader("default")));
	object->SetPosition(0.0f, 0.0f, -10.0f);
	object->SetRotation(0.0f, 0.0f, 0.0f);
	
	camera = new GameObject();
	camera->SetPosition(0.0f, 1.0f, 0.0f);
	//camera->SetRotation(-.4f, 0.0f, .1f);
	camera->AddComponent(new Camera());
	camera->Init();
	
	return true;
}

bool ModuleScene::Start()
{
	//object->Start();
	camera->Start();

	return true;
}

UpdateState ModuleScene::Update()
{
	object->Update();
	camera->Update();

	return UpdateState::Update_Continue;
}

bool ModuleScene::CleanUp()
{
	object->End();
	camera->End();

	object->CleanUp();
	camera->CleanUp();

	delete object;
	delete camera;

	return true;
}