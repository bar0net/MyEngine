#ifndef PANEL_SCENE_H
#define PANEL_SCENE_H

#include "_Vendor/imgui-docking/imgui.h"

#include "Application.h"
#include "Module/ModuleRenderer.h"
#include "GameObject/Components/ComponentCameraControl.h"

class PanelScene
{
public:
	PanelScene() {};
	~PanelScene() {};

	void Draw(bool &enabled, unsigned int renderTextureID, CameraControl* control, float& scene_width, float& scene_height)
	{
		assert(control);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 10));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		ImGui::Begin("Scene", &enabled, ImGuiWindowFlags_NoScrollbar);
		ImVec2 size = ImGui::GetContentRegionAvail();
		ImVec2 imgSize = size;
		float x_space = 0;
		float y_space = 0;

		if (size.x * App->renderer->height < size.y * App->renderer->width)
		{
			imgSize.y = (size.x * App->renderer->height / App->renderer->width);
			y_space = (size.y - imgSize.y) / 2.0F;
			ImGui::Dummy(ImVec2(0, y_space));
		}
		else
		{
			imgSize.x = (size.y * App->renderer->width / App->renderer->height);
			x_space = (size.x - imgSize.x) / 2.0F;
			ImGui::Dummy(ImVec2(x_space, 0)); ImGui::SameLine();
		}

		ImGui::Image((ImTextureID)renderTextureID, imgSize, ImVec2(0, 1), ImVec2(1, 0));

		scene_width = imgSize.x;
		scene_height = imgSize.y;

		ImVec2 wPos = ImGui::GetWindowPos();
		control->enabled = ImGui::IsWindowFocused();
		control->mouse_enabled = ImGui::IsMouseHoveringRect(ImVec2(wPos.x + x_space, wPos.y + y_space), ImVec2(wPos.x + x_space + scene_width, wPos.y + y_space + scene_height));

		ImGui::End();
		ImGui::PopStyleVar(2);
	}

	CameraControl* control = nullptr;
};

#endif // !PANEL_SCENE_H

