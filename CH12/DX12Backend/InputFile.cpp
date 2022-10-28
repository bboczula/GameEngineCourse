#include "pch.h"

#include "InputFile.h"

GpuApi::InputFile::InputFile() : fileSize(0), fileBuffer(nullptr)
{
	OutputDebugStringA("InputFile::InputFile()\n");
}

GpuApi::InputFile::~InputFile()
{
	OutputDebugStringA("InputFile::~InputFile()\n");
}

void GpuApi::InputFile::Open(LPCSTR fileName)
{
	File::Open(fileName, GENERIC_READ, OPEN_EXISTING);
}

void GpuApi::InputFile::Read()
{
	fileSize = GetFileSize(fileHandle, nullptr);
	fileBuffer = new char[fileSize];
	if (!ReadFile(fileHandle, fileBuffer, fileSize, nullptr, nullptr))
	{
		exit(3);
	}
}

void* GpuApi::InputFile::GetBuffer()
{
	return fileBuffer;
}

DWORD GpuApi::InputFile::GetSize()
{
	return fileSize;
}
