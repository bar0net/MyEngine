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

	bool Init() override;
	UpdateState PreUpdate() override;
	UpdateState Update() override;
	UpdateState PostUpdate() override;
	bool CleanUp() override;

	void Draw(const MyEngine::VertexArray* vao, const MyEngine::IndexBuffer* ibo, const MyEngine::Shader* shader) const;
	void DrawLines(const MyEngine::VertexArray* vao, const MyEngine::IndexBuffer* ibo, const MyEngine::Shader* shader, float line_width = 1.0f) const;
	void DrawPoints(const MyEngine::VertexArray * vao, const MyEngine::IndexBuffer * ibo, const MyEngine::Shader * shader, float point_size = 1.0f) const;

	MyEngine::Shader* CreateShader(const char* name, const char* vShader_file, const char* fShader_file);
	MyEngine::Shader* GetShader(const char* name);
	void EmptyShaders();
	void ResizedWindow();
	void EnableVSync(bool enabled);

	void UpdateClearColor() const;
	void UpdateClearColor(float r, float g, float b, float a);

	MyEngine::WindowData* data = nullptr;
	std::unordered_map<const char*, MyEngine::Shader*> shaders;

	int width = 1280;
	int height = 720;

	bool vsyncEnabled = true;
	bool showWireframe = false;
	float clearColor[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
	float wireColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
};


#endif //!_MYENGINE_RENDERER_H