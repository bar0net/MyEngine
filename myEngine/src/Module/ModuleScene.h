#ifndef _MYENGINE_MODULE_SCENE_H
#define _MYENGINE_MODULE_SCENE_H

#include "Module.h"
#include <unordered_map>
#include <stack>

class GameObject;

namespace MyEngine 
{
	class Shader;
}

class ModuleScene : public Module
{
public:
	ModuleScene() {};
	virtual ~ModuleScene() {};

	bool Init() override;
	bool Start() override;
	UpdateState Update() override;
	UpdateState PostUpdate() override;
	bool CleanUp() override;

	void NewModel(const char* file);

	GameObject* Find(const char* name);
	void DeleteGameObject(const char* name);
	void DeleteGameObject(GameObject* gameObject);

public:
	MyEngine::Shader* shader = nullptr;
	std::unordered_map<std::string, GameObject*> gameObjects;

private:
	std::stack<GameObject*> removeStack;
};


#endif //!_MYENGINE_MODULE_SCENE_H
