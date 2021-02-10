#include "WindowApplication.h"

#define WINDOW_CLASS_NAME L"SapphireMyWindowWin32Class"
#define WINDOW_TITLE L"SapphireEngine"
#define DEFAULT_WINDOW_STYLE WS_VISIBLE | WS_CLIPCHILDREN | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_BORDER

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
	{
		std::cout << "  WindowProcedure - WM_DESTROY received" << std::endl;

		PostQuitMessage(0);
		break;
	}
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

Sapphire::WindowApplication::WindowApplication(UINT width, UINT height)
	: width(width), height(height), hwnd(NULL), instance(GetModuleHandle(NULL))
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

	MSG msg{ 0 };
	while (1)
	{
		bool isMessageAvailable = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
		if (isMessageAvailable)
		{
			if (msg.message == WM_QUIT)
			{
				std::cout << "  Sapphire::WindowApplication::Run() - WM_QUIT received" << std::endl;
				break;
			}

			DispatchMessage(&msg);
		}
		
		Tick();
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
	windowClass.hInstance = instance;

	ExitIfFailed(RegisterClassEx(&windowClass));
}

void Sapphire::WindowApplication::CreateWindowInstance()
{
	std::cout << " Sapphire::WindowApplication::CreateWindowInstance()" << std::endl;

	RECT windowRectangle = { 0, 0, width, height };
	AdjustWindowRect(&windowRectangle, WS_OVERLAPPEDWINDOW, FALSE);

	hwnd = CreateWindowEx(NULL, WINDOW_CLASS_NAME, WINDOW_TITLE, DEFAULT_WINDOW_STYLE,
		CW_USEDEFAULT, CW_USEDEFAULT, windowRectangle.right, windowRectangle.bottom, NULL, NULL, instance, NULL);

	if (!hwnd)
	{
		std::cout << "ERROR: Window creation failed (" << GetLastError() << ")" << std::endl;
		exit(1);
	}
}
