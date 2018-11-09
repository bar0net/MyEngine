#include "ModuleScene.h"

#include <vector>

#include "ModuleRenderer.h"
#include "ModuleModelLoader.h"

#include "Application.h"

#include "ModuleTexture.h"
#include "ModuleModelLoader.h"

#include "GameObject/GameObject.h"
#include "GameObject/Components/ComponentMeshRenderer.h"
#include "GameObject/Components/ComponentCamera.h"
#include "GameObject/Components/ComponentCameraControl.h"

#include "GL_Buffers/VertexBufferLayout.h"
#include "GL_Buffers/Shader.h"

//#include "../Utils/Shader.h"

bool ModuleScene::Init() 
{
	NewModel("BakerHouse.fbx");

	gameObjects["Camera"] = new GameObject("Camera");
	gameObjects["Camera"]->SetPosition(0.0F, 1.0F, 10.0F);
	gameObjects["Camera"]->AddComponent(new Camera());
	gameObjects["Camera"]->AddComponent(new CameraControl());
	gameObjects["Camera"]->Init();
	
	return true;
}

bool ModuleScene::Start()
{
	for (std::unordered_map<std::string, GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
		it->second->Start();

	return true;
}

UpdateState ModuleScene::Update()
{
	for (std::unordered_map<std::string, GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
		it->second->Update();

	return UpdateState::Update_Continue;
}

bool ModuleScene::CleanUp()
{
	for (std::unordered_map<std::string, GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
		it->second->End();

	for (std::unordered_map<std::string, GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
		it->second->CleanUp();

	// Delete all gameobjects
	for (std::unordered_map<std::string, GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
		delete(it->second);
	gameObjects.clear();

	return true;
}

void ModuleScene::NewModel(const char * file)
{
	std::string name = "Model";
	std::string root = "Model ";
	unsigned int count = 0;
	float4x4 transform;

	while (App->scene->gameObjects.find(name.c_str()) != App->scene->gameObjects.end())
		name = root + std::to_string(++count);

	std::vector<Model> models;
	if (ModuleModelLoader::Load(file, models, &transform ))
	{
		gameObjects[name] = new GameObject(name.data());
		shader = App->renderer->CreateShader("texture", "texture.vs", "texture.fs");
		gameObjects[name]->AddComponent(new MeshRenderer(models, shader));

		gameObjects[name]->SetPosition(0.0F, 0.0F, 0.0F);
		gameObjects[name]->SetRotation(0.0F, 0.0F, 0.0F);
	}
	else
	{
		LOGERROR("Could not load %s.", file);
	}
}

GameObject * ModuleScene::Find(const char * name)
{
	for (std::unordered_map<std::string, GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		if (it->first == std::string(name)) 
			return it->second;
	}
	return nullptr;
}
