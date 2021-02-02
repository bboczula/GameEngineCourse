#include "Engine.h"

Sapphire::Engine::Engine(UINT width, UINT height) : WindowApplication(width, height)
{
	std::cout << " Sapphire::Engine::Engine()" << std::endl;
}

Sapphire::Engine::~Engine()
{
	std::cout << " Sapphire::Engine::~Engine()" << std::endl;
}

void Sapphire::Engine::Initialize()
{
	std::cout << "  Sapphire::Engine::Initialize()" << std::endl;
}

void Sapphire::Engine::Tick()
{
	// We won't log Tick, because it would immediately flood the logs.
}
