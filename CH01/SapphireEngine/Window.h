#pragma once

#include <Windows.h>
#include <iostream>
#include "WindowContext.h"
#include "Utils.h"

namespace Sapphire
{
	class Window
	{
	public:
		Window(UINT width, UINT height);
		~Window();
		void RegisterWindowClass();
		void CreateWindowInstance();
		WindowContext GetWindowContext();
	private:
		WindowContext windowContext;
	};
}