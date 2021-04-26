#pragma once

#include <Windows.h>
#include "Logger.h"
#include "Utils.h"

namespace Sapphire
{
	class WindowApplication
	{
	public:
		WindowApplication(UINT width, UINT height);
		~WindowApplication();
		void Run();
	protected:
		virtual void Initialize();
		virtual void Tick();
		void RegisterWindowClass();
		void CreateWindowInstance();
		HWND hwnd;
		HINSTANCE instance;
		LONG width;
		LONG height;
	};
}