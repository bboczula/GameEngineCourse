#pragma once

#include <d3d12.h>

namespace Sapphire
{
	class DX12RenderTarget
	{
		friend class DX12CommandList;
	public:
		DX12RenderTarget(ID3D12Device* device, ID3D12Resource* resource, D3D12_CPU_DESCRIPTOR_HANDLE descriptor);
		~DX12RenderTarget();
	private:
		D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle;
	};
}