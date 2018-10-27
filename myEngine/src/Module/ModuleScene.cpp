#include "ModuleScene.h"

#include <vector>

#include "../Application.h"
#include "ModuleRenderer.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Components/MeshRenderer.h"
#include "../Utils/Shader.h"

bool ModuleScene::Init() 
{
	object = new GameObject();

	std::vector<float> triangle = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	std::vector<unsigned int> indices = {
		0, 1, 2,
		2, 3, 0
	};


	App->renderer->CreateShader("default", "default.vs", "default.fs");

	object->AddComponent(new MeshRenderer(&triangle, &indices, App->renderer->GetShader("default")));

	object->Start();

	return true;
}

UpdateState ModuleScene::Update()
{
	object->Update();

	return UpdateState::Update_Continue;
}

bool ModuleScene::CleanUp()
{
	object->End();

	return true;
}