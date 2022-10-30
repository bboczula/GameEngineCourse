#pragma once

#ifdef DX12BACKEND_EXPORTS
#define GPUAPI_DX12_API __declspec(dllexport)
#else
#define GPUAPI_DX12_API __declspec(dllimport)
#endif

#include <d3d12.h>

namespace Sapphire
{
	class DX12Resource;
	class DX12Device;

	extern class GPUAPI_DX12_API DX12Texture
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