#include "GameObject.h"

#include "Component.h"
#include "../_Vendor/MathGeoLib/Math/MathAll.h"
#include "LogSystem.h"


GameObject::~GameObject()
{

}

void GameObject::Init()
{
	for (std::unordered_map<const char*, Component*>::iterator it = components.begin(); it != components.end(); ++it)
		it->second->Init();
}

void GameObject::Start()
{
	for (std::unordered_map<const char*, Component*>::iterator it = components.begin(); it != components.end(); ++it)
		it->second->Start();
}

void GameObject::Update()
{
	for (std::unordered_map<const char*, Component*>::iterator it = components.begin(); it != components.end(); ++it)
		it->second->Update();
}

void GameObject::End()
{
	for (std::unordered_map<const char*, Component*>::iterator it = components.begin(); it != components.end(); ++it)
		it->second->End();
}

void GameObject::CleanUp()
{
	for (std::unordered_map<const char*, Component*>::iterator it = components.begin(); it != components.end(); ++it)
	{
		it->second->CleanUp();
		delete(it->second);
	}

	components.clear();
}

void GameObject::AddComponent(Component* component)
{
	if (components.find(component->GetName()) == components.end())
	{
		LOGINFO("Adding component %s to gameObject %s", component->GetName(), name);
		component->SetGameObject(this);
		components[component->GetName()] = component;
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
	transform = math::float4x4::FromTRS(position, Quat::FromEulerXYZ(DEG2RAD * rotation.x, DEG2RAD * rotation.y, DEG2RAD * rotation.z), scale);
	transformChanged = true;
}

void GameObject::SetRotation(float x, float y, float z)
{
	rotation = { x, y, z };
	transform = math::float4x4::FromTRS(position, Quat::FromEulerXYZ(DEG2RAD * rotation.x, DEG2RAD * rotation.y, DEG2RAD * rotation.z), scale);
	transformChanged = true;
}

void GameObject::SetScale(float x, float y, float z)
{
	scale = { x, y, z };
	transform = math::float4x4::FromTRS(position, Quat::FromEulerXYZ(DEG2RAD * rotation.x, DEG2RAD * rotation.y, DEG2RAD * rotation.z), scale);
	transformChanged = true;
}

void GameObject::Translate(float x, float y, float z)
{
	SetPosition(position.x + x, position.y + y, position.z + z);
}

void GameObject::Rotate(float x, float y, float z)
{
	SetRotation(rotation.x + x, rotation.y + y, rotation.z + z);
}