#pragma once

namespace Sapphire
{
	const unsigned int MAX_CAPABILITES = 1;

	enum Capabilities
	{
		ALLOW_TEARING = 0
	};

	class HardwareCapabilities
	{
	public:
		HardwareCapabilities();
		~HardwareCapabilities();
		void setCapability(Capabilities capability);
		bool getCapability(Capabilities capability);
	private:
		bool capabilities[MAX_CAPABILITES];
	};
}