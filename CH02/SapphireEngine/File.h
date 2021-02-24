#pragma once

#include <Windows.h>

class File
{
public:
	File();
	~File();
	void open(LPCSTR fileName);
	void close();
	void write(LPCVOID data, DWORD size);
private:
	HANDLE fileHandle;
};

