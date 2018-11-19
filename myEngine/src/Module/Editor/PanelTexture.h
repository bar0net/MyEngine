#ifndef _PANEL_TEXTURE_H
#define _PANEL_TEXTURE_H

#include "_Vendor/imgui-docking/imgui.h"

#include "Application.h"
#include "Module/ModuleTexture.h"
#include "GL_Buffers/Texture2D.h"

class PanelTexture
{
public:
	static void Draw(bool &enabled)
	{
		ImGui::Begin("Textures", &enabled);

		unsigned int toDelete = 0U;
		for (std::unordered_map<std::string, MyEngine::Texture2D*>::iterator it = App->texture->file2texture.begin(); it != App->texture->file2texture.end(); ++it)
		{
			ImGui::Image((ImTextureID)(it->second->ID()), ImVec2(30, 30)); 
			ImGui::SameLine();
			ImGui::Text("'%s'", it->first.c_str()); 
			
			if (App->texture->checkers == it->second->ID()) continue;
			ImGui::SameLine();
			std::string s = "Delete Texture " + std::to_string(it->second->ID());
			if (ImGui::Button(s.c_str())) toDelete = it->second->ID();
		}

		if (toDelete > 0) App->texture->DeleteTexture(toDelete);
		ImGui::End();
	}
};

#endif // !_PANEL_TEXTURE_H

