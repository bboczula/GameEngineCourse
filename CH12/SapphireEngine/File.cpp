#include "File.h"

void Sapphire::File::Close()
{
	CloseHandle(fileHandle);
}

void Sapphire::File::Open(LPCSTR fileName, DWORD accessType, DWORD creationType)
{
	fileHandle = CreateFileA(fileName, accessType, 0, nullptr, creationType, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (fileHandle == INVALID_HANDLE_VALUE)
	{
		MessageBoxA(nullptr, "Unable to open file!", "File Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
}
