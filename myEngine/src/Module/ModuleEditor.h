#ifndef _MODULE_EDITOR_H
#define _MODULE_EDITOR_H

#include "Module.h"

struct ImGuiIO;
class MovingArray;

namespace MyEngine
{
	class IndexBuffer;
	class VertexBuffer;
	class VertexArray;
	class Shader;
}

class GameObject;
class Camera;
class CameraControl;

class ModuleEditor : public Module
{
public:
	bool Init();
	bool Start();
	UpdateState PreUpdate();
	UpdateState Update();
	bool CleanUp();
	
	void ProcessEvent(void* event) const;

private:
	void FrameStart();
	void FrameEnd() const;

	void PanelPerformance() const;
	void PanelEditor();
	void PanelObjects();

	void PanelCamera(Camera* component) const;
	void PanelCameraControl(CameraControl* component) const;

	ImGuiIO* io = nullptr;
	MovingArray* fps = nullptr;
	MovingArray* avg_ms_array = nullptr;
	unsigned int avg_ms = 0;

	MyEngine::VertexArray* vao_grid = nullptr;
	MyEngine::IndexBuffer* ibo_grid = nullptr;
	MyEngine::VertexBuffer* vbo_grid = nullptr;
	MyEngine::Shader* shader_grid = nullptr;
	bool show_grid = true;
	float grid_color[4] = { 1.0f,1.0f,1.0f,1.0f };

	bool debug_window = true;
	bool config_window = true;
	bool inspect_window = true;
	GameObject* inspect_object = nullptr;
};




#endif // !_MODULE_EDITOR_H

