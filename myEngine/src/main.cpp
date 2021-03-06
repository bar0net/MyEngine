#include <iostream>

#include "LogSystem.h"
#include "Application.h"


#define _DEV_MODE_ 0

#if (_DEV_MODE_==1)
	#define _CRTDBG_MAP_ALLOC  
	#include <crtdbg.h> 
#endif

enum class AppStatus 
{ 
	None = 0, 
	Start, 
	Update, 
	End,
	Exit
};

MyEngine::LogSystem* logger = nullptr;
Application* App = nullptr;

int main(int argc, const char* argv[])
{
	logger = new MyEngine::LogSystem(MyEngine::LogLevel::None);

	LOGINFO("Begin Application");
	App = new Application();
	AppStatus status = AppStatus::Start;

	while (status != AppStatus::Exit) {
		switch (status)
		{
		case AppStatus::Start:
			LOGINFO("Application Start");
			if (App->Start())
			{
				status = AppStatus::Update;
				LOGINFO("Application Main Loop");
			}
			else status = AppStatus::End;
			break;

		case AppStatus::Update:
			if (!App->Update()) status = AppStatus::End;
			break;

		case AppStatus::End:
			LOGINFO("Application End");
			App->End();
			status = AppStatus::Exit;
			break;
		
		default:
			status = AppStatus::Exit;
			break;
		}
	}

	LOGINFO("Application Clear.");
	delete(logger);
	delete(App);

	#if (_DEV_MODE_==1)
		_CrtDumpMemoryLeaks();
		std::cin.get();
	#endif
	return 0;
}