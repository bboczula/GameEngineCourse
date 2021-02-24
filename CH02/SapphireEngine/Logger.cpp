#include "Logger.h"

// Resource: http://laristra.github.io/flecsi/src/developer-guide/patterns/meyers_singleton.html

Logger& Logger::getInstance()
{
	// This is honestly one of the wierdest thing i've seen in C++
	// "Normal" singleton was not possible to call destructor
	// And this calls destructor as if there was not a problem at all

	// When you declare variable in a function, the 'static' keyword specifies that
	// the variable retains its state between calls to that function

	// The 'static' variables are stored in the data segment of the memory. The data segment
	// is a part of the virtual address space of a program.

	// All the static variables that do not have an explicit initialization or are initialized to zero
	// are stored in the uninitialized data segment (BSS)

	// Static local variable retain their values between function call and are initialized once.

	static Logger logger;
	return logger;

	// Another thing - it is important to make sure that this gets initialized first
}

Logger::Logger() : logLevelTreshold(LogLevel::Info)
{
	stringBuffer = new CHAR[1024];
	logFile.open("sapphire_log.txt");
	log(LogLevel::Info, "%s", "----- LOG STARTED -----\n");
}

Logger::~Logger()
{
	log(LogLevel::Info, "%s", "----- LOG ENDED -----\n");
	delete[] stringBuffer;
	logFile.close();
}

void Logger::log(LogLevel logLevel, LPCSTR format, ...)
{
	if (logLevel > logLevelTreshold)
	{
		return;
	}

	va_list variableArgumentList;
	va_start(variableArgumentList, format);
	int numOfWrittenCharacters = vsprintf_s(stringBuffer, 1024, format, variableArgumentList);
	va_end(variableArgumentList);

	struct tm newTime;
	time_t currentTime = time(nullptr);
	errno_t errorNumber = localtime_s(&newTime, &currentTime);

	char dataToWrite[1024];
	int numOfDataToWrite = sprintf_s(dataToWrite, 1024, "%.2d:%.2d:%.2d - %s", newTime.tm_hour, newTime.tm_min, newTime.tm_sec, stringBuffer);

	OutputDebugStringA(dataToWrite);

	logFile.write(dataToWrite, numOfDataToWrite);
}