#include "ModuleRenderer.h"

#include <vector>

#include "../Utils/Window_Utils.h"
#include "../Utils/Render_Utils.h"
#include "../Utils/VertexBuffer.h"
#include "../Utils/IndexBuffer.h"
#include "../Utils/Shader.h"

ModuleRenderer::ModuleRenderer()
{

}


ModuleRenderer::~ModuleRenderer()
{

}


bool ModuleRenderer::Init()
{
	data = MyEngine::WindowUtils::CreateWindow("Test Window", width, height);

	if (!data->valid)
	{
		LOGERROR("Failed to create a window.");
		return false;
	}
	LOGINFO("Window successfully created.");

	MyEngine::RenderUtils::CreateViewport(width, height, { 0.3f, 0.3f, 0.3f, 1.0f }, 1.0);

	return true;
}


UpdateState ModuleRenderer::PreUpdate()
{
	MyEngine::RenderUtils::ClearViewport();

	return UpdateState::Update_Continue;
}

UpdateState ModuleRenderer::Update()
{
	return UpdateState::Update_Continue;
}

UpdateState ModuleRenderer::PostUpdate()
{
	MyEngine::WindowUtils::SwapWindow(data);

	return UpdateState::Update_Continue;
}


bool ModuleRenderer::CleanUp()
{
	LOGINFO("Destroying window.");
	MyEngine::WindowUtils::DestroyWindow(data);

	delete data;
	LOGINFO("Renderer closed.");

	this->EmptyShaders();

	return true;
}

void ModuleRenderer::CreateShader(const char* name, const char* vShader_file, const char* fShader_file)
{
	if (materials.find(name) == materials.end())
		materials[name] = new MyEngine::Shader(vShader_file, fShader_file);
	else
		LOGWARNING("Trying to add a new shader %s but it already exists.", name);
}

MyEngine::Shader* ModuleRenderer::GetShader(const char* name)
{
	if (materials.find(name) != materials.end())
		return materials[name];
	else
	{
		if (materials.size() > 0)
		{
			LOGERROR("Trying to access shader %s but it doesn't exist. Using %s shader instead.", name, materials.begin()->first);
			return materials.begin()->second;
		}
		else 
		{
			LOGERROR("Trying to access shader %s but the shader collection is empty.", name);
			ASSERT(false);
			return nullptr;
		}
	}
}

void ModuleRenderer::EmptyShaders()
{
	LOGINFO("Emptying the shaders collection");
	//for (std::unordered_map<const char*, MyEngine::Shader*>::iterator it = materials.begin(); it != materials.end(); ++it)
		//delete (it->second);

	materials.empty();
}