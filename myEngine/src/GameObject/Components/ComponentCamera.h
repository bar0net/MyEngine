#ifndef _MYENGINE_COMPONENT_CAMERA_H
#define _MYENGINE_COMPONENT_CAMERA_H

#include "Component.h"

#include "_Vendor/MathGeoLib/Geometry/Frustum.h"
#include "_Vendor/MathGeoLib/Math/float4x4.h"

namespace MyEngine { class UniformBuffer; }

enum class ProjectionType
{
	Prespective = 1,
	Orthographic = 2
};

class Camera : public Component
{
public:
	Camera();
	Camera(ProjectionType type, float nearPlane, float farPlane, float fov);
	virtual ~Camera();

	void Init() {}
	void Update();

	void SetGameObject(GameObject* gameObject);

	void UpdateFrustum();
	math::float4x4 ProjectionMatrix();

	bool projectionChanged = true;

	ProjectionType type = ProjectionType::Prespective;
	float nearPlane = 0.5f;
	float farPlane = 100.0f;
	float fov = 90;

private:
	math::Frustum frustum;
	MyEngine::UniformBuffer* ubo = nullptr;
};

#endif // !_MYENGINE_COMPONENT_CAMERA_H
