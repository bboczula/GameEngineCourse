#pragma once

#include <Windows.h>
#include <iostream>
#include "Utils.h"

#define DEFAULT_WINDOW_WIDTH 1280
#define DEFAULT_WINDOW_HEIGHT 720

namespace Sapphire
{
	class Window
	{
	public:
		Window(UINT width = DEFAULT_WINDOW_WIDTH, UINT height = DEFAULT_WINDOW_HEIGHT);
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