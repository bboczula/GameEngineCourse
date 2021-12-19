#pragma once

#include <Windows.h>

#include "File.h"

namespace Sapphire
{
	class OutputFile : public File
	{
	public:
		OutputFile();
		~OutputFile();
		virtual void Open(LPCSTR fileName) override;
		void Write(LPCVOID data, DWORD size);
	};
}
