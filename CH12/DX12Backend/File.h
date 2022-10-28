#pragma once

#include <Windows.h>
#include <stdlib.h>

namespace GpuApi
{
	class File
	{
	public:
		virtual void Open(LPCSTR fileName) = 0;
		void Close();
	protected:
		void Open(LPCSTR fileName, DWORD accessType, DWORD creationType);
		HANDLE fileHandle;
	};
}