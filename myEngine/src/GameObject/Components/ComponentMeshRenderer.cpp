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
	componentType = ComponentType::MESH_RENDERER;

	float mins[3] = { INFINITY,  INFINITY,  INFINITY };
	float maxs[3] = { -INFINITY, -INFINITY, -INFINITY };

	for (unsigned int i = 0U; i < models.size(); ++i)
	{
		Mesh* mesh = new Mesh;

		mesh->num_triangles = models[i].num_triangles;
		App->texture->AssignTexture(models[i].textureID, mesh);
		mesh->vbo = new MyEngine::VertexBuffer( &models[i].vertices );
		mesh->vao = new MyEngine::VertexArray();
		mesh->vao->AddBuffer(*(mesh->vbo), models[i].layout);
		mesh->ibo = new MyEngine::IndexBuffer(&models[i].indices);

		meshes.emplace_back(mesh);

		for (unsigned int j = 0U; j < 3U; ++j)
		{
			if (models[i].maxs[j] > maxs[j]) maxs[j] = models[i].maxs[j];
			if (models[i].mins[j] < mins[j]) mins[j] = models[i].mins[j];
		}
	}
	
	for (unsigned int i = 0U; i < 3U; ++i)
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

	for (unsigned int i = 0U; i < meshes.size(); ++i)
	{
		RELEASE(meshes[i]->vbo);
		RELEASE(meshes[i]->ibo);
		RELEASE(meshes[i]->vao);
		RELEASE(meshes[i]);
	}
	meshes.clear();
}

void MeshRenderer::Update()
{
	for (Mesh* mesh : meshes)
	{
		App->renderer->Draw(mesh, gameObject->ModelMatrix(), shader);
	}
}

void MeshRenderer::CleanUp()
{
	LOGINFO("Mesh Rendederer Clean up.");
	
	for (Mesh* mesh : meshes)
	{
		App->texture->AssignTexture(App->texture->checkers, mesh);
	}
}
