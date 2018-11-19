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

	void Update() override;

	void SetGameObject(GameObject* gameObject);

	void UpdateFrustum();
	math::float4x4 ProjectionMatrix();

	bool projectionChanged = true;

	ProjectionType projectionType = ProjectionType::Prespective;
	float nearPlane = 0.5F;
	float farPlane = 1000.0F;
	float fov = 90.0F;

private:
	math::Frustum frustum;
	MyEngine::UniformBuffer* ubo = nullptr;
};

#endif // !_MYENGINE_COMPONENT_CAMERA_H
