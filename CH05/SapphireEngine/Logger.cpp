#include "Logger.h"

// Resource: http://laristra.github.io/flecsi/src/developer-guide/patterns/meyers_singleton.html

Sapphire::Logger& Sapphire::Logger::GetInstance()
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

Sapphire::Logger::Logger()
{
	logFile.Open("sapphire_log.txt");
	Log("%s", "----- LOG STARTED -----\n");
}

Sapphire::Logger::~Logger()
{
	Log("%s", "----- LOG ENDED -----\n");
	logFile.Close();
}

void Sapphire::Logger::Log(LPCSTR format, ...)
{
	CHAR messageBuffer[STRING_BUFFER_SIZE];
	va_list variableArgumentList;
	va_start(variableArgumentList, format);
	vsprintf_s(messageBuffer, STRING_BUFFER_SIZE, format, variableArgumentList);
	va_end(variableArgumentList);

	tm newTime;
	time_t currentTime = time(nullptr);
	errno_t errorNumber = localtime_s(&newTime, &currentTime);

	char lineBuffer[STRING_BUFFER_SIZE];
	int numOfCharactersWritten = sprintf_s(lineBuffer, STRING_BUFFER_SIZE, "%.2d:%.2d:%.2d - %s", newTime.tm_hour, newTime.tm_min, newTime.tm_sec, messageBuffer);

	OutputDebugStringA(lineBuffer);
	logFile.Write(lineBuffer, numOfCharactersWritten);
}