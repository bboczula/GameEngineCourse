#pragma once

#include <d3d12.h>

#include "Logger.h"
#include "Utils.h"

namespace Sapphire
{
	class DX12DescriptorHeap
	{
	public:
		DX12DescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE);
		~DX12DescriptorHeap();
		SIZE_T AllocateDescriptor();
	private:
		ID3D12DescriptorHeap* heap;
		UINT descriptorSize;
		UINT baseIndex;
	};
}