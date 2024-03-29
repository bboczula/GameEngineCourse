#pragma once

#include "Utils.h"
#include "Logger.h"
#include "DX12Device.h"
#include "d3dx12.h"

#include <d3d12.h>

namespace Sapphire
{
	class DX12Resource
	{
		friend class DX12CommandList;
	public:
		DX12Resource(void* source, D3D12_RESOURCE_STATES state);
		~DX12Resource();
		ID3D12Resource* GetResource();
	private:
		D3D12_RESOURCE_STATES state;
		ID3D12Resource* resource;
	};
}