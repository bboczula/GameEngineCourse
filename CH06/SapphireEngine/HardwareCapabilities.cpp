#include "HardwareCapabilities.h"

Sapphire::HardwareCapabilities::HardwareCapabilities()
{
}

Sapphire::HardwareCapabilities::~HardwareCapabilities()
{
}

void Sapphire::HardwareCapabilities::setCapability(Capabilities capability)
{
	capabilities[capability] = true;
}

bool Sapphire::HardwareCapabilities::getCapability(Capabilities capability)
{
	return capabilities[capability];
}
