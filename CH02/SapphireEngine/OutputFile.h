#pragma once

#include <Windows.h>

namespace Sapphire
{
	class OutputFile
	{
	public:
		OutputFile();
		~OutputFile();
		void Open(LPCSTR fileName);
		void Close();
		void Write(LPCVOID data, DWORD size);
	private:
		HANDLE fileHandle;
	};
}
