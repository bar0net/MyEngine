#ifndef _PANEL_TEXTURE_H
#define _PANEL_TEXTURE_H

#include "_Vendor/imgui-docking/imgui.h"

#include "Application.h"
#include "Module/ModuleTexture.h"


class PanelTexture
{
public:
	void Draw(bool &enabled)
	{
		ImGui::Begin("Textures", &enabled);

		unsigned int toDelete = 0;
		for (std::unordered_map<std::string, unsigned int>::iterator it = App->texture->file2texture.begin(); it != App->texture->file2texture.end(); ++it)
		{
			ImGui::Image((ImTextureID)(it->second), ImVec2(30, 30)); 
			ImGui::SameLine();
			ImGui::Text("'%s'", it->first.c_str()); 
			
			if (App->texture->checkers == it->second) continue;
			ImGui::SameLine();
			if (ImGui::Button("Delete")) toDelete = it->second;
		}

		if (toDelete > 0) App->texture->DeleteTexture(toDelete);
		ImGui::End();
	}
};

#endif // !_PANEL_TEXTURE_H

