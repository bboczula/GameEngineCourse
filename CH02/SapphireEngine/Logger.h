#pragma once

// Resource: https://stackoverflow.com/questions/1008019/c-singleton-design-pattern

#include <ctime>
#include <iomanip>
#include <Windows.h>

#include "OutputFile.h"

namespace Sapphire
{
	enum class LogLevel
	{
		Fatal,
		Error,
		Warning,
		Info
	};

	class Logger
	{
	public:
		void log(LogLevel logLevel, LPCSTR format, ...);
		static Logger& getInstance();
		Logger(Logger& other) = delete;
		Logger& operator=(const Logger&) = delete;
	private:
		Logger();
		~Logger();
		LogLevel logLevelTreshold;
		CHAR* stringBuffer;
		OutputFile logFile;
	};
}