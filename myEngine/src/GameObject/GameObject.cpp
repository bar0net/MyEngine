#include "GameObject.h"

#include "Component.h"
#include "../_Vendor/MathGeoLib/Math/MathAll.h"


GameObject::~GameObject()
{
	for (std::list<Component*>::iterator it = components.begin(); it != components.end(); ++it)
		delete(*it);

	components.clear();
}

void GameObject::Init()
{
	for (std::list<Component*>::iterator it = components.begin(); it != components.end(); ++it)
		(*it)->Init();
}

void GameObject::Start()
{
	for (std::list<Component*>::iterator it = components.begin(); it != components.end(); ++it)
		(*it)->Start();
}

void GameObject::Update()
{
	for (std::list<Component*>::iterator it = components.begin(); it != components.end(); ++it)
		(*it)->Update();
}

void GameObject::End()
{
	for (std::list<Component*>::iterator it = components.begin(); it != components.end(); ++it)
		(*it)->End();
}

void GameObject::CleanUp()
{
	for (std::list<Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		(*it)->CleanUp();
		delete(*it);
	}

	components.clear();
}

void GameObject::AddComponent(Component* component)
{
	component->SetGameObject(this);
	components.push_back(component);
	component->Init();
}

void GameObject::SetPosition(float x, float y, float z)
{
	position = { x, y, z };
	transform = math::float4x4::FromTRS(position, Quat::FromEulerXYZ(rotation.x, rotation.y, rotation.z), scale);
	transformChanged = true;
}

void GameObject::SetRotation(float x, float y, float z)
{
	rotation = { x, y, z };
	transform = math::float4x4::FromTRS(position, Quat::FromEulerXYZ(rotation.x, rotation.y, rotation.z), scale);
	transformChanged = true;
}

void GameObject::SetScale(float x, float y, float z)
{
	scale = { x, y, z };
	transform = math::float4x4::FromTRS(position, Quat::FromEulerXYZ(rotation.x, rotation.y, rotation.z), scale);
	transformChanged = true;
}