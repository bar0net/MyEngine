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
	
	MyEngine::Shader* shader;

	std::unordered_map<const char*, GameObject*> gameObjects;
};


#endif //!_MYENGINE_MODULE_SCENE_H
