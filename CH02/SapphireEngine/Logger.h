#pragma once

// Resource: https://stackoverflow.com/questions/1008019/c-singleton-design-pattern

#include <fstream>
#include <ctime>
#include <iomanip>
#include <Windows.h>

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
	std::ofstream logFile;
};