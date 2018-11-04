#ifndef _MYENGINE_MODULE_SCENE_H
#define _MYENGINE_MODULE_SCENE_H

#include "Module.h"
#include <unordered_map>

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

	bool Init();
	bool Start();
	UpdateState Update();
	bool CleanUp();

	void NewModel(const char* file);
	
	MyEngine::Shader* shader = nullptr;

	std::unordered_map<std::string, GameObject*> gameObjects;
};


#endif //!_MYENGINE_MODULE_SCENE_H
