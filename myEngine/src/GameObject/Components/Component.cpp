#include "Component.h"

#include "../GameObject.h"


void Component::SetGameObject(GameObject* const gameObject)
{
	this->gameObject = gameObject;
}

GameObject * Component::GetGameObject()
{
	return gameObject;
}
