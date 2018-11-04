#ifndef _MYENGINE_MESHRENDERER_H
#define _MYENGINE_MESHRENDERER_H

#include "../Component.h"

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
};

class MeshRenderer : public Component
{
public:
	MeshRenderer(const std::vector<Model>& models, MyEngine::Shader* shader);
	virtual ~MeshRenderer();

	void Update();

	std::vector<Mesh> meshes;
	MyEngine::Shader *shader;
};

#endif // !_MYENGINE_MESHRENDERER_H

