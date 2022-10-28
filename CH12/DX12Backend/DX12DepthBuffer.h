#pragma once

#include "DX12Device.h"
#include "DX12Resource.h"

namespace GpuApi
{
	class DX12DepthBuffer
	{
		friend class DX12CommandList;
	public:
		DX12DepthBuffer(DX12Device* device, D3D12_CPU_DESCRIPTOR_HANDLE dsvDescriptorHandle, UINT width, UINT height);
		DX12DepthBuffer(DX12Device* device, D3D12_CPU_DESCRIPTOR_HANDLE dsvDescriptorHandle, UINT width, UINT height, D3D12_CPU_DESCRIPTOR_HANDLE srvDescriptorHandle, UINT descriptorIndex);
		~DX12DepthBuffer();
		UINT GetDescriptorIndex();
		DX12Resource* GetResource();
	private:
		DX12Resource* depthBuffer;
		D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle;
		UINT descriptorIndex;
	};
}