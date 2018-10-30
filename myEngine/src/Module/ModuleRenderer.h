#ifndef _MYENGINE_RENDERER_H
#define _MYENGINE_RENDERER_H

#include "Module.h"
#include <unordered_map>
 
namespace MyEngine 
{
	struct WindowData;
	class IndexBuffer;
	class VertexBuffer;
	class VertexArray;
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
	void Draw(MyEngine::VertexArray* vao, MyEngine::IndexBuffer* ibo, MyEngine::Shader* shader);

	void CreateShader(const char* name, const char* vShader_file, const char* fShader_file);
	MyEngine::Shader* GetShader(const char* name);
	void EmptyShaders();
	void ResizedWindow();

	MyEngine::WindowData* data = nullptr;
	std::unordered_map<const char*, MyEngine::Shader*> materials;

	int width = 1280;
	int height = 720;

	int active_vao = -1;
	int active_ibo = -1;
	int active_shader = -1;
};


#endif //!_MYENGINE_RENDERER_H