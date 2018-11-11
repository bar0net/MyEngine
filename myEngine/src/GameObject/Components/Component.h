#ifndef _GAMEOBJECT_COMPONENT_H
#define _GAMEOBJECT_COMPONENT_H

class GameObject;

enum class ComponentType
{
	NONE = 0,
	CAMERA,
	CAMERA_CONTROL,
	MESH_RENDERER
};

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

	virtual void SetGameObject(GameObject* const gameObject);
	virtual GameObject* GetGameObject();

	const char* const GetName() const { return name; }
	const ComponentType GetType() const { return componentType; }

protected:
	GameObject* gameObject = nullptr;
	const char* name = nullptr;
	ComponentType componentType = ComponentType::NONE;
};

#endif //!_GAMEOBJECT_COMPONENT_H
