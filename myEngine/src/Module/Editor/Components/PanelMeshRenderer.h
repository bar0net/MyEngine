#ifndef _PANEL_COMPONENT_MESH_RENDERER_H
#define _PANEL_COMPONENT_MESH_RENDERER_H

#include "_Vendor/imgui-docking/imgui.h"

#include "GameObject/GameObject.h"
#include "GameObject/Components/ComponentMeshRenderer.h"

class PanelMeshRenderer
{
public:
	static void Draw(MeshRenderer* component)
	{
		assert(component);

		if (ImGui::CollapsingHeader(component->GetName()))
		{
			ImGui::Text("Bounding Box");
			ImGui::Text("Local Center: (%f, %f, %f)",
				component->center[0] * component->GetGameObject()->Scale().x,
				component->center[1] * component->GetGameObject()->Scale().y,
				component->center[2] * component->GetGameObject()->Scale().z);
			ImGui::Text("Width: %f, Height: %f, Depth: %f",
				component->dimensions[0] * component->GetGameObject()->Scale().x,
				component->dimensions[1] * component->GetGameObject()->Scale().y,
				component->dimensions[2] * component->GetGameObject()->Scale().z);
			ImGui::Separator();

			for (unsigned int i = 0; i < component->meshes.size(); ++i)
			{
				ImGui::Text("SubMesh (%i)", i);
				ImGui::Text("Numer of Triangles: %d", component->meshes[i]->num_triangles);
				std::string s = "Display Texture " + std::to_string(i);
				ImGui::Checkbox(s.c_str(), &component->meshes[i]->display_texture);
				if (component->meshes[i]->display_texture)
					ImGui::Image((ImTextureID)component->meshes[i]->textureID, ImVec2(75, 75), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1));
				else
					ImGui::Image((ImTextureID)component->meshes[i]->textureID, ImVec2(75, 75), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0.6F, 0.6F, 0.6F, 1));

				ImGui::SameLine();

				std::string popupID = std::string("MeshAlbedo") + std::to_string(i);
				std::string albedoID = std::string("Albedo (") + std::to_string(i) + std::string(")");

				bool open_albedo = ImGui::ColorButton(albedoID.c_str(), *(ImVec4*)&component->meshes[i]->albedo, 0, ImVec2(10, 75));
				ImGui::SameLine(); ImGui::Text("Albedo");

				if (open_albedo) ImGui::OpenPopup(popupID.c_str());
				if (ImGui::BeginPopup(popupID.c_str()))
				{
					ImGui::ColorPicker4(albedoID.c_str(), component->meshes[i]->albedo);
					ImGui::EndPopup();
				}

				ImGui::Separator();
			}
		}
	}
};

#endif //!_PANEL_COMPONENT_MESH_RENDERER_H
