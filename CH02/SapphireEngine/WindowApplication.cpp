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
		Logger::getInstance().logMessage("WindowProcedure - WM_DESTROY received\n");

		PostQuitMessage(0);
		break;
	}
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

Sapphire::WindowApplication::WindowApplication(UINT width, UINT height)
	: width(width), height(height), hwnd(NULL), instance(GetModuleHandle(NULL))
{
	Logger::getInstance().logMessage("Sapphire::WindowApplication::WindowApplication(%d x %d)\n", width, height);
	RegisterWindowClass();
	CreateWindowInstance();
}

Sapphire::WindowApplication::~WindowApplication()
{
	
}

void Sapphire::WindowApplication::Run()
{
	Logger::getInstance().logMessage("Sapphire::WindowApplication::Run() - start\n");

	Initialize();

	MSG msg{ 0 };
	while (1)
	{
		bool isMessageAvailable = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
		if (isMessageAvailable)
		{
			if (msg.message == WM_QUIT)
			{
				Logger::getInstance().logMessage("Sapphire::WindowApplication::Run() - WM_QUIT received\n");
				break;
			}

			DispatchMessage(&msg);
		}
		
		Tick();
	}

	Logger::getInstance().logMessage("Sapphire::WindowApplication::Run() - finished\n");
}

void Sapphire::WindowApplication::Initialize()
{
	Logger::getInstance().logMessage("Sapphire::WindowApplication::Initialize()\n");
}

void Sapphire::WindowApplication::Tick()
{
}

void Sapphire::WindowApplication::RegisterWindowClass()
{
	Logger::getInstance().logMessage("Sapphire::WindowApplication::RegisterWindowClass()\n");

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
	Logger::getInstance().logMessage("Sapphire::WindowApplication::CreateWindowInstance()\n");

	RECT windowRectangle = { 0, 0, width, height };
	AdjustWindowRect(&windowRectangle, WS_OVERLAPPEDWINDOW, FALSE);

	hwnd = CreateWindowEx(NULL, WINDOW_CLASS_NAME, WINDOW_TITLE, DEFAULT_WINDOW_STYLE,
		CW_USEDEFAULT, CW_USEDEFAULT, windowRectangle.right, windowRectangle.bottom, NULL, NULL, instance, NULL);

	if (!hwnd)
	{
		Logger::getInstance().logMessage("%s (%d)", "ERROR: Window creation failed", GetLastError());
		exit(1);
	}
}
