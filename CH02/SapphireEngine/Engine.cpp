#include "Engine.h"

Sapphire::Engine::Engine(UINT width, UINT height) : WindowApplication(width, height)
{
	Logger::getInstance().log("%s", "Sapphire::Engine::Engine()\n");
}

Sapphire::Engine::~Engine()
{
	Logger::getInstance().log("%s", "Sapphire::Engine::~Engine()");
}

void Sapphire::Engine::Initialize()
{
	Logger::getInstance().log("%s", "Sapphire::Engine::Initialize()");
}

void Sapphire::Engine::Tick()
{
	// We won't log Tick, because it would immediately flood the logs.
}
