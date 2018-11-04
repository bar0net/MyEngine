#include "MeshRenderer.h"

#include "../../Utils/Shader.h"
#include "../../Utils/VertexBuffer.h"
#include "../../Utils/IndexBuffer.h"
#include "../../Utils/VertexArray.h"
#include "../../Utils/VertexBufferLayout.h"
#include "../GameObject.h"
#include "../../Application.h"
#include "../../Module/ModuleRenderer.h"
#include "../../Module/ModuleTime.h"

#include "GL/glew.h"


MeshRenderer::MeshRenderer(const std::vector<float>* vertices, const MyEngine::VertexBufferLayout* layout, const std::vector<unsigned int>* indices, MyEngine::Shader* shader) : Component("MeshRenderer"), shader(shader)
{
	this->vbo = new MyEngine::VertexBuffer(vertices);
	this->vao = new MyEngine::VertexArray();
	this->vao->AddBuffer(*vbo, *layout);

	this->ibo = new MyEngine::IndexBuffer(indices);
	math::float4x4 I = math::float4x4::identity;
	this->shader->SetUniform4x4("model", I); 
	
	this->shader->SetUniform4("albedo", 0.2F, 0.8F, 0.2F, 1.0F);

	LOGINFO("Creating Mesh Renderer.");
}

MeshRenderer::~MeshRenderer()
{
	LOGINFO("Destroying Mesh Renderer.");
	delete vbo;
	delete ibo;
	delete vao;
}

void MeshRenderer::Update()
{
	if (this->gameObject->transformChanged)
	{
		this->shader->Bind();
		this->shader->SetUniform4x4("model", *gameObject->ModelMatrix());
		this->gameObject->transformChanged = false;
	}
	App->renderer->Draw(vao, ibo, shader);
}