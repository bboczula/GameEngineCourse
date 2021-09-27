#pragma once

#include <d3d12.h>

#include "Logger.h"
#include "Utils.h"

namespace Sapphire
{
	class CommandList
	{
	public:
		CommandList(ID3D12Device* device);
		~CommandList();
		ID3D12CommandList* Get();
		void SetResourceBarrier(D3D12_RESOURCE_BARRIER& resourceBarrier);
		void SetRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE& cpuDescriptorHandle);
		void ClearRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE& cpuDescriptorHandle, const float* color);
		void Close();
		void Reset();
	private:
		ID3D12CommandAllocator* commandAllocator;
		ID3D12GraphicsCommandList* commandList;
	};
}