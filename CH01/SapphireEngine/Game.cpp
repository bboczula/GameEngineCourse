#include "Game.h"

#include <iostream>

#ifndef LOG
#define LOG(s) std::cout << s << std::endl
#endif // !LOG

Sapphire::Game::Game(int width, int height) : window(width, height)
{
	LOG("BaseWindow::BaseWindow()");
}

Sapphire::Game::~Game()
{
	LOG("BaseWindow::~BaseWindow()");
}

void Sapphire::Game::Run()
{
	LOG("BaseWindow::start()");

	Initialize();

	// For each thread that creates the window, the OS creates a queue for window messages.
	// This queue holds messages  for all windows that are created on that thread.
	MSG msg{ 0 };
	while (1)
	{
		//// If HWND parameter is NULL, then function retrieves messages for any window that belongs to current thread,
		//// so both window messages and thread messages.
		bool isMsgOnQueue = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
		if (isMsgOnQueue)
		{
			// WM_QUIT meanst stop the message loop and exit the application
			if (msg.message == WM_QUIT)
			{
				break;
			}

			// This function translates keystrokes into characters.
			TranslateMessage(&msg);

			// This function tells OS to call Window Procedure
			DispatchMessage(&msg);
		}
		
		Tick();
	}

	LOG("BaseWindow::start() - finished");
}

void Sapphire::Game::Initialize()
{
}

void Sapphire::Game::Tick()
{
}
