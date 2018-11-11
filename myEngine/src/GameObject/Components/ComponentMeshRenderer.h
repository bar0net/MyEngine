#ifndef _MYENGINE_MESHRENDERER_H
#define _MYENGINE_MESHRENDERER_H

#include "Component.h"

#include <vector>

struct Model;

namespace MyEngine 
{
	class VertexBuffer;
	class IndexBuffer;
	class Shader;
	class VertexBufferLayout;
	class VertexArray;
}

struct Mesh
{
	MyEngine::VertexBuffer *vbo;
	MyEngine::IndexBuffer *ibo;
	MyEngine::VertexArray *vao;
	unsigned int num_triangles;
	unsigned int textureID;
	bool display_texture = true;
	unsigned int polygon = 3;
	float albedo[4] = { 1.0F, 1.0F, 1.0F, 1.0F };
};

class MeshRenderer : public Component
{
public:
	MeshRenderer(const std::vector<Model>& models, MyEngine::Shader* shader);
	virtual ~MeshRenderer();

	void Update() override;
	void CleanUp() override;

	std::vector<Mesh*> meshes;
	MyEngine::Shader *shader;

	float center[3]		= { 0, 0, 0 };
	float dimensions[3]	= { 0, 0, 0 };
};

#endif // !_MYENGINE_MESHRENDERER_H
