#ifndef _PANEL_COMPONENT_CAMERA_H
#define _PANEL_COMPONENT_CAMERA_H

#include "_Vendor/imgui-docking/imgui.h"
#include "GameObject/Components/ComponentCamera.h"

class PanelCamera
{
public:
	static void Draw(Camera* component)
	{
		assert(component);
		if (ImGui::CollapsingHeader("Camera Properties"))
		{
			if (ImGui::InputFloat("Near Plane", &component->nearPlane)) component->UpdateFrustum();
			if (ImGui::InputFloat("Far Plane", &component->farPlane)) component->UpdateFrustum();
			if (ImGui::SliderFloat("FOV", &component->fov, 60, 120)) component->UpdateFrustum();
		}
	}
};


#endif // !_PANEL_COMPONENT_CAMERA_H
