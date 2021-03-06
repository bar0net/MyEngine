#ifndef _MODULE_EDITOR_H
#define _MODULE_EDITOR_H

#include "Module.h"

#include "Editor/PanelPerformance.h"
#include "Editor/PanelConsole.h"
#include "Editor/PanelEditor.h"
#include "Editor/PanelScene.h"

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
struct Mesh;

class PanelTexture;

class ModuleEditor : public Module
{
public:
	bool Init() override;
	bool Start() override;
	UpdateState PreUpdate() override;
	UpdateState Update() override;
	bool CleanUp() override;
	
	void ProcessEvent(void* event) const;
	GameObject* GetInspectedObject();
	void SetRenderTexture();

private:
	void FrameStart();
	void FrameEnd() const;

	void CreateGrid();
	void CreateGizmo();

	void CreateDockSpace();
	bool MainMenuBar();

public:
	MyEngine::FrameBuffer* frameBuffer = nullptr;
	MyEngine::RenderBuffer* renderBuffer = nullptr;
	MyEngine::Texture2D* renderTexture = nullptr;

	// ===== Grid =====
	float grid_color[4] = { 1.0f,1.0f,1.0f,1.0f };
	MyEngine::Shader* shader_grid = nullptr;
	Mesh* grid_mesh = nullptr;

	// ===== Origin Gizmo =====
	MyEngine::Shader* shader_gizmo = nullptr;
	Mesh* gizmo_mesh = nullptr;

private:
	ImGuiIO* io = nullptr;
	float scene_width = 0.0F;
	float scene_height = 0.0F;

	GameObject* inspect_object = nullptr;

	// Panels
	PanelPerfomance* panel_performance = nullptr;
	PanelConsole* panel_console = nullptr;
	PanelEditor* panel_editor = nullptr;
	PanelScene* panel_scene = nullptr;

	bool debug_window = true;
	bool config_window = true;
	bool hierarchy_window = true;
	bool inspect_window = true;
	bool console_window = true;
	bool scene_window = true;
	bool texture_window = true;

	bool show_info = true;
	bool show_debug = true;
	bool show_warning = true;
	bool show_error = true;

	GameObject* editor_camera = nullptr;
};




#endif // !_MODULE_EDITOR_H

