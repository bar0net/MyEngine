#ifndef _MYENGINE_MESHRENDERER_H
#define _MYENGINE_MESHRENDERER_H

#include "../Component.h"

#include <vector>

namespace MyEngine 
{
	class VertexBuffer;
	class IndexBuffer;
	class Shader;
	class VertexBufferLayout;
	class VertexArray;
}

class MeshRenderer : public Component
{
public:
	MeshRenderer(const std::vector<float>* vertices, const MyEngine::VertexBufferLayout* layout, const std::vector<unsigned int>* indices, MyEngine::Shader* shader);
	virtual ~MeshRenderer();

	void Update();
	void CleanUp();

	MyEngine::Shader *shader;
	MyEngine::VertexBuffer *vbo;
	MyEngine::IndexBuffer *ibo;
	MyEngine::VertexArray *vao;
};

#endif // !_MYENGINE_MESHRENDERER_H

