#pragma once

#include <Windows.h>

namespace Sapphire
{
	class OutputFile
	{
	public:
		OutputFile();
		~OutputFile();
		void open(LPCSTR fileName);
		void close();
		void write(LPCVOID data, DWORD size);
	private:
		HANDLE fileHandle;
	};
}
