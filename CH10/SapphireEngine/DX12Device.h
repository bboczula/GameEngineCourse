#pragma once

#include "Utils.h"
#include "Logger.h"

#include <d3d12.h>
#include <dxgi1_5.h>

namespace Sapphire
{
	class DX12Device
	{
	public:
		DX12Device(IDXGIAdapter1* adapter);
		~DX12Device();
		ID3D12Device* GetDevice();
	private:
		void EnableDebugLayer();
		ID3D12Device* device;
	};
}