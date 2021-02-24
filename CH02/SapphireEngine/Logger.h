#pragma once

// Resource: https://stackoverflow.com/questions/1008019/c-singleton-design-pattern

#include <ctime>
#include <iomanip>
#include <Windows.h>

#include "OutputFile.h"

namespace Sapphire
{
	class Logger
	{
	public:
		void log(LPCSTR format, ...);
		static Logger& getInstance();
		Logger(Logger& other) = delete;
		Logger& operator=(const Logger&) = delete;
	private:
		Logger();
		~Logger();
		CHAR* stringBuffer;
		OutputFile logFile;
	};
}