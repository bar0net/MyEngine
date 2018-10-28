#include "MeshRenderer.h"

#include "../../Utils/Shader.h"
#include "../../Utils/VertexBuffer.h"
#include "../../Utils/IndexBuffer.h"
#include "../GameObject.h"

float r;

MeshRenderer::MeshRenderer(const std::vector<float>* vertices, const std::vector<unsigned int>* indices, MyEngine::Shader* shader) : Component("MeshRenderer")
{
	this->vbo = new MyEngine::VertexBuffer(vertices);
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
	r = r + 0.01f;
	if (r > 6.283f) r -= 6.283f;
	gameObject->SetRotation(0,r,0);

	this->shader->Bind();
	this->vbo->Bind();
	this->ibo->Bind();
	//if (this->gameObject->transformChanged) 
		this->shader->SetUniform4x4("model", this->gameObject->ModelMatrix());
	this->ibo->Draw();
}

void MeshRenderer::CleanUp()
{

}