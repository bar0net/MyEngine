#ifndef _MYENGINE_RENDERER_H
#define _MYENGINE_RENDERER_H

#include "Module.h"
 
namespace MyEngine 
{
	struct WindowData;
	class IndexBuffer;
	class VertexBuffer;
	class Shader;
}

class ModuleRenderer : 	public Module
{
public:
	ModuleRenderer();
	virtual ~ModuleRenderer();

	bool Init();
	UpdateState PreUpdate();
	UpdateState Update();
	UpdateState PostUpdate();
	bool CleanUp();

	MyEngine::WindowData* data = nullptr;

	unsigned int width = 1280;
	unsigned int height = 720;

	MyEngine::IndexBuffer* ibo = nullptr;
	MyEngine::VertexBuffer* vbo = nullptr;
	MyEngine::Shader* shader = nullptr;
};


#endif //!_MYENGINE_RENDERER_H