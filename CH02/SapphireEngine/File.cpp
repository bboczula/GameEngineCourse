#include "File.h"

File::File() : fileHandle(NULL)
{
}

File::~File()
{
}

void File::open(LPCSTR fileName)
{
	// Open another file
	fileHandle = CreateFileA(fileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	// Handle errors
	if (fileHandle == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, L"Unable to open file!", L"File Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
}

void File::close()
{
	CloseHandle(fileHandle);
}

void File::write(LPCVOID data, DWORD size)
{
	// Log to windows log file
	DWORD dwBytesWritten = 0;
	BOOL bErrorFlag = WriteFile(fileHandle, data, size, &dwBytesWritten, NULL);

	if (FALSE == bErrorFlag)
	{
		MessageBox(NULL, L"Unable to write to file!", L"File Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	else
	{
		if (dwBytesWritten != size)
		{
			// This is an error because a synchronous write that results in
			// success (WriteFile returns TRUE) should write all data as
			// requested. This would not necessarily be the case for
			// asynchronous writes.
			MessageBox(NULL, L"Unable to write entire string!", L"File Error", MB_OK | MB_ICONERROR);
			exit(1);
		}
	}
}
