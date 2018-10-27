#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

#include <list>

#include "Component.h"

class GameObject
{
public:
	GameObject() {};
	virtual ~GameObject();

	virtual void Start();
	virtual void Update();
	virtual void End();

	void AddComponent(Component* component);

	float position[3];
	float rotation[3];
	float scale[3];

	std::list<Component*> components;

};

#endif // !_GAME_OBJECT_H

