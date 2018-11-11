#ifndef _MYENGINE_CAMERACONTROL_H
#define _MYENGINE_CAMERACONTROL_H

#include "Component.h"

class CameraControl : public Component
{
public:
	CameraControl() : Component("CameraControl") {};

	void Update();

	bool enabled = false;
	bool mouse_enabled = false;
	float velocity = 4.0f;
	float angularVelocity = 90.0f;

private:
	float ratio = 1;
};

#endif // !_MYENGINE_CAMERACONTROL_H

