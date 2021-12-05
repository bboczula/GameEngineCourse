#include "InputFile.h"

Sapphire::InputFile::InputFile()
{
	OutputDebugStringA("InputFile::InputFile()\n");
}

Sapphire::InputFile::~InputFile()
{
	OutputDebugStringA("InputFile::~InputFile()\n");
}

void Sapphire::InputFile::Open(LPCSTR fileName)
{
	// Open another file
	fileHandle = CreateFileA(fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	// Handle errors
	if (fileHandle == INVALID_HANDLE_VALUE)
	{
		MessageBoxA(NULL, "Unable to open file!", "File Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
}

void Sapphire::InputFile::Close()
{
	CloseHandle(fileHandle);
}

void Sapphire::InputFile::Read()
{
	fileSize = GetFileSize(fileHandle, NULL);
	fileBuffer = new char[fileSize];
	if (!ReadFile(fileHandle, fileBuffer, fileSize, NULL, NULL))
	{
		Logger::GetInstance().Log("%s (%d)", "ERROR: Reading from file failed", GetLastError());
		exit(3);
	}
}

void* Sapphire::InputFile::GetBuffer()
{
	return fileBuffer;
}

DWORD Sapphire::InputFile::GetSize()
{
	return fileSize;
}
