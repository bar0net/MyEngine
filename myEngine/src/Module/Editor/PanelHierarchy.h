#ifndef _PANEL_HIERARCHY_H
#define _PANEL_HIERARCHY_H

#include "_Vendor/imgui-docking/imgui.h"
#include "GameObject/GameObject.h"

#include "Application.h"
#include "Module/ModuleEditor.h"

class PanelHierarchy
{
public:
	static void Draw(bool* enabled, GameObject*& inspect_object)
	{
		assert(enabled);
		GameObject* output = nullptr;

		ImGui::Begin("Hierarchy", enabled);
		if (App->scene->gameObjects.size() != 0)
		{
			ImVec2 panel_size = ImGui::GetWindowSize();

			ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0, 0));
			for (std::unordered_map<std::string, GameObject*>::iterator it = App->scene->gameObjects.begin(); it != App->scene->gameObjects.end(); ++it)
			{
				if (inspect_object == it->second)
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2F, 0.3F, 0.7F, 1.0F));
				else
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0F, 0.0F, 0.0F, 0.0F));

				if (ImGui::Button(it->first.c_str(), ImVec2(panel_size.x - 17, 0)))
					inspect_object = it->second;

				ImGui::PopStyleColor();
			}
			ImGui::PopStyleVar();
		}
		ImGui::End();

		//return output;
	}
};

#endif // !_PANEL_HIERARCHY_H

