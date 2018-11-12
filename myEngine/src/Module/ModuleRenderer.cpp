#include "ModuleRenderer.h"

#include <vector>

#include "GL_Buffers/VertexBuffer.h"
#include "GL_Buffers/IndexBuffer.h"
#include "GL_Buffers/VertexArray.h"
#include "GL_Buffers/Shader.h"
#include "GL_Buffers/FrameBuffer.h"
#include "GL_Buffers/Texture2D.h"
#include "GL_Buffers/RenderBuffer.h"

#include "Application.h"
#include "GameObject/GameObject.h"
#include "GameObject/Components/ComponentCamera.h"
#include "Utils/Render_Utils.h"
#include "Utils/Window_Utils.h"

#include "ModuleScene.h"
#include "ModuleTexture.h"

#include "ModuleEditor.h"

ModuleRenderer::ModuleRenderer()
{

}


ModuleRenderer::~ModuleRenderer()
{

}


bool ModuleRenderer::Init()
{
	data = MyEngine::WindowUtils::CreateWindow("Test Window", width, height);
	MyEngine::RenderUtils::EnableVSync(vsyncEnabled);

	if (!data->valid)
	{
		LOGERROR("Failed to create a window.");
		return false;
	}
	LOGINFO("Window successfully created.");

	MyEngine::RenderUtils::CreateViewport(width, height, { 0.3F, 0.3F, 0.3F, 1.0F }, 1.0F);

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
	Render();
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

void ModuleRenderer::Render()
{
	float4x4 identity = float4x4::identity;
	for (std::vector<DrawCall>::iterator it = drawCalls.begin(); it != drawCalls.end(); ++it)
	{
		unsigned int texture = 0;

		if (it->mesh->textureID == 0) it->shader->DisableTexture2D();
		else
		{
			if (it->mesh->display_texture) texture = it->mesh->textureID;
			else texture = App->texture->checkers;

			if (MyEngine::Globals::active_texture != texture)
			{
				it->shader->DisableTexture2D();
				it->shader->EnableTexture2D(texture);
			}
		}
		
		if (it->transform) it->shader->SetUniform4x4("model", *(it->transform));
		else it->shader->SetUniform4x4("model", identity);


		it->shader->SetUniform4("albedo", it->mesh->albedo[0], it->mesh->albedo[1], it->mesh->albedo[2], it->mesh->albedo[3]);

		it->shader->Bind();
		it->mesh->vao->Bind();
		it->mesh->ibo->Bind();

		if (showWireframe)	it->mesh->ibo->DrawWires();
		else
		{
			if (it->mesh->polygon == 1) it->mesh->ibo->DrawPoints(it->size);
			if (it->mesh->polygon == 2) it->mesh->ibo->DrawLines(it->size);
			if (it->mesh->polygon == 3) it->mesh->ibo->Draw();
		}
	}
	drawCalls.clear();
}

void ModuleRenderer::Draw(const Mesh * mesh, math::float4x4* transform, const MyEngine::Shader * shader, float size)
{
	assert(mesh && shader);

	drawCalls.emplace_back(mesh, transform, shader, size);
}

/*
void ModuleRenderer::Draw(const MyEngine::VertexArray* vao, const MyEngine::IndexBuffer * ibo, const MyEngine::Shader * shader)
{
	assert(vao && ibo && shader);

	drawCalls.push_back(DrawCall(vao, ibo, shader, 3));
}

void ModuleRenderer::DrawLines(const MyEngine::VertexArray * vao, const MyEngine::IndexBuffer * ibo, const MyEngine::Shader * shader, float line_width)
{
	assert(vao && ibo && shader);

	drawCalls.push_back(DrawCall(vao, ibo, shader, 2, line_width));
}

void ModuleRenderer::DrawPoints(const MyEngine::VertexArray * vao, const MyEngine::IndexBuffer * ibo, const MyEngine::Shader * shader, float point_size)
{
	assert(vao && ibo && shader);

	drawCalls.push_back(DrawCall(vao, ibo, shader, 1, point_size));
}
*/

MyEngine::Shader* ModuleRenderer::CreateShader(const char* name, const char* vShader_file, const char* fShader_file)
{
	assert(name && vShader_file && fShader_file);

	if (shaders.find(name) == shaders.end())
		shaders[name] = new MyEngine::Shader(vShader_file, fShader_file);
	else
		LOGWARNING("Cannot create new shader %s because it already exists.", name);
	
	return shaders[name];
}

MyEngine::Shader* ModuleRenderer::GetShader(const char* name)
{
	assert(name);

	if (shaders.find(name) != shaders.end())
		return shaders[name];
	else
	{
		if (shaders.size() > 0)
		{
			LOGERROR("Trying to access shader %s but it doesn't exist. Using %s shader instead.", name, shaders.begin()->first);
			return shaders.begin()->second;
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
	shaders.empty();
}

void ModuleRenderer::ResizedWindow()
{
	MyEngine::WindowUtils::WindowSize(data, &width, &height);
	MyEngine::RenderUtils::ModifyViewportSize(width, height);

	Camera* c = (Camera*)App->scene->gameObjects["Camera"]->components[ComponentType::CAMERA][0];
	assert(c);
	c->UpdateFrustum();
}

void ModuleRenderer::EnableVSync(bool enabled)
{
	MyEngine::RenderUtils::EnableVSync(enabled);
	vsyncEnabled = enabled;
}

void ModuleRenderer::UpdateClearColor() const
{
	MyEngine::RenderUtils::ChangeClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
}

void ModuleRenderer::UpdateClearColor(float r, float g, float b, float a)
{
	MyEngine::RenderUtils::ChangeClearColor(r, g, b, a);
	clearColor[0] = r; clearColor[1] = g; clearColor[2] = b; clearColor[3] = a;
}

DrawCall::DrawCall(const Mesh * mesh, math::float4x4 * transform, const MyEngine::Shader * shader, float size) :
mesh(mesh), transform(transform), shader(shader), size(size) { }
