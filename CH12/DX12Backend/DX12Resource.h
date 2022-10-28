#pragma once

#include "Utils.h"
#include "DX12Device.h"
#include "d3dx12.h"

#include <d3d12.h>

namespace GpuApi
{
	class DX12Resource
	{
		friend class DX12CommandList;
	public:
		static DX12Resource* CreateEmpty(void* source, D3D12_RESOURCE_STATES state)
		{
			return new DX12Resource(source, state);
		}
		static DX12Resource* CreateUpload(DX12Device* device, UINT64 bufferSize)
		{
			return new DX12Resource(device, bufferSize);
		}
		static DX12Resource* CreateTexture2D(DX12Device* device, DXGI_FORMAT format, UINT width, UINT height)
		{
			return new DX12Resource(device, format, width, height);
		}
		static DX12Resource* CreateRenderTarget(DX12Device* device, DXGI_FORMAT format, UINT width, UINT height)
		{
			return new DX12Resource(device, format, width, height, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
		}
		static DX12Resource* CreateDepthBuffer(DX12Device* device, UINT width, UINT height)
		{
			return new DX12Resource(device, DXGI_FORMAT_D32_FLOAT, width, height, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
		}
		//DX12Resource(DX12Device* device);
		~DX12Resource();
		void Upload(void* source, size_t size);
		void Upload(float* source, UINT width, UINT heigh, UINT pixelSize, UINT alignedRowPitch);
		D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress();
		ID3D12Resource* GetResource();
	protected:
		DX12Resource(void* source, D3D12_RESOURCE_STATES state);
		DX12Resource(DX12Device* device, UINT64 bufferSize);
		DX12Resource(DX12Device* device, DXGI_FORMAT textureFormat, UINT width, UINT height);
		DX12Resource(DX12Device* device, DXGI_FORMAT textureFormat, UINT width, UINT height, D3D12_RESOURCE_FLAGS flags);
	private:
		D3D12_RESOURCE_STATES state;
		ID3D12Resource* resource;
	};
}