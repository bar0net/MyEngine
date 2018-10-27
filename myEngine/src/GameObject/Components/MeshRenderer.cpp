#include "MeshRenderer.h"

#include "../../Utils/Shader.h"
#include "../../Utils/VertexBuffer.h"
#include "../../Utils/IndexBuffer.h"

MeshRenderer::MeshRenderer(const std::vector<float>* vertices, const std::vector<unsigned int>* indices, MyEngine::Shader* shader)
{
	this->vbo = new MyEngine::VertexBuffer(vertices);
	this->ibo = new MyEngine::IndexBuffer(indices);
	this->shader = shader;
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
	this->shader->Bind();
	this->vbo->Bind();
	this->ibo->Bind();
	this->ibo->Draw();
}