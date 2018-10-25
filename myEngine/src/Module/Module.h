#ifndef _MYENGINE_MODULE_H
#define _MYENGINE_MODULE_H

#include "LogSystem.h"

enum class UpdateState 
{ 
	Update_Continue = 1, 
	Update_Error, 
	Update_End 
};

class Module
{
public:
	Module() {}
	virtual ~Module() {}

	virtual bool Init()					{ return true; }
	virtual bool Start()				{ return true; }
	virtual UpdateState PreUpdate()		{ return UpdateState::Update_Continue; }
	virtual UpdateState Update()		{ return UpdateState::Update_Continue; }
	virtual UpdateState PostUpdate()	{ return UpdateState::Update_End; }
	virtual bool End()					{ return true; }
	virtual bool CleanUp()				{ return true; }
};

extern MyEngine::LogSystem* logger;

#endif // !_MYENGINE_MODULE_H

