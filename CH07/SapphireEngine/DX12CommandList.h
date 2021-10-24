#pragma once

#include <d3d12.h>

#include "Logger.h"
#include "Utils.h"
#include "DX12RenderTarget.h"

namespace Sapphire
{
	class DX12CommandList
	{
		friend class DX12CommandQueue;
	public:
		DX12CommandList(ID3D12Device* device);
		~DX12CommandList();
		void TransitionTo(DX12RenderTarget* renderTarget, D3D12_RESOURCE_STATES newResourceState);
		void SetRenderTarget(DX12RenderTarget* renderTarget);
		void ClearRenderTarget(DX12RenderTarget* renderTarget, const float* color);
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