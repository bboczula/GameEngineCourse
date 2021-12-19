#include "Renderer.h"

Sapphire::Renderer::Renderer(HWND hwnd, LONG width, LONG height)
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::Renderer()");

	dxgiManager = new DxgiManager;
	device = new DX12Device(dxgiManager->dxgiAdapter);
	commandQueue = new DX12CommandQueue(device);
	dxgiManager->CreateSwapChain(commandQueue, hwnd, settings.isVsyncEnabled);
	rtvDescriptorHeap = new DX12DescriptorHeap(device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	for (UINT i = 0; i < FRAME_COUNT; i++)
	{
		ID3D12Resource* tempResource;
		ExitIfFailed(dxgiManager->dxgiSwapChain->GetBuffer(i, IID_PPV_ARGS(&tempResource)));
		dxResources[i] = new DX12Resource(tempResource, D3D12_RESOURCE_STATE_COMMON);

		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
		rtvHandle.ptr = rtvDescriptorHeap->AllocateDescriptor();
		renderTargets[i] = new DX12RenderTarget(device, dxResources[i], rtvHandle);
	}

	commandList = new DX12CommandList(device);
	vertexShader = new DX12Shader("bypass_vs.cso");
	pixelShader = new DX12Shader("bypass_ps.cso");
}

Sapphire::Renderer::~Renderer()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::~Renderer()");

	delete pixelShader;
	delete vertexShader;
	for (int i = 0; i < FRAME_COUNT; i++)
	{
		delete dxResources[i];
		delete renderTargets[i];
	}
	delete rtvDescriptorHeap;
	delete commandList;
	delete commandQueue;
	delete dxgiManager;
	delete device;
}

void Sapphire::Renderer::Render()
{
	RecordCommandList();
	ExecuteCommandList();
	PresentFrame();
}

void Sapphire::Renderer::RecordCommandList()
{
	const float clearColorOne[] = { 0.3098f, 0.4509f, 0.7490f, 1.0f };
	const float clearColorTwo[] = { 0.1176f, 0.1882f, 0.4470f, 1.0f };
	unsigned int currentFrameIndex = dxgiManager->currentFrameIndex;

	commandList->Reset();
	commandList->TransitionTo(dxResources[currentFrameIndex], D3D12_RESOURCE_STATE_RENDER_TARGET);
	commandList->SetRenderTarget(renderTargets[currentFrameIndex]);
	commandList->ClearRenderTarget(renderTargets[currentFrameIndex], currentFrameIndex ? clearColorOne : clearColorTwo);
	commandList->TransitionTo(dxResources[currentFrameIndex], D3D12_RESOURCE_STATE_PRESENT);
	commandList->Close();
}

void Sapphire::Renderer::ExecuteCommandList()
{
	commandQueue->Execute(commandList);
}

void Sapphire::Renderer::PresentFrame()
{
	dxgiManager->PresentFrame(settings.isVsyncEnabled);
}
