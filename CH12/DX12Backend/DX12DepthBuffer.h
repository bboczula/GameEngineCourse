#pragma once

#ifdef DX12BACKEND_EXPORTS
#define GPUAPI_DX12_API __declspec(dllexport)
#else
#define GPUAPI_DX12_API __declspec(dllimport)
#endif

#include <d3d12.h>

namespace Sapphire
{
	class DX12Device;
	class DX12Resource;

	class GPUAPI_DX12_API DX12DepthBuffer
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