#include "OutputFile.h"

Sapphire::OutputFile::OutputFile() : fileHandle(NULL)
{
	OutputDebugStringA("OutputFile::OutputFile()\n");
}

Sapphire::OutputFile::~OutputFile()
{
	OutputDebugStringA("OutputFile::~OutputFile()\n");
}

void Sapphire::OutputFile::Open(LPCSTR fileName)
{
	// Open another file
	fileHandle = CreateFileA(fileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	// Handle errors
	if (fileHandle == INVALID_HANDLE_VALUE)
	{
		MessageBoxA(NULL, "Unable to open file!", "File Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
}

void Sapphire::OutputFile::Close()
{
	CloseHandle(fileHandle);
}

void Sapphire::OutputFile::Write(LPCVOID data, DWORD dataSizeInBytes)
{
	DWORD bytesWritten = 0;
	BOOL errorFlag = WriteFile(fileHandle, data, dataSizeInBytes, &bytesWritten, NULL);

	if (FALSE == errorFlag)
	{
		MessageBoxA(NULL, "Unable to write to file!", "File Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	else if (bytesWritten != dataSizeInBytes)
	{
		MessageBoxA(NULL, "Unable to write entire string!", "File Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
}
