#pragma once

#include <Windows.h>
#include <d3d12.h>

namespace Sapphire
{
	// Forward Declarations
	class DX12Resource;
	class DX12Device;

	class DX12ConstantBuffer
	{
	public:
		DX12ConstantBuffer(DX12Device* device, UINT64 bufferSize, D3D12_CPU_DESCRIPTOR_HANDLE srvDescriptorHandle, UINT descriptorIndex);
		~DX12ConstantBuffer();
		UINT GetDescriptorIndex();
		void UploadFloat4(float a, float b, float c, float d);
	private:
		DX12Resource* constantBuffer;
		UINT descriptorIndex;
	};
}