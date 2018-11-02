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
		 0.0f,  1.0f,  0.0f, 0.50f, 1.0f,	//0
		-0.5f, -0.5f,  0.5f, 0.00f, 0.0f,	//1
		 0.5f, -0.5f,  0.5f, 1.00f, 0.0f,	//2
		-0.5f, -0.5f, -0.5f, 0.33f, 0.0f,	//3
		 0.5f, -0.5f, -0.5f, 0.66f, 0.0f	//4
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
		-0.5f,   0.5f,  0.0f, 0.0f, 1.0f,	//0
		 0.5f,   0.5f,  0.0f, 1.0f, 1.0f,	//1
		-0.5f,  -0.5f,  0.0f, 0.0f, 0.0f,	//2
		 0.5f,  -0.5f,  0.0f, 1.0f, 0.0f,	//3
	};

	std::vector<unsigned int> indices = {
		1, 0, 2,
		1, 2, 3
	};*/

	MyEngine::VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);

	App->renderer->CreateShader("texture", "texture.vs", "texture.fs");
	MyEngine::Shader* shader = App->renderer->GetShader("texture");
	unsigned int textureID = App->texture->LoadTexture("Lenna.png");
	shader->Bind();
	shader->AddTexture2D(textureID);

	gameObjects["Pyramid"]->AddComponent(new MeshRenderer(&triangle, &layout, &indices, shader));
	gameObjects["Pyramid"]->SetPosition(0.0f, 4.0f, -10.0f);
	gameObjects["Pyramid"]->SetRotation(0.0f, 0.0f, 0.0f);
	
	gameObjects["Camera"] = new GameObject("Camera");
	gameObjects["Camera"]->SetPosition(0.0f, 1.0f, 10.0f);
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


	for (std::unordered_map<const char*, GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
		delete(it->second);
	gameObjects.clear();

	return true;
}