#include "CommandList.h"

Sapphire::CommandList::CommandList(ID3D12Device* device)
{
	Logger::GetInstance().Log("%s\n", "Sapphire::CommandList::CommandList");

	ExitIfFailed(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator)));
	ExitIfFailed(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, nullptr, IID_PPV_ARGS(&commandList)));
	commandList->Close();
}

Sapphire::CommandList::~CommandList()
{
	SafeRelease(&commandList);
	SafeRelease(&commandAllocator);
}

void Sapphire::CommandList::SetResourceBarrier(D3D12_RESOURCE_BARRIER& resourceBarrier)
{
	commandList->ResourceBarrier(1, &resourceBarrier);
}

void Sapphire::CommandList::SetRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE& cpuDescriptorHandle)
{
	commandList->OMSetRenderTargets(1, &cpuDescriptorHandle, FALSE, nullptr);
}

void Sapphire::CommandList::SetViewport(D3D12_VIEWPORT viewport)
{
	commandList->RSSetViewports(1, &viewport);
}

void Sapphire::CommandList::SetScissors(D3D12_RECT scissorRect)
{
	commandList->RSSetScissorRects(1, &scissorRect);
}

void Sapphire::CommandList::SetGraphicsRootSignature(ID3D12RootSignature* rootSignature)
{
	commandList->SetGraphicsRootSignature(rootSignature);
}

void Sapphire::CommandList::SetPipelineState(ID3D12PipelineState* pipelineState)
{
	commandList->SetPipelineState(pipelineState);
}

void Sapphire::CommandList::ClearRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE& cpuDescriptorHandle, const float* color)
{
	commandList->ClearRenderTargetView(cpuDescriptorHandle, color, 0, nullptr);
}

void Sapphire::CommandList::Close()
{
	ExitIfFailed(commandList->Close());
}

void Sapphire::CommandList::Reset()
{
	ExitIfFailed(commandAllocator->Reset());
	ExitIfFailed(commandList->Reset(commandAllocator, nullptr));
}
