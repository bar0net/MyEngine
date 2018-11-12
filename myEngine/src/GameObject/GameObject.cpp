#include "GameObject.h"

#include "LogSystem.h"
#include "Globals.h"

#include "Components/Component.h"
#include "_Vendor/MathGeoLib/Math/MathAll.h"


GameObject::~GameObject()
{
	for (std::unordered_map<ComponentType, std::vector<Component*>>::iterator it = components.begin(); it != components.end(); ++it)
	{
		for (std::vector<Component*>::iterator jt = it->second.begin(); jt != it->second.end(); ++jt)
			delete *jt;
		it->second.clear();
	}
	components.clear();
}

void GameObject::Init()
{
	for (std::unordered_map<ComponentType, std::vector<Component*>>::iterator it = components.begin(); it != components.end(); ++it)
	{
		for (std::vector<Component*>::iterator jt = it->second.begin(); jt != it->second.end(); ++jt)
			(*jt)->Init();
	}
}

void GameObject::Start()
{
	for (std::unordered_map<ComponentType, std::vector<Component*>>::iterator it = components.begin(); it != components.end(); ++it)
	{
		for (std::vector<Component*>::iterator jt = it->second.begin(); jt != it->second.end(); ++jt)
			(*jt)->Start();
	}
}

void GameObject::Update()
{
	for (std::unordered_map<ComponentType, std::vector<Component*>>::iterator it = components.begin(); it != components.end(); ++it)
	{
		for (std::vector<Component*>::iterator jt = it->second.begin(); jt != it->second.end(); ++jt)
			(*jt)->Update();
	}
}

void GameObject::End()
{
	for (std::unordered_map<ComponentType, std::vector<Component*>>::iterator it = components.begin(); it != components.end(); ++it)
	{
		for (std::vector<Component*>::iterator jt = it->second.begin(); jt != it->second.end(); ++jt)
			(*jt)->End();
	}
}

void GameObject::CleanUp()
{
	for (std::unordered_map<ComponentType, std::vector<Component*>>::iterator it = components.begin(); it != components.end(); ++it)
	{
		for (std::vector<Component*>::iterator jt = it->second.begin(); jt != it->second.end(); ++jt)
		{
			(*jt)->CleanUp();
			delete(*jt);
			*jt = nullptr;
		}
		it->second.clear();
	}
	components.clear();
}

void GameObject::SetModelMatrix(math::float4x4 * transform)
{
	this->transform = *transform;

	this->position = transform->TranslatePart();
	this->rotation = math::Quat(*transform);
	this->scale = transform->GetScale();

	transformChanged = true;
}

void GameObject::AddComponent(Component* const component)
{
	if (components.find(component->GetType()) == components.end())
	{
		LOGINFO("Adding component %s to gameObject %s", component->GetName(), name.c_str());
		component->SetGameObject(this);
		components[component->GetType()].push_back(component);
		component->Init();
	}
	else
	{
		LOGWARNING("Cannot add component %s to gameObject %s. This gameObject already has a component with that name.", component->GetName(), name);
	}
}

void GameObject::SetPosition(float x, float y, float z)
{
	position = { x, y, z };
	transform.SetTranslatePart({ x,y,z });
	transformChanged = true;
}

void GameObject::SetPosition(float3 position)
{
	this->position = position;
	transform.SetTranslatePart(position);
	transformChanged = true;
}

// It's important to use FromEulerZYX so we can easyly implement rotations.
void GameObject::SetRotation(float x, float y, float z)
{
	rotation = Quat::FromEulerZYX(z * DEG2RAD, y * DEG2RAD, x * DEG2RAD);
	transform = math::float4x4::FromTRS(position, rotation, scale);
	transformChanged = true;
}

void GameObject::SetScale(float x, float y, float z)
{
	scale = { x, y, z };
	transform = math::float4x4::FromTRS(position, rotation, scale);
	transformChanged = true;
}

void GameObject::Translate(math::float3 direction)
{
	SetPosition(position.x + direction.x, position.y + direction.y, position.z + direction.z);
}

void GameObject::Translate(float x, float y, float z)
{
	SetPosition(position.x + x, position.y + y, position.z + z);
}

void GameObject::Rotate(float x, float y, float z)
{
	rotation = rotation * Quat::RotateZ(z*DEG2RAD) * Quat::RotateY(y*DEG2RAD) * Quat::RotateX(x*DEG2RAD);
	
	transform = math::float4x4::FromTRS(position, rotation, scale);
	transformChanged = true;
}

math::float3 GameObject::Up()
{
	return (transform * math::float4::unitY).Float3Part();
}

math::float3 GameObject::Right()
{
	return (transform * math::float4::unitX).Float3Part();
}

math::float3 GameObject::Front()
{
	return -(transform * math::float4::unitZ).Float3Part();
}
