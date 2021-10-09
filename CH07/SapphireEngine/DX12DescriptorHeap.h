#pragma once

#include <d3d12.h>

#include "Logger.h"
#include "Utils.h"

namespace Sapphire
{
	class DX12DescriptorHeap
	{
	public:
		DX12DescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE, UINT size);
		~DX12DescriptorHeap();
		SIZE_T GetCpuDescriptorHandle(UINT index);
	private:
		ID3D12DescriptorHeap* rtvHeap;
		UINT rtvDescriptorSize;
	};
}