#ifndef _MODULE_EDITOR_H
#define _MODULE_EDITOR_H

#include "Module.h"

#include "Editor/PanelPerformance.h"
#include "Editor/PanelConsole.h"
#include "Editor/PanelEditor.h"

struct ImGuiIO;
class MovingArray;

namespace MyEngine
{
	class IndexBuffer;
	class VertexBuffer;
	class VertexArray;
	class Shader;
	class Texture2D;
	class FrameBuffer;
	class RenderBuffer;
}

class GameObject;
class Camera;
class CameraControl;
class MeshRenderer;

class ModuleEditor : public Module
{
public:
	bool Init();
	bool Start();
	UpdateState PreUpdate();
	UpdateState Update();
	bool CleanUp();
	
	void ProcessEvent(void* event) const;

	MyEngine::FrameBuffer* frameBuffer;
	MyEngine::RenderBuffer* renderBuffer;
	MyEngine::Texture2D* renderTexture;

private:
	void FrameStart();
	void FrameEnd() const;

	void PanelObjects();

	void CreateGrid();
	void CreateGizmo();

	void MainMenuBar();
	void PanelCamera(Camera* component) const;
	void PanelCameraControl(CameraControl* component) const;
	void PanelMeshRenderer(MeshRenderer* component) const;

public:
	// ===== Grid =====
	float grid_color[4] = { 1.0f,1.0f,1.0f,1.0f };
	MyEngine::VertexArray* vao_grid = nullptr;
	MyEngine::IndexBuffer* ibo_grid = nullptr;
	MyEngine::VertexBuffer* vbo_grid = nullptr;
	MyEngine::Shader* shader_grid = nullptr;

	// ===== Origin Gizmo =====
	MyEngine::VertexArray* vao_gizmo = nullptr;
	MyEngine::IndexBuffer* ibo_gizmo = nullptr;
	MyEngine::VertexBuffer* vbo_gizmo = nullptr;
	MyEngine::Shader* shader_gizmo = nullptr;


private:
	ImGuiIO* io = nullptr;
	float scene_width = 0;
	float scene_height = 0;
	GameObject* inspect_object = nullptr;

	// Panels
	PanelPerfomance* panel_performance;
	PanelConsole* panel_console;
	PanelEditor* panel_editor;

	bool debug_window = true;
	bool config_window = true;
	bool inspect_window = true;
	bool console_window = true;
	bool scene_window = true;

	bool show_info = true;
	bool show_debug = true;
	bool show_warning = true;
	bool show_error = true;
};




#endif // !_MODULE_EDITOR_H

