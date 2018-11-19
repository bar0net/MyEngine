#include "ComponentCamera.h"

#include "_Vendor/MathGeoLib/Math/float3.h"

#include "Application.h"
#include "Globals.h"
#include "GL_Buffers/Shader.h"
#include "GL_Buffers/UniformBuffer.h"
#include "Module/ModuleRenderer.h"
#include "GameObject/GameObject.h"

Camera::Camera() : Component("Camera")
{
	componentType = ComponentType::CAMERA;
	ubo = new MyEngine::UniformBuffer;
	UpdateFrustum();
}

Camera::Camera(ProjectionType type, float nearPlane, float farPlane, float fov) :
	projectionType(type), nearPlane(nearPlane), farPlane(farPlane), fov(fov), Component("Camera")
{
	componentType = ComponentType::CAMERA;
	UpdateFrustum();
}

Camera::~Camera()
{
	RELEASE(ubo);
}

void Camera::Update() 
{
	if (gameObject->transformChanged) 
	{
		math::float4x4 VM = gameObject->ModelMatrix()->Inverted();
		ubo->SetUniform4x4(SHADER_VIEW_OFFSET, &VM);
		gameObject->transformChanged = false;
	}
}

void Camera::UpdateFrustum()
{
	if (projectionType == ProjectionType::Orthographic) frustum.type = FrustumType::OrthographicFrustum;
	else frustum.type = FrustumType::PerspectiveFrustum;

	frustum.pos = math::float3::zero;
	frustum.front = -math::float3::unitZ;
	frustum.up = math::float3::unitY;

	frustum.nearPlaneDistance = nearPlane;
	frustum.farPlaneDistance = farPlane;

	frustum.horizontalFov = (MYENGINE_PI * fov / 180.0F);
	frustum.verticalFov = 2.0F * atan(tan( frustum.horizontalFov / 2.0F) * ((float)App->renderer->height / (float)App->renderer->width));

	if (gameObject != nullptr)
	{
		math::float4x4 VM = gameObject->ModelMatrix()->Inverted();
		ubo->SetUniform4x4(SHADER_VIEW_OFFSET, &VM);
	}

	ubo->SetUniform4x4(SHADER_PROJ_OFFSET, &frustum.ProjectionMatrix());
}

math::float4x4 Camera::ProjectionMatrix()
{
	return frustum.ProjectionMatrix();
}


void Camera::SetGameObject(GameObject* gameObject) 
{
	Component::SetGameObject(gameObject);

	math::float4x4 VM = gameObject->ModelMatrix()->Inverted();
	ubo->SetUniform4x4(SHADER_VIEW_OFFSET, &VM);
}