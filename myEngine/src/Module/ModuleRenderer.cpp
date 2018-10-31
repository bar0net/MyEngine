#include "ModuleRenderer.h"

#include <vector>

#include "../Utils/Window_Utils.h"
#include "../Utils/Render_Utils.h"
#include "../Utils/VertexBuffer.h"
#include "../Utils/IndexBuffer.h"
#include "../Utils/VertexArray.h"
#include "../Utils/Shader.h"
#include "../Application.h"

#include "ModuleScene.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/Components/Camera.h"

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

void ModuleRenderer::Draw(MyEngine::VertexArray * vao, MyEngine::IndexBuffer * ibo, MyEngine::Shader * shader)
{
	if ((int)vao->vao != active_vao)
	{
		active_vao = vao->vao;
		vao->Bind();
	}

	if ((int)ibo->ibo != active_ibo)
	{
		active_ibo = ibo->ibo;
		ibo->Bind();
	}

	if ((int)shader->program != active_shader)
	{
		active_shader = shader->program;
		shader->Bind();
	}

	ibo->Draw();
}

void ModuleRenderer::DrawLines(MyEngine::VertexArray * vao, MyEngine::IndexBuffer * ibo, MyEngine::Shader * shader)
{
	if ((int)vao->vao != active_vao)
	{
		active_vao = vao->vao;
		vao->Bind();
	}

	if ((int)ibo->ibo != active_ibo)
	{
		active_ibo = ibo->ibo;
		ibo->Bind();
	}

	if ((int)shader->program != active_shader)
	{
		active_shader = shader->program;
		shader->Bind();
	}

	ibo->DrawLines();
}

void ModuleRenderer::CreateShader(const char* name, const char* vShader_file, const char* fShader_file)
{
	if (materials.find(name) == materials.end())
		materials[name] = new MyEngine::Shader(vShader_file, fShader_file);
	else
		LOGWARNING("Cannot create new shader %s because it already exists.", name);
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

void ModuleRenderer::ResizedWindow()
{
	MyEngine::WindowUtils::WindowSize(data, &width, &height);
	MyEngine::RenderUtils::ModifyViewportSize(width, height);
	Camera* c = (Camera*)App->scene->gameObjects["Camera"]->components["Camera"];
	c->UpdateFrustum();
}

void ModuleRenderer::UpdateClearColor()
{
	MyEngine::RenderUtils::ChangeClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
}

void ModuleRenderer::UpdateClearColor(float r, float g, float b, float a)
{
	MyEngine::RenderUtils::ChangeClearColor(r, g, b, a);
	clearColor[0] = r; clearColor[1] = g; clearColor[2] = b; clearColor[3] = a;
}
