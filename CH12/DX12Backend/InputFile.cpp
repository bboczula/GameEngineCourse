#include "pch.h"

#include "InputFile.h"

InputFile::InputFile() : fileSize(0), fileBuffer(nullptr)
{
	OutputDebugStringA("InputFile::InputFile()\n");
}

InputFile::~InputFile()
{
	OutputDebugStringA("InputFile::~InputFile()\n");
}

void InputFile::Open(LPCSTR fileName)
{
	File::Open(fileName, GENERIC_READ, OPEN_EXISTING);
}

void InputFile::Read()
{
	fileSize = GetFileSize(fileHandle, nullptr);
	fileBuffer = new char[fileSize];
	if (!ReadFile(fileHandle, fileBuffer, fileSize, nullptr, nullptr))
	{
		exit(3);
	}
}

void* InputFile::GetBuffer()
{
	return fileBuffer;
}

DWORD InputFile::GetSize()
{
	return fileSize;
}
