#include "Engine.h"

Sapphire::Engine::Engine(UINT width, UINT height) : WindowApplication(width, height)
{
	Logger::GetInstance().Log("%s", "Sapphire::Engine::Engine()\n");
	renderer = new Renderer(hwnd, width, height);
}

Sapphire::Engine::~Engine()
{
	Logger::GetInstance().Log("%s", "Sapphire::Engine::~Engine()\n");
	if (renderer)
	{
		delete renderer;
	}

#if _DEBUG
	ReportLiveObjects();
#endif
}

void Sapphire::Engine::Initialize()
{
	Logger::GetInstance().Log("%s", "Sapphire::Engine::Initialize()\n");
}

void Sapphire::Engine::Tick()
{
	renderer->Render();
}

void Sapphire::Engine::ReportLiveObjects()
{
	IDXGIDebug* dxgiDebugInterface;
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebugInterface))))
	{
		dxgiDebugInterface->ReportLiveObjects(DXGI_DEBUG_ALL,
			DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_SUMMARY | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
	}
	SafeRelease(&dxgiDebugInterface);
}
