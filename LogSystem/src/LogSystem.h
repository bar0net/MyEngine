#ifndef _MYENGINE_LOGSYSTEM_
#define _MYENGINE_LOGSYSTEM_

#include <queue>

#define LOG(x)				logger->Log(x)
#define LOGINFO(x, ...)		logger->Log(MyEngine::LogLevel::Info, __FILE__, __LINE__, x, __VA_ARGS__)
#define LOGDEBUG(x, ...)	logger->Log(MyEngine::LogLevel::Debug, __FILE__, __LINE__, x, __VA_ARGS__)
#define LOGWARNING(x, ...)	logger->Log(MyEngine::LogLevel::Warning, __FILE__, __LINE__, x, __VA_ARGS__)
#define LOGERROR(x, ...)	logger->Log(MyEngine::LogLevel::Error, __FILE__, __LINE__, x, __VA_ARGS__)


namespace MyEngine 
{
	enum class LogLevel 
	{
		None = 0,
		Debug = 1,
		Info = 2,
		Warning = 3,
		Error = 4,
	};

	struct LogData 
	{
		LogLevel type;
		std::string message;
	};

	class LogSystem
	{
	public:
		LogSystem() {}
		LogSystem(LogLevel level) : level(level) {}
		virtual ~LogSystem();

		void Log(LogLevel level, const char* file, int line, const char* message...);
		void Log(const char* message);
		void Log(std::string message);
		void Print();

	private:
		LogLevel level = LogLevel::Debug;
		std::queue<LogData> history;

	};

}

extern MyEngine::LogSystem* logger;

#endif //!_MYENGINE_LOGSYSTEM_