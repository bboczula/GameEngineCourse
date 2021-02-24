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
}

Logger::Logger()
{
	logFile.open("sapphire_log.txt");
}

Logger::~Logger()
{
	logFile.close();
}

void Logger::log(LPCSTR format, ...)
{
	// First, we need to bypass the formatting string and params to sprintf.
	// sprintf function composes a string with tthe same text as printf, but instead of printing
	// to standard output, it is stored as a cstring in the buffer pointer.
	char buffer[1024];
	va_list va;
	va_start(va, format);
	int numOfWrittenCharacters = vsprintf_s(buffer, format, va);
	va_end(va);

	//size_t* returnValue = nullptr;
	//wchar_t wtext[1024];
	//size_t count = 1024;
	//mbstowcs_s(returnValue, wtext, strlen(buffer) + 1, buffer, count);

	OutputDebugStringA(buffer);

	struct tm newTime;
	time_t currentTime = time(nullptr);
	errno_t errorNumber = localtime_s(&newTime, &currentTime);
	logFile << std::setw(2) << std::setfill('0') << newTime.tm_hour << ":" << std::setw(2) << std::setfill('0') << newTime.tm_min
		<< ":" << std::setw(2) << std::setfill('0') << newTime.tm_sec << ": ";

	logFile << buffer;
}