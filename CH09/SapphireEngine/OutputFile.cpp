#include "OutputFile.h"

Sapphire::OutputFile::OutputFile()
{
	OutputDebugStringA("OutputFile::OutputFile()\n");
}

Sapphire::OutputFile::~OutputFile()
{
	OutputDebugStringA("OutputFile::~OutputFile()\n");
}

void Sapphire::OutputFile::Open(LPCSTR fileName)
{
	File::Open(fileName, GENERIC_WRITE, CREATE_ALWAYS);
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
