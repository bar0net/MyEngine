#ifndef _PANEL_COMPONENT_CAMERA_CONTROL_H
#define _PANEL_COMPONENT_CAMERA_CONTROL_H

#include "_Vendor/imgui-docking/imgui.h"
#include "GameObject/Components/ComponentCameraControl.h"

class PanelCameraControl
{
public:
	static void Draw(CameraControl* component)
	{
		assert(component);
		if (ImGui::CollapsingHeader(component->GetName()))
		{
			ImGui::InputFloat("Velocity", &component->velocity);
			ImGui::InputFloat("Angular Velocity", &component->angularVelocity);
		}
	}
};

#endif // !_PANEL_COMPONENT_CAMERA_CONTROL_H

