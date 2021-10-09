#pragma once

#include <d3d12.h>

#include "Logger.h"
#include "Utils.h"

namespace Sapphire
{
	class DX12CommandList
	{
		friend class DX12CommandQueue;
	public:
		DX12CommandList(ID3D12Device* device);
		~DX12CommandList();
		void SetResourceBarrier(D3D12_RESOURCE_BARRIER& resourceBarrier);
		void SetRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE& cpuDescriptorHandle);
		void ClearRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE& cpuDescriptorHandle, const float* color);
		void Close();
		void Reset();
		// CH08
		void SetViewport(D3D12_VIEWPORT viewport);
		void SetScissors(D3D12_RECT scissorRect);
		void SetGraphicsRootSignature(ID3D12RootSignature* rootSignature);
		// CH09
		void SetPipelineState(ID3D12PipelineState* pipelineState);
	private:
		ID3D12CommandAllocator* commandAllocator;
		ID3D12GraphicsCommandList* commandList;
	};
}