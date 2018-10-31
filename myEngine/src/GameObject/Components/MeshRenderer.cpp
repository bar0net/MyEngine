#include "MeshRenderer.h"

#include "../../Utils/Shader.h"
#include "../../Utils/VertexBuffer.h"
#include "../../Utils/IndexBuffer.h"
#include "../../Utils/VertexArray.h"
#include "../../Utils/VertexBufferLayout.h"
#include "../GameObject.h"
#include "../../Application.h"
#include "../../Module/ModuleRenderer.h"

float r;

MeshRenderer::MeshRenderer(const std::vector<float>* vertices, const MyEngine::VertexBufferLayout* layout, const std::vector<unsigned int>* indices, MyEngine::Shader* shader) : Component("MeshRenderer")
{
	// ToConsider: Instead of passing vertices and layout, pass the vao directly (?)
	this->vbo = new MyEngine::VertexBuffer(vertices);
	this->vao = new MyEngine::VertexArray();
	this->vao->AddBuffer(*vbo, *layout);

	this->ibo = new MyEngine::IndexBuffer(indices);
	this->shader = shader;
	math::float4x4 I = math::float4x4::identity;
	this->shader->SetUniform4x4("model", &I); 
	
	this->shader->SetUniform4("albedo", 0.2f, 0.8f, 0.2f, 1.0f);

	LOGINFO("Creating Mesh Renderer.");
}

MeshRenderer::~MeshRenderer()
{
	LOGINFO("Destroying Mesh Renderer.");
	delete vbo;
	delete ibo;
}

void MeshRenderer::Update()
{
	r = r + 1.f;
	if (r > 360) r -= 360;
	gameObject->SetRotation(0,r,0);

	//this->vao->Bind();
	//this->ibo->Bind();
	if (this->gameObject->transformChanged)
	{
		if (App->renderer->active_shader != shader->program) 
		{
			this->shader->Bind();
			App->renderer->active_shader = shader->program;
		}
		this->shader->SetUniform4x4("model", this->gameObject->ModelMatrix());
		this->gameObject->transformChanged = false;
	}
	App->renderer->Draw(vao, ibo, shader);

	//this->ibo->Draw();
}

void MeshRenderer::CleanUp()
{

}