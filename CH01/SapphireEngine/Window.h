#pragma once

#include <Windows.h>
#include <iostream>
#include "Utils.h"

namespace Sapphire
{
	class Window
	{
	public:
		Window(UINT width, UINT height);
		~Window();
		void run();
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