#pragma once

// Resource: https://stackoverflow.com/questions/1008019/c-singleton-design-pattern

#include <ctime>
#include <iomanip>
#include <Windows.h>

#include "OutputFile.h"

#define STRING_BUFFER_SIZE 1024

namespace Sapphire
{
	class Logger
	{
	public:
		void Log(LPCSTR format, ...);
		static Logger& GetInstance();
		Logger(Logger& other) = delete;
		Logger& operator=(const Logger&) = delete;
	private:
		Logger();
		~Logger();
		OutputFile logFile;
	};
}