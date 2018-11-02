#include "ModuleScene.h"

#include <vector>

#include "ModuleRenderer.h"
#include "../Application.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Components/MeshRenderer.h"
#include "../GameObject/Components/Camera.h"
#include "../GameObject/Components/CameraControl.h"
#include "../Utils/VertexBufferLayout.h"
#include "../Module/ModuleTexture.h"
#include "../Utils/Shader.h"

//#include "../Utils/Shader.h"

bool ModuleScene::Init() 
{
	gameObjects["Pyramid"] = new GameObject("Triangle");

	/* ==== PYRAMID ==== */
	std::vector<float> triangle = {
		 0.0F,  1.0F,  0.0F, 0.50F, 1.0F,	//0
		-0.5F, -0.5F,  0.5F, 0.00F, 0.0F,	//1
		 0.5F, -0.5F,  0.5F, 1.00F, 0.0F,	//2
		-0.5F, -0.5F, -0.5F, 0.33F, 0.0F,	//3
		 0.5F, -0.5F, -0.5F, 0.66F, 0.0F	//4
	};

	std::vector<unsigned int> indices = {
		0, 1, 2,
		0, 3, 1,
		0, 4, 3,
		0, 2, 4,
		3, 4, 1,
		1, 4, 2
	};


	/* ==== QUAD ====
	std::vector<float> triangle = {
		-0.5f,   0.5f,  0.0f, 0.0f, 1.0f,	
		 0.5f,   0.5f,  0.0f, 1.0f, 1.0f,	
		-0.5f,  -0.5f,  0.0f, 0.0f, 0.0f,	
		 0.5f,  -0.5f,  0.0f, 1.0f, 0.0f,	
	};

	std::vector<unsigned int> indices = {
		1, 0, 2,
		1, 2, 3
	};*/

	MyEngine::VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);

	MyEngine::Shader* texture_shader = App->renderer->CreateShader("texture", "texture.vs", "texture.fs");
	unsigned int textureID = App->texture->LoadTexture("Lenna.png");
	texture_shader->Bind();
	texture_shader->AddTexture2D(textureID);

	gameObjects["Pyramid"]->AddComponent(new MeshRenderer(&triangle, &layout, &indices, texture_shader));
	gameObjects["Pyramid"]->SetPosition(0.0F, 4.0F, -10.0F);
	gameObjects["Pyramid"]->SetRotation(0.0F, 0.0F, 0.0F);
	
	gameObjects["Camera"] = new GameObject("Camera");
	gameObjects["Camera"]->SetPosition(0.0F, 1.0F, 10.0F);
	gameObjects["Camera"]->AddComponent(new Camera());
	gameObjects["Camera"]->AddComponent(new CameraControl());
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

	// Delete all gameobjects
	for (std::unordered_map<const char*, GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
		delete(it->second);
	gameObjects.clear();

	return true;
}