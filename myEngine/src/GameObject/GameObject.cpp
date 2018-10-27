#include "GameObject.h"

#include "Component.h"


GameObject::~GameObject()
{
	for (std::list<Component*>::iterator it = components.begin(); it != components.end(); ++it)
		delete(*it);

	components.clear();
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

void GameObject::AddComponent(Component* component)
{
	component->SetGameObject(this);
	components.push_back(component);
}