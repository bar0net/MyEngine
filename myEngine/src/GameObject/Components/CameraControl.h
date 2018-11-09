#ifndef _MYENGINE_CAMERACONTROL_H
#define _MYENGINE_CAMERACONTROL_H

#include "../Component.h"

class CameraControl : public Component
{
public:
	CameraControl() : Component("CameraControl") {};

	void Update();

	float enabled = false;
	float mouse_enabled = false;
	float velocity = 4.0f;
	float angularVelocity = 90.0f;
};

#endif // !_MYENGINE_CAMERACONTROL_H

