#pragma once

#include <Windows.h>

#include "Logger.h"

namespace Sapphire
{
	class InputFile
	{
	public:
		InputFile();
		~InputFile();
		void Open(LPCSTR fileName);
		void Close();
		void Read();
		void* GetBuffer();
		DWORD GetSize();
	private:
		HANDLE fileHandle;
		DWORD fileSize;
		char* fileBuffer;
	};
}
