#ifndef _PANEL_COMPONENT_MESH_RENDERER_H
#define _PANEL_COMPONENT_MESH_RENDERER_H

#include "_Vendor/imgui-docking/imgui.h"

#include "GameObject/GameObject.h"
#include "GameObject/Components/ComponentMeshRenderer.h"

#include "Application.h"
#include "Module/ModuleTexture.h"

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

			static int selected_mesh = -1;
			for (unsigned int i = 0; i < component->meshes.size(); ++i)
			{
				std::string strTextureButton = "Change\nTexture\nMesh" + std::to_string(i);
				
				std::string popupID = std::string("MeshAlbedo") + std::to_string(i);
				std::string albedoID = std::string("Albedo (") + std::to_string(i) + std::string(")");
				std::string popupTextureID = std::string("TextureMenu") + std::to_string(i);


				ImGui::Text("SubMesh (%i)", i);
				ImGui::Text("Numer of Triangles: %d", component->meshes[i]->num_triangles);

				std::string strCheckbox = "Display Textures for Mesh " + std::to_string(i);
				ImGui::Checkbox(strCheckbox.c_str(), &component->meshes[i]->display_texture);

				ImVec4 tint(1, 1, 1, 1);
				if (!component->meshes[i]->display_texture) tint = { 0.6F, 0.6F, 0.6F, 1 };

				ImGui::Image((ImTextureID)component->meshes[i]->textureID, ImVec2(75, 75), ImVec2(0, 0), ImVec2(1, 1), tint);

				/*if (ImGui::ImageButton((ImTextureID)component->meshes[i]->textureID, ImVec2(75, 75), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0), tint))
				{
					selected_mesh = i;
					open_textures = true;
				}*/


				ImGui::SameLine();
				bool open_textures = false;
				if (ImGui::Button(strTextureButton.c_str(), ImVec2(50, 75)))
				{
					open_textures = true;
				}

				ImGui::SameLine();
				bool open_albedo = ImGui::ColorButton(albedoID.c_str(), *(ImVec4*)&component->meshes[i]->albedo, 0, ImVec2(10, 75));
				ImGui::SameLine(); ImGui::Text("Albedo");

				if (open_albedo) ImGui::OpenPopup(popupID.c_str());
				if (ImGui::BeginPopup(popupID.c_str()))
				{
					ImGui::ColorPicker4(albedoID.c_str(), component->meshes[i]->albedo);
					ImGui::EndPopup();
				}

				if (open_textures) ImGui::OpenPopup(popupTextureID.c_str());
				if (ImGui::BeginPopup(popupTextureID.c_str()))
				{
					for (auto it = App->texture->file2texture.begin(); it != App->texture->file2texture.end(); ++it)
					{
						if (ImGui::ImageButton((ImTextureID)it->second->ID(), ImVec2(10, 10)))
						{
							component->meshes[i]->textureID = it->second->ID();
							ImGui::CloseCurrentPopup();
						}
						ImGui::SameLine();
						ImGui::Text(it->first.c_str());
					}
					ImGui::EndPopup();
				}

				ImGui::Separator();
			}	


		}

	}
};

#endif //!_PANEL_COMPONENT_MESH_RENDERER_H
