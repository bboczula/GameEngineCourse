#include "WindowApplication.h"

#define WINDOW_CLASS_NAME L"SapphireMyWindowWin32Class"
#define WINDOW_TITLE L"SapphireEngine"
#define DEFAULT_WINDOW_STYLE WS_VISIBLE | WS_CLIPCHILDREN | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_BORDER

Sapphire::WindowApplication::WindowApplication(UINT width, UINT height)
	: width(width), height(height), hwnd(nullptr), instance(GetModuleHandle(nullptr))
{
	Logger::GetInstance().Log("Sapphire::WindowApplication::WindowApplication(%d x %d)\n", width, height);
	RegisterWindowClass();
	CreateWindowInstance();
}

Sapphire::WindowApplication::~WindowApplication()
{
	Logger::GetInstance().Log("Sapphire::WindowApplication::~WindowApplication()\n");
}

void Sapphire::WindowApplication::Run()
{
	Logger::GetInstance().Log("Sapphire::WindowApplication::Run() - start\n");

	Initialize();

	MSG msg{ 0 };
	while (1)
	{
		//bool isMessageAvailable = PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE);
		// This "while" helps process a lot of messages at once and reduce lag
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				Logger::GetInstance().Log("Sapphire::WindowApplication::Run() - WM_QUIT received\n");
				exit(0);
			}
	
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		Tick();
		PostTick();
	}

	Logger::GetInstance().Log("Sapphire::WindowApplication::Run() - finished\n");
}

void Sapphire::WindowApplication::PostTick()
{
	std::list<MessageQueueObserver*>::iterator iterator = observers.begin();
	while (iterator != observers.end())
	{
		(*iterator)->PostFrame();
		++iterator;
	}
}

void Sapphire::WindowApplication::Attach(MessageQueueObserver* observer)
{
	observers.push_back(observer);
}

void Sapphire::WindowApplication::Initialize()
{
	Logger::GetInstance().Log("Sapphire::WindowApplication::Initialize()\n");

	// Initialize Components
	std::list<MessageQueueObserver*>::iterator iterator = observers.begin();
	while (iterator != observers.end())
	{
		(*iterator)->Initialize();
		++iterator;
	}
}

void Sapphire::WindowApplication::Tick()
{
}

void Sapphire::WindowApplication::RegisterWindowClass()
{
	Logger::GetInstance().Log("Sapphire::WindowApplication::RegisterWindowClass()\n");

	WNDCLASSEX windowClass;
	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.lpszClassName = WINDOW_CLASS_NAME;
	windowClass.lpfnWndProc = WindowApplication::ThisWindowProcedure;
	windowClass.hInstance = instance;

	ExitIfFailed(RegisterClassEx(&windowClass));
}

void Sapphire::WindowApplication::CreateWindowInstance()
{
	Logger::GetInstance().Log("Sapphire::WindowApplication::CreateWindowInstance()\n");

	std::pair<LONG, LONG> windowDimentions;

	RECT windowRectangle = { 0, 0, width, height };
	AdjustWindowRect(&windowRectangle, DEFAULT_WINDOW_STYLE, FALSE);
	windowDimentions.first = windowRectangle.right - windowRectangle.left;
	windowDimentions.second = windowRectangle.bottom - windowRectangle.top;

	hwnd = CreateWindowEx(NULL, WINDOW_CLASS_NAME, WINDOW_TITLE, DEFAULT_WINDOW_STYLE,
		CW_USEDEFAULT, CW_USEDEFAULT, windowDimentions.first, windowDimentions.second, nullptr, nullptr, instance, this);

	if (!hwnd)
	{
		Logger::GetInstance().Log("%s (%d)", "ERROR: Window creation failed", GetLastError());
		exit(1);
	}
}

LRESULT Sapphire::WindowApplication::ThisWindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	WindowApplication* self = nullptr;
	if (msg == WM_NCCREATE)
	{
		LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		self = static_cast<WindowApplication*>(lpcs->lpCreateParams);

		SetLastError(0);
		if (SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self)) == 0)
		{
			if (GetLastError() != 0)
			{
				return false;
			}
		}
	}
	else
	{
		self = reinterpret_cast<WindowApplication*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}

	if (self)
	{
		return self->WindowProcedure(hwnd, msg, wParam, lParam);
	}
	else
	{
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

LRESULT Sapphire::WindowApplication::WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Here you notify the observers
	std::list<MessageQueueObserver*>::iterator iterator = observers.begin();
	while (iterator != observers.end())
	{
		(*iterator)->Handle(hwnd, msg, wParam, lParam);
		++iterator;
	}

	switch (msg)
	{
	case WM_DESTROY:
	{
		Sapphire::Logger::GetInstance().Log("WindowProcedure - WM_DESTROY received\n");

		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}
