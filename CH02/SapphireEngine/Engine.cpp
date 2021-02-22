#include "Engine.h"

Sapphire::Engine::Engine(UINT width, UINT height) : WindowApplication(width, height)
{
	Logger::getInstance().logMessage("%s", "Sapphire::Engine::Engine()\n");
}

Sapphire::Engine::~Engine()
{
	Logger::getInstance().logMessage("%s", "Sapphire::Engine::~Engine()");
}

void Sapphire::Engine::Initialize()
{
	Logger::getInstance().logMessage("%s", "Sapphire::Engine::Initialize()");
}

void Sapphire::Engine::Tick()
{
	// We won't log Tick, because it would immediately flood the logs.
}
