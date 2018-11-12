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
struct Mesh;

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

