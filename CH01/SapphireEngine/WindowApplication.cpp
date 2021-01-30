#include "WindowApplication.h"

#define WINDOW_CLASS_NAME "SapphireMyWindowWin32Class"
#define WINDOW_TITLE "SapphireEngine"
#define DEFAULT_WINDOW_STYLE WS_VISIBLE | WS_CLIPCHILDREN | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU
#define DEFAULT_WINDOW_STYLE_EX WS_EX_APPWINDOW | WS_EX_CLIENTEDGE

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
	{
		// This function puts WM_QUIT message on top of the queue - a speciall message that causes GetMessage to return 0
		PostQuitMessage(0);
		break;
	}
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

Sapphire::WindowApplication::WindowApplication(UINT width, UINT height)
	: width(width), height(height), hwnd(NULL), instance(NULL)
{
	std::cout << "Sapphire::WindowApplication::WindowApplication()" << std::endl;
	RegisterWindowClass();
	CreateWindowInstance();
}

Sapphire::WindowApplication::~WindowApplication()
{
	std::cout << "Sapphire::WindowApplication::~WindowApplication()" << std::endl;
}

void Sapphire::WindowApplication::Run()
{
	std::cout << " Sapphire::WindowApplication::Run() - start" << std::endl;

	Initialize();

	// For each thread that creates the window, the OS creates a queue for window messages.
	// This queue holds messages  for all windows that are created on that thread.
	MSG msg{ 0 };
	while (1)
	{
		//// If HWND parameter is NULL, then function retrieves messages for any window that belongs to current thread,
		//// so both window messages and thread messages.
		bool isMessageAvailable = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
		if (isMessageAvailable)
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
		else
		{
			Tick();
		}
	}

	std::cout << " Sapphire::WindowApplication::Run() - finished" << std::endl;
}

void Sapphire::WindowApplication::Initialize()
{
}

void Sapphire::WindowApplication::Tick()
{
}

void Sapphire::WindowApplication::RegisterWindowClass()
{
	std::cout << " Sapphire::WindowApplication::RegisterWindowClass()" << std::endl;

	WNDCLASSEX windowClass;
	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.lpszClassName = WINDOW_CLASS_NAME;
	windowClass.lpfnWndProc = (WNDPROC)WindowProcedure;
	windowClass.hInstance = GetModuleHandle(NULL);

	ThrowIfFailed(RegisterClassEx(&windowClass));
}

void Sapphire::WindowApplication::CreateWindowInstance()
{
	std::cout << " Sapphire::WindowApplication::CreateWindowInstance()" << std::endl;

	RECT windowRectangle = { 0, 0, width, height };
	AdjustWindowRect(&windowRectangle, WS_OVERLAPPEDWINDOW, FALSE);

	hwnd = CreateWindowEx(DEFAULT_WINDOW_STYLE_EX, WINDOW_CLASS_NAME, WINDOW_TITLE, DEFAULT_WINDOW_STYLE,
		CW_USEDEFAULT, CW_USEDEFAULT, windowRectangle.right, windowRectangle.bottom, NULL, NULL, instance, NULL);

	if (!hwnd)
	{
		std::cout << "ERROR: Window creation failed (" << GetLastError() << ")" << std::endl;
		exit(1);
	}
}
