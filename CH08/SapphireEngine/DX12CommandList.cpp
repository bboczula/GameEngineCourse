#include "DX12CommandList.h"

Sapphire::DX12CommandList::DX12CommandList(ID3D12Device* device)
{
	Logger::GetInstance().Log("%s\n", "Sapphire::DX12CommandList::DX12CommandList");

	ExitIfFailed(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator)));
	ExitIfFailed(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, nullptr, IID_PPV_ARGS(&commandList)));
	commandList->Close();
}

Sapphire::DX12CommandList::~DX12CommandList()
{
	SafeRelease(&commandList);
	SafeRelease(&commandAllocator);
}

void Sapphire::DX12CommandList::TransitionTo(DX12RenderTarget* renderTarget, D3D12_RESOURCE_STATES nextState)
{
	D3D12_RESOURCE_BARRIER barrier;
	ZeroMemory(&barrier, sizeof(barrier));
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = renderTarget->resource;
	barrier.Transition.StateBefore = renderTarget->resourceState;
	barrier.Transition.StateAfter = nextState;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	commandList->ResourceBarrier(1, &barrier);

	renderTarget->resourceState = nextState;
}

void Sapphire::DX12CommandList::SetRenderTarget(DX12RenderTarget* renderTarget)
{
	commandList->OMSetRenderTargets(1, &renderTarget->descriptorHandle, FALSE, nullptr);
}

void Sapphire::DX12CommandList::SetViewport(D3D12_VIEWPORT viewport)
{
	commandList->RSSetViewports(1, &viewport);
}

void Sapphire::DX12CommandList::SetScissors(D3D12_RECT scissorRect)
{
	commandList->RSSetScissorRects(1, &scissorRect);
}

void Sapphire::DX12CommandList::SetPipelineState(DX12PipelineState* pipelineState)
{
	commandList->SetGraphicsRootSignature(pipelineState->rootSignature);
	commandList->SetPipelineState(pipelineState->pipelineState);
}

void Sapphire::DX12CommandList::Draw(D3D12_VERTEX_BUFFER_VIEW vbv)
{
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->IASetVertexBuffers(0, 1, &vbv);
	commandList->DrawInstanced(3, 1, 0, 0);
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
