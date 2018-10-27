#ifndef _GAMEOBJECT_COMPONENT_H
#define _GAMEOBJECT_COMPONENT_H

class GameObject;

class Component
{
public:
	Component() {};
	virtual ~Component() {};

	virtual void Start() {};
	virtual void Update() {};
	virtual void End() {};

	void SetGameObject(GameObject* gameObject);

protected:
	GameObject* gameObject;
};

#endif //!_GAMEOBJECT_COMPONENT_H
