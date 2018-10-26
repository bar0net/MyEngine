#include <iostream>

#include "LogSystem.h"
#include "Application.h"

#define _DEV_MODE_ 1

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

int main() 
{
	logger = new MyEngine::LogSystem(MyEngine::LogLevel::Debug);

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
				
		}
	}

	LOGINFO("Application Clear.");

	#if (_DEV_MODE_==1)
		LOGINFO("Press Enter to close.");
		std::cin.get();
	#endif

	delete(logger);
	delete(App);
	return 0;
}