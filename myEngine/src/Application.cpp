#include "Application.h"

#include "Module/ModuleRenderer.h"
#include "Module/ModuleInput.h"
#include "Module/ModuleEditor.h"
#include "Module/ModuleTime.h"
#include "Module/ModuleScene.h"
#include "Module/ModuleTexture.h"
#include "Module/ModuleModelLoader.h"

#include "Utils/Chrono.h"

Application::Application()
{
	MyEngine::uChrono uchrono;
	MyEngine::Chrono chrono;
	chrono.Start();
	uchrono.Start();
	modules.push_back(renderer = new ModuleRenderer());
	modules.push_back(texture = new ModuleTexture());
	modules.push_back(input = new ModuleInput());
	modules.push_back(time = new ModuleTime());
	modules.push_back(scene = new ModuleScene());
	modules.push_back(editor = new ModuleEditor());
	modules.push_back(models = new ModuleModelLoader());


	float a = uchrono.Stop();
	unsigned int b = chrono.Stop();
	LOGERROR("float test %f", 0.9876F);
	LOGDEBUG("App Start: %sms -- %sms", std::to_string(a).c_str() , std::to_string(b).c_str());
}


Application::~Application()
{

}

bool Application::Start()
{
	MyEngine::uChrono uchrono;
	MyEngine::Chrono chrono;
	chrono.Start();
	uchrono.Start();
	bool ret = true;

	for (std::list<Module *>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init();

	for (std::list<Module *>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Start();

	float a = uchrono.Stop();
	float b = chrono.Stop();
	LOGDEBUG("App Start: %fms -- %fms",a, b);
	return ret;
}

bool Application::Update()
{
	UpdateState ret = UpdateState::Update_Continue;

	for (std::list<Module *>::iterator it = modules.begin(); it != modules.end() && ret == UpdateState::Update_Continue; ++it)
		ret = (*it)->PreUpdate();

	for (std::list<Module *>::iterator it = modules.begin(); it != modules.end() && ret == UpdateState::Update_Continue; ++it)
		ret = (*it)->Update();

	for (std::list<Module *>::iterator it = modules.begin(); it != modules.end() && ret == UpdateState::Update_Continue; ++it)
		ret = (*it)->PostUpdate();

	return ret == UpdateState::Update_Continue;
}

bool Application::End()
{
	bool ret = true;

	for (std::list<Module *>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->End();

	for (std::list<Module *>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		ret = (*it)->CleanUp();

	for (std::list<Module *>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		delete (*it);

	modules.clear();

	return ret;
}