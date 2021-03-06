#ifndef _MYENGINE_CAMERACONTROL_H
#define _MYENGINE_CAMERACONTROL_H

#include "Component.h"

class CameraControl : public Component
{
public:
	CameraControl() : Component("CameraControl") { componentType = ComponentType::CAMERA_CONTROL; };

	void Update() override;

	bool enabled = false;
	bool mouse_enabled = false;
	float velocity = 4.0f;
	float angularVelocity = 90.0f;
	bool worldYorbit = true;

private:
	float ratio = 1;
};

#endif // !_MYENGINE_CAMERACONTROL_H

