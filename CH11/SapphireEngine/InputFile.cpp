#include "InputFile.h"

Sapphire::InputFile::InputFile() : fileSize(0), fileBuffer(nullptr)
{
	OutputDebugStringA("InputFile::InputFile()\n");
}

Sapphire::InputFile::~InputFile()
{
	OutputDebugStringA("InputFile::~InputFile()\n");
}

void Sapphire::InputFile::Open(LPCSTR fileName)
{
	File::Open(fileName, GENERIC_READ, OPEN_EXISTING);
}

void Sapphire::InputFile::Read()
{
	fileSize = GetFileSize(fileHandle, nullptr);
	fileBuffer = new char[fileSize];
	if (!ReadFile(fileHandle, fileBuffer, fileSize, nullptr, nullptr))
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
