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
	gameObjects["Pyramid"] = new GameObject("Triangle");

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
		0, 2, 4,
		3, 4, 1,
		1, 4, 2
	};

	App->renderer->CreateShader("default", "default.vs", "default.fs");
	
	gameObjects["Pyramid"]->AddComponent(new MeshRenderer(&triangle, &indices, App->renderer->GetShader("default")));
	gameObjects["Pyramid"]->SetPosition(0.0f, 4.0f, -10.0f);
	gameObjects["Pyramid"]->SetRotation(0.0f, 0.0f, 0.0f);
	
	gameObjects["Camera"] = new GameObject("Camera");
	gameObjects["Camera"]->SetPosition(0.0f, 1.0f, 10.0f);
	//camera->SetRotation(-.4f, 0.0f, .1f);
	gameObjects["Camera"]->AddComponent(new Camera());
	gameObjects["Camera"]->Init();
	
	return true;
}

bool ModuleScene::Start()
{
	for (std::unordered_map<const char*, GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
		it->second->Start();

	return true;
}

UpdateState ModuleScene::Update()
{
	for (std::unordered_map<const char*, GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
		it->second->Update();

	return UpdateState::Update_Continue;
}

bool ModuleScene::CleanUp()
{
	for (std::unordered_map<const char*, GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
		it->second->End();

	for (std::unordered_map<const char*, GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
		it->second->CleanUp();


	for (std::unordered_map<const char*, GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
		delete(it->second);
	gameObjects.clear();

	return true;
}