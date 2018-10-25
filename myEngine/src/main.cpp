#include <iostream>

#include "LogSystem.h"
#include "Application.h"

enum class AppStatus 
{ 
	None = 0, 
	Start, 
	Update, 
	End,
	Exit
};

MyEngine::LogSystem* logger = nullptr;

int main() 
{
	logger = new MyEngine::LogSystem(MyEngine::LogLevel::Debug);

	LOGINFO("Begin Application");
	Application* App = new Application();
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


	LOGINFO("Application Clear");

	delete(logger);
	delete(App);
	std::cin.get();

	return 0;
}