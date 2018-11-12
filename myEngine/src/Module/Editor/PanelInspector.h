#ifndef _PANEL_INSPECTOR_H
#define _PANEL_INSPECTOR_H

#include "_Vendor/imgui-docking/imgui.h"

#include "Application.h"
#include "GameObject/GameObject.h"
#include "Module/ModuleScene.h"

#include "Components/PanelMeshRenderer.h"
#include "Components/PanelCameraControl.h"
#include "Components/PanelCamera.h"

class PanelInspector
{
public:
	static void Draw(bool* enabled, GameObject*& inspect_object, const GameObject* editor_camera)
	{
		assert(editor_camera && enabled);
		bool delete_object = false;

		ImGui::Begin("Inspector", enabled);
		if (inspect_object != nullptr)
		{
			ImGui::TextColored(ImVec4(0.3F, 0.5F, 0.8F, 1.0F), inspect_object->GetName());

			if (inspect_object != editor_camera)
			{
				ImGui::SameLine(0.0F, 10.0F);
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8F, 0.2F, 0.2F, 1.0F));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.85F, 0.2F, 0.2F, 1.0F));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.85F, 0.2F, 0.2F, 1.0F));
				if (ImGui::SmallButton("Delete"))
				{
					App->scene->DeleteGameObject(inspect_object);
					delete_object = true;
				}
				ImGui::PopStyleColor(3);
			}

			float pos[3] = { inspect_object->Position()[0], inspect_object->Position()[1], inspect_object->Position()[2] };
			if (ImGui::DragFloat3("Position", pos, 2)) inspect_object->SetPosition(pos[0], pos[1], pos[2]);

			float3 rotation = inspect_object->Rotation();
			float rot[3] = { rotation.x, rotation.y, rotation.z };
			if (ImGui::DragFloat3("Rotation", rot, 2)) inspect_object->SetRotation(rot[0], rot[1], rot[2]);

			float scale[3] = { inspect_object->Scale().x, inspect_object->Scale().y, inspect_object->Scale().z };
			if (ImGui::DragFloat3("Scale", scale, 2)) inspect_object->SetScale(scale[0], scale[1], scale[2]);


			ImGui::Separator();
			for (std::unordered_map<ComponentType, std::vector<Component*>>::iterator it = inspect_object->components.begin(); it != inspect_object->components.end(); ++it)
			{
				switch (it->first)
				{
				case ComponentType::CAMERA:
					for (std::vector<Component*>::iterator jt = it->second.begin(); jt != it->second.end(); ++jt)
						PanelCamera::Draw((Camera*)(*jt));
					break;

				case ComponentType::CAMERA_CONTROL:
					for (std::vector<Component*>::iterator jt = it->second.begin(); jt != it->second.end(); ++jt)
						PanelCameraControl::Draw((CameraControl*)(*jt));
					break;

				case ComponentType::MESH_RENDERER:
					for (std::vector<Component*>::iterator jt = it->second.begin(); jt != it->second.end(); ++jt)
						PanelMeshRenderer::Draw((MeshRenderer*)(*jt));
					break;

				default:
					for (std::vector<Component*>::iterator jt = it->second.begin(); jt != it->second.end(); ++jt)
						ImGui::Text((*jt)->GetName());
					break;
				}
			}

		}
		ImGui::End();

		if (delete_object) 
			inspect_object = nullptr;
	}
};

#endif // !_PANEL_INSPECTOR_H

