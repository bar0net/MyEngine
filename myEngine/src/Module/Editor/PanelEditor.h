#ifndef _PANEL_EDITOR_H
#define _PANEL_EDITOR_H

#include "_Vendor/imgui-docking/imgui.h"

#include "Application.h"
#include "Module/ModuleRenderer.h"
#include "GL_Buffers/Shader.h"

class PanelEditor
{
public:
	void Draw(bool& enabled, MyEngine::Shader* shader_grid, float* grid_color)
	{
		ImGui::Begin("Configuration", &enabled);

		// Background Color
		ImVec4 bgcolor(App->renderer->clearColor[0], App->renderer->clearColor[1], App->renderer->clearColor[2], App->renderer->clearColor[3]);
		bool open_bgcolor = ImGui::ColorButton("BGButton", bgcolor);
		ImGui::SameLine(); ImGui::Text("Background Color");
		if (open_bgcolor) ImGui::OpenPopup("BackgroundPicker");
		if (ImGui::BeginPopup("BackgroundPicker"))
		{
			if (ImGui::ColorPicker4("Background Color", App->renderer->clearColor)) App->renderer->UpdateClearColor();
			ImGui::EndPopup();
		}

		// Grid Color
		bool open_gridcolor = ImGui::ColorButton("GridColorButton", *(ImVec4*)&grid_color);
		ImGui::SameLine(); ImGui::Text("Grid Color");
		if (open_gridcolor) ImGui::OpenPopup("GridColorPicker");
		if (ImGui::BeginPopup("GridColorPicker"))
		{
			if (ImGui::ColorPicker4("Grid Color", grid_color))
				shader_grid->SetUniform4("albedo", grid_color[0], grid_color[1], grid_color[2], grid_color[3]);
			ImGui::EndPopup();
		}

		ImGui::Checkbox("Show Grid", &show_grid);
		ImGui::Checkbox("Wireframe", &App->renderer->showWireframe);

		ImGui::End();
	}


	bool show_grid = true;
};

#endif // !_PANEL_EDITOR_H

