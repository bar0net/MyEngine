#ifndef _MYENGINE_MODULE_SCENE_H
#define _MYENGINE_MODULE_SCENE_H

#include "Module.h"

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
	UpdateState Update();
	bool CleanUp();
	
	GameObject* object;
	//GameObject* camera;
	MyEngine::Shader* shader;
};


#endif //!_MYENGINE_MODULE_SCENE_H
