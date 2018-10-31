#include "LogSystem.h"

#include <string>
#include <iostream>
#include <stdarg.h>

MyEngine::LogSystem::~LogSystem()
{
	while (!history.empty()) history.pop();
}

void  MyEngine::LogSystem::Print()
{
	while (!history.empty())
	{
		std::cout << history.front().message << std::endl;
		history.pop();
	}
}

void MyEngine::LogSystem::Log(LogLevel level, const char* file, int line, const char* message...)
{
	if (level < this->level) return;
	std::string s;

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
	
	std::cout << s << std::endl;

	LogData data = { level, s };
	history.push(data);
}

void MyEngine::LogSystem::Log(const char * message)
{
	std::string s(message);

	std::cout << message << std::endl;
	history.push({ LogLevel::None, s });
}


void MyEngine::LogSystem::Log(std::string message)
{
	std::cout << message << std::endl;
	history.push({ LogLevel::None, message });
}