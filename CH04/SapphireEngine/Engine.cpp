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
}

void Sapphire::Engine::Initialize()
{
	Logger::GetInstance().Log("%s", "Sapphire::Engine::Initialize()\n");
}

void Sapphire::Engine::Tick()
{
	renderer->Render();
}
