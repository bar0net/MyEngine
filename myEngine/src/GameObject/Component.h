#ifndef _GAMEOBJECT_COMPONENT_H
#define _GAMEOBJECT_COMPONENT_H

class GameObject;

class Component
{
public:
	Component(const char* name) : name(name) {};
	virtual ~Component() {};

	virtual void Init() {};
	virtual void Start() {};
	virtual void Update() {};
	virtual void End() {};
	virtual void CleanUp() {};

	virtual void SetGameObject(GameObject* gameObject);

	const char* GetName() { return name; }

protected:
	GameObject* gameObject;
	const char* name;
};

#endif //!_GAMEOBJECT_COMPONENT_H
