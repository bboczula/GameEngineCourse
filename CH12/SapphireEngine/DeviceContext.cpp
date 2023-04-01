#include "DeviceContext.h"

#include "TConfigVariable.h"

// Global Configuration Variables
TConfigVariable<bool> forceDebugLayer(false);

Sapphire::DeviceContext::DeviceContext(HWND hwnd, unsigned int width, unsigned int height)
{
	dxgiManager = new DxgiManager;
	device = new DX12Device(dxgiManager->dxgiAdapter, forceDebugLayer.Get());
	commandQueue = new DX12CommandQueue(device);
	dxgiManager->CreateSwapChain(commandQueue, hwnd, settings.isVsyncEnabled, width, height);
}

Sapphire::DeviceContext::~DeviceContext()
{
	delete commandQueue;
	delete device;
	delete dxgiManager;
}

Sapphire::DX12Resource* Sapphire::DeviceContext::GetBackBuffer(UINT index)
{
	// Not really sure how to overcome this; i think i need this temporary pointer, I'll just won't touch the thing it points to
	ID3D12Resource* tempResource;

	// This one is really hard to undangle. Maybe I can add this to the public interface?
	ExitIfFailed(dxgiManager->dxgiSwapChain->GetBuffer(index, IID_PPV_ARGS(&tempResource)));

	return DX12Resource::CreateEmpty(tempResource, D3D12_RESOURCE_STATE_COMMON);;
}

UINT Sapphire::DeviceContext::GetCurrentFrameIndex()
{
	return dxgiManager->currentFrameIndex;;
}

void Sapphire::DeviceContext::Present()
{
	dxgiManager->PresentFrame(settings.isVsyncEnabled);
}

Sapphire::DX12Device* Sapphire::DeviceContext::GetDevice()
{
	return device;
}

void Sapphire::DeviceContext::Execute(DX12CommandList* commandList)
{
	commandQueue->Execute(commandList);
}

Sapphire::DX12DescriptorHeap* Sapphire::DeviceContext::CreateRtvDescriptorHeap()
{
	return new DX12DescriptorHeap(device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
}

Sapphire::DX12DescriptorHeap* Sapphire::DeviceContext::CreateDsvDescriptorHeap()
{
	return new DX12DescriptorHeap(device, D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
}

Sapphire::DX12DescriptorHeap* Sapphire::DeviceContext::CreateSrvDescriptorHeap()
{
	return new DX12DescriptorHeap(device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

Sapphire::DX12CommandList* Sapphire::DeviceContext::CreateCommandList()
{
	return new DX12CommandList(device);
}
