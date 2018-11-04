#include "MeshRenderer.h"

#include "../GameObject.h"
#include "../../Application.h"
#include "../../Globals.h"
#include "../../Module/ModuleRenderer.h"
#include "../../Module/ModuleModelLoader.h"
#include "../../Module/ModuleTime.h"
#include "../../Utils/Shader.h"
#include "../../Utils/VertexBuffer.h"
#include "../../Utils/IndexBuffer.h"
#include "../../Utils/VertexArray.h"
#include "../../Utils/VertexBufferLayout.h"

#include "GL/glew.h"


MeshRenderer::MeshRenderer(const std::vector<Model>& models, MyEngine::Shader* shader) : Component("MeshRenderer"), shader(shader)
{
	//math::float4x4 I = math::float4x4::identity;
	//this->shader->SetUniform4x4("model", I); 

	meshes.clear();
	meshes.reserve(models.size());

	for (unsigned int i = 0; i < models.size(); i++)
	{
		Mesh mesh;

		mesh.num_triangles = models[i].num_triangles;
		mesh.textureID = models[i].textureID;
		mesh.vbo = new MyEngine::VertexBuffer( &models[i].vertices );
		mesh.vao = new MyEngine::VertexArray();
		mesh.vao->AddBuffer(*mesh.vbo, models[i].layout);
		mesh.ibo = new MyEngine::IndexBuffer(&models[i].indices);

		meshes.emplace_back(mesh);
	}
	
	this->shader->SetUniform4("albedo", 0.2F, 0.8F, 0.2F, 1.0F);

	LOGINFO("Creating Mesh Renderer.");
}

MeshRenderer::~MeshRenderer()
{
	LOGINFO("Destroying Mesh Renderer.");

	for (unsigned int i = 0; i < meshes.size(); ++i)
	{
		delete meshes[i].vbo;
		delete meshes[i].ibo;
		delete meshes[i].vao;
	}
	//delete shader;
}

void MeshRenderer::Update()
{
	if (this->gameObject->transformChanged)
	{
		this->shader->Bind();
		this->shader->SetUniform4x4("model", *gameObject->ModelMatrix());
		this->gameObject->transformChanged = false;
	}

	for (Mesh mesh : meshes)
	{
		if (MyEngine::Globals::active_texture != mesh.textureID)
		{
			shader->EnableTexture2D(mesh.textureID);
			MyEngine::Globals::active_texture = mesh.textureID;
		}

		App->renderer->Draw(mesh.vao, mesh.ibo, shader);
	}
}