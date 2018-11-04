#include "LogSystem.h"

#include <string>
#include <stdarg.h>

#define _USE_CONSOLE_ 1

#if (_USE_CONSOLE_ == 1)
	#include <iostream>
	#define PRINT(x) std::cout << x << std::endl;
#else
	#define PRINT(x)
#endif


MyEngine::LogSystem::~LogSystem()
{
	while (!history.empty()) history.pop_back();
}

void MyEngine::LogSystem::Log(LogLevel level, const char* file, int line, const char* message...)
{
	if (level < this->level) return;
	std::string s = "";

	switch (level)
	{
	case LogLevel::Debug:
		s = "[Debug] <";
		break;
	case LogLevel::Info:
		s = "[Info] <";
		break;
	case LogLevel::Warning:
		s = "[Warning] <";
		break;
	case LogLevel::Error:
		s = "[Error] <";
		break;

	default:
		s = "[None] <";
		break;
	}
	s.append(file); s.append(":"); s += std::to_string(line); s.append("> ");
	s.append(message);

	va_list args;
	va_start(args, message);

	unsigned int i = 0;
	while (true)
	{
		i = s.find('%');
		if (i == std::string::npos || s.size() < i + 1) break;

		switch (s[i+1])
		{
		case 's':
			s = s.substr(0, i) + va_arg(args, char*) + s.substr(i + 2);
			break;

		case 'd':
			s = s.substr(0, i) + std::to_string(va_arg(args, int)) + s.substr(i + 2);
			break;

		case 'i':
			s = s.substr(0, i) + std::to_string(va_arg(args, int)) + s.substr(i + 2);
			break;
		}
	}
	
	PRINT(s);

	LogData data = { level, s };
	history.push_back(data);
}

void MyEngine::LogSystem::Log(const char * message)
{
	std::string s(message);
	PRINT(s);
	history.push_back({ LogLevel::None, s });
}


void MyEngine::LogSystem::Log(std::string message)
{
	PRINT(message);
	history.push_back({ LogLevel::None, message });
}