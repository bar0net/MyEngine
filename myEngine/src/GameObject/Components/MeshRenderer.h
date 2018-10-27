#ifndef _MYENGINE_MESHRENDERER_H
#define _MYENGINE_MESHRENDERER_H

#include "../Component.h"

#include <vector>

namespace MyEngine 
{
	class VertexBuffer;
	class IndexBuffer;
	class Shader;
}

class MeshRenderer : public Component
{
public:
	MeshRenderer(const std::vector<float>* vertices, const std::vector<unsigned int>* indices, MyEngine::Shader* shader);
	virtual ~MeshRenderer();

	void Update();

	MyEngine::Shader *shader;
	MyEngine::VertexBuffer *vbo;
	MyEngine::IndexBuffer *ibo;
};

#endif // !_MYENGINE_MESHRENDERER_H

