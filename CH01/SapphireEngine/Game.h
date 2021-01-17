#pragma once

#include <string>
#include <Windows.h>
#include <windowsx.h>
#include <stdint.h>
#include <string>

#include "Window.h"

namespace Sapphire
{
	class Game
	{
	public:
		Game(int width, int height);
		virtual ~Game();
		void Run();
	protected:
		Window window;
		virtual void Initialize();
		virtual void Tick();
	};
}