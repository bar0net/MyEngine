#ifndef _MODULE_EDITOR_H
#define _MODULE_EDITOR_H

#include "Module.h"

struct ImGuiIO;
class MovingArray;

namespace MyEngine
{
	class IndexBuffer;
	class VertexBuffer;
	class Shader;
}

class ModuleEditor : public Module
{
public:
	bool Init();
	bool Start();
	UpdateState PreUpdate();
	UpdateState Update();
	bool CleanUp();
	
	void ProcessEvent(void* event);

private:
	void FrameStart();
	void FrameEnd();

	ImGuiIO* io;
	MovingArray* fps;

	MyEngine::IndexBuffer* ibo_grid = nullptr;
	MyEngine::VertexBuffer* vbo_grid = nullptr;
	MyEngine::Shader* shader_grid = nullptr;
	bool show_grid = true;
};


#endif // !_MODULE_EDITOR_H

