#pragma once

#include "DX12Resource.h"

namespace GpuApi
{
	class DX12Texture
	{
	public:
		DX12Texture(DX12Device* device, D3D12_CPU_DESCRIPTOR_HANDLE srvDescriptorHandle, UINT width, UINT height, UINT descriptorIndex);
		DX12Resource* GetResource();
		~DX12Texture();
		UINT GetDescriptorIndex();
	private:
		DX12Resource* textureBuffer;
		UINT descriptorIndex;
	};
}