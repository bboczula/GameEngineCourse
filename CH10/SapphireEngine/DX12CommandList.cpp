#include "DX12CommandList.h"

Sapphire::DX12CommandList::DX12CommandList(DX12Device* device)
{
	Logger::GetInstance().Log("%s\n", "Sapphire::DX12CommandList::DX12CommandList");

	ExitIfFailed(device->GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator)));
	ExitIfFailed(device->GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, nullptr, IID_PPV_ARGS(&commandList)));
	commandList->Close();
}

Sapphire::DX12CommandList::~DX12CommandList()
{
	SafeRelease(&commandList);
	SafeRelease(&commandAllocator);
}

void Sapphire::DX12CommandList::TransitionTo(DX12Resource* resource, D3D12_RESOURCE_STATES nextState)
{
	D3D12_RESOURCE_BARRIER barrier;
	ZeroMemory(&barrier, sizeof(barrier));
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = resource->resource;
	barrier.Transition.StateBefore = resource->state;
	barrier.Transition.StateAfter = nextState;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	commandList->ResourceBarrier(1, &barrier);

	resource->state = nextState;
}

void Sapphire::DX12CommandList::SetRenderTarget(DX12RenderTarget* renderTarget)
{
	commandList->OMSetRenderTargets(1, &renderTarget->descriptorHandle, FALSE, nullptr);
}

void Sapphire::DX12CommandList::SetPipelineState(DX12PipelineState* pipelineState)
{
	commandList->SetGraphicsRootSignature(pipelineState->rootSignature);
	commandList->SetPipelineState(pipelineState->pipelineState);
}

void Sapphire::DX12CommandList::ClearRenderTarget(DX12RenderTarget* renderTarget, const float* color)
{
	commandList->ClearRenderTargetView(renderTarget->descriptorHandle, color, 0, nullptr);
}

void Sapphire::DX12CommandList::Close()
{
	ExitIfFailed(commandList->Close());
}

void Sapphire::DX12CommandList::Reset()
{
	ExitIfFailed(commandAllocator->Reset());
	ExitIfFailed(commandList->Reset(commandAllocator, nullptr));
}
