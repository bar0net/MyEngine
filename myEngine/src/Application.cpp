#include "Application.h"

#include "Module/ModuleRenderer.h"
#include "Module/ModuleInput.h"
#include "Module/ModuleEditor.h"

Application::Application()
{
	modules.push_back(renderer = new ModuleRenderer());
	modules.push_back(input = new ModuleInput());
	modules.push_back(editor = new ModuleEditor());
}


Application::~Application()
{
	for (std::list<Module *>::reverse_iterator it = modules.rbegin(); it != modules.rend(); ++it)
		delete(*it);

	modules.clear();
}

bool Application::Start()
{
	bool ret = true;

	for (std::list<Module *>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init();

	for (std::list<Module *>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Start();

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

	return ret;

}