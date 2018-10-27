#include "Component.h"

#include "GameObject.h"


void Component::SetGameObject(GameObject* gameObject)
{
	this->gameObject = gameObject;
}
