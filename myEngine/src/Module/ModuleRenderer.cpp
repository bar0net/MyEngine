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

	// --
	/*std::vector<float> triangle = {
		 0.0f,  -0.33f,  0.5f,
		-0.33f, -0.33f, -0.33f,
		 0.33f, -0.33f, -0.33f,
		 0.0f,  0.66f,  0.0f
	};*/

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

	vbo = new MyEngine::VertexBuffer(&triangle);
	ibo = new MyEngine::IndexBuffer(&indices);
	shader = new MyEngine::Shader("default.vs", "default.fs");
	//--

	return true;
}


UpdateState ModuleRenderer::PreUpdate()
{
	MyEngine::RenderUtils::ClearViewport();

	return UpdateState::Update_Continue;
}

UpdateState ModuleRenderer::Update()
{
	shader->Bind();
	vbo->Bind();
	ibo->Bind();
	ibo->Draw();
	ibo->UnBind();
	vbo->UnBind();
	shader->UnBind();

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

	delete(shader);
	delete(vbo);

	delete data;
	return true;
}