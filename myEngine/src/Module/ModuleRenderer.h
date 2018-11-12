#ifndef _MYENGINE_RENDERER_H
#define _MYENGINE_RENDERER_H


#include "Module.h"

#include <unordered_map>
#include <vector>

//#include "_Vendor/MathGeoLib/Math/float4x4.h"

namespace MyEngine 
{
	struct WindowData;
	class IndexBuffer;
	class VertexBuffer;
	class VertexArray;
	class Shader;
}

namespace math
{
	class float4x4;
}



struct Mesh
{
	MyEngine::VertexBuffer *vbo = nullptr;
	MyEngine::IndexBuffer *ibo = nullptr;
	MyEngine::VertexArray *vao = nullptr;
	unsigned int num_triangles = 0;
	unsigned int textureID = 0;
	bool display_texture = true;
	unsigned int polygon = 3;
	float albedo[4] = { 1.0F, 1.0F, 1.0F, 1.0F };
};


struct DrawCall
{
public:
	DrawCall(const Mesh* mesh, math::float4x4* transform, const MyEngine::Shader* shader, float size = 0);

	const Mesh* mesh = nullptr;
	math::float4x4* transform = nullptr;
	const MyEngine::Shader* shader = nullptr;
	float size = 0;
};


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

	void Render();

	void Draw(const Mesh* mesh, math::float4x4* transform, const MyEngine::Shader* shader, float size = 0);
	//void Draw(const MyEngine::VertexArray* vao, const MyEngine::IndexBuffer* ibo, const MyEngine::Shader* shader);
	//void DrawLines(const MyEngine::VertexArray* vao, const MyEngine::IndexBuffer* ibo, const MyEngine::Shader* shader, float line_width = 1.0f);
	//void DrawPoints(const MyEngine::VertexArray * vao, const MyEngine::IndexBuffer * ibo, const MyEngine::Shader * shader, float point_size = 1.0f);

	MyEngine::Shader* CreateShader(const char* name, const char* vShader_file, const char* fShader_file);
	MyEngine::Shader* GetShader(const char* name);
	void EmptyShaders();
	void ResizedWindow();
	void EnableVSync(bool enabled);

	void UpdateClearColor() const;
	void UpdateClearColor(float r, float g, float b, float a);

public:
	MyEngine::WindowData* data = nullptr;
	std::unordered_map<const char*, MyEngine::Shader*> shaders;
	std::vector<DrawCall> drawCalls;

	int width = 1280;
	int height = 720;

	bool vsyncEnabled = true;
	bool showWireframe = false;
	float clearColor[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
	float wireColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
};


#endif //!_MYENGINE_RENDERER_H