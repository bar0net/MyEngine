#include "ComponentMeshRenderer.h"

#include "../GameObject.h"

#include "Application.h"
#include "Globals.h"
#include "Module/ModuleRenderer.h"
#include "Module/ModuleModelLoader.h"
#include "Module/ModuleTexture.h"
#include "Module/ModuleTime.h"
#include "GL_Buffers/Shader.h"
#include "GL_Buffers/VertexBuffer.h"
#include "GL_Buffers/IndexBuffer.h"
#include "GL_Buffers/VertexArray.h"
#include "GL_Buffers/VertexBufferLayout.h"

#include "GL/glew.h"


MeshRenderer::MeshRenderer(const std::vector<Model>& models, MyEngine::Shader* shader) : Component("MeshRenderer"), shader(shader)
{
	meshes.clear();
	meshes.reserve(models.size());

	float mins[3] = { INFINITY,  INFINITY,  INFINITY };
	float maxs[3] = { -INFINITY, -INFINITY, -INFINITY };

	for (unsigned int i = 0; i < models.size(); i++)
	{
		Mesh* mesh = new Mesh;

		mesh->num_triangles = models[i].num_triangles;
		App->texture->AssignTexture(models[i].textureID, mesh);
		mesh->vbo = new MyEngine::VertexBuffer( &models[i].vertices );
		mesh->vao = new MyEngine::VertexArray();
		mesh->vao->AddBuffer(*(mesh->vbo), models[i].layout);
		mesh->ibo = new MyEngine::IndexBuffer(&models[i].indices);

		meshes.emplace_back(mesh);

		for (unsigned int j = 0; j < 3; j++)
		{
			if (models[i].maxs[j] > maxs[j]) maxs[j] = models[i].maxs[j];
			if (models[i].mins[j] < mins[j]) mins[j] = models[i].mins[j];
		}
	}
	
	for (unsigned int i = 0; i < 3; i++)
	{
		center[i] = 0.5F * (maxs[i] + mins[i]);
		dimensions[i] = (maxs[i] - mins[i]);
	}

	this->shader->SetUniform4("albedo", 0.2F, 0.8F, 0.2F, 1.0F);

	LOGINFO("Creating Mesh Renderer.");
}

MeshRenderer::~MeshRenderer()
{
	LOGINFO("Destroying Mesh Renderer.");

	for (unsigned int i = 0; i < meshes.size(); ++i)
	{
		delete meshes[i]->vbo;
		delete meshes[i]->ibo;
		delete meshes[i]->vao;
		delete meshes[i];
	}
	meshes.clear();

	//delete shader;
}

void MeshRenderer::Update()
{
	this->shader->Bind();
	this->shader->SetUniform4x4("model", *gameObject->ModelMatrix());

	/*if (this->gameObject->transformChanged)
	{
		this->gameObject->transformChanged = false;
	}*/

	for (Mesh* mesh : meshes)
	{
		unsigned int texture;

		if (mesh->display_texture)
			texture = mesh->textureID;
		else
			texture = App->texture->checkers;

		shader->SetUniform4("albedo", mesh->albedo[0], mesh->albedo[1], mesh->albedo[2], mesh->albedo[3]);

		if (MyEngine::Globals::active_texture != texture)
		{
			shader->DisableTexture2D();
			shader->EnableTexture2D(texture);
			MyEngine::Globals::active_texture = texture;
		}

		switch (mesh->polygon)
		{
		case (1):
			App->renderer->DrawPoints(mesh->vao, mesh->ibo, shader);
			break;
		case (2):
			App->renderer->DrawLines(mesh->vao, mesh->ibo, shader);
			break;
		case (3):
			App->renderer->Draw(mesh->vao, mesh->ibo, shader);
			break;
		default:
			break;
		}
	}
}