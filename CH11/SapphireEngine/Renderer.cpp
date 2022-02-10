#include "Renderer.h"

Sapphire::Renderer::Renderer(HWND hwnd, LONG width, LONG height)
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::Renderer()");

	// This kind of shapes up as DX12DeviceContext
	dxgiManager = new DxgiManager;
	device = new DX12Device(dxgiManager->dxgiAdapter);
	commandQueue = new DX12CommandQueue(device);
	dxgiManager->CreateSwapChain(commandQueue, hwnd, settings.isVsyncEnabled);
	rtvDescriptorHeap = new DX12DescriptorHeap(device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// And this kind of shapes up as DX12RenderContext
	for (UINT i = 0; i < FRAME_COUNT; i++)
	{
		// Not really sure how to overcome this; i think i need this temporary pointer, I'll just won't touch the thing it points to
		ID3D12Resource* tempResource;

		// This one is really hard to undangle. Maybe I can add this to the public interface?
		ExitIfFailed(dxgiManager->dxgiSwapChain->GetBuffer(i, IID_PPV_ARGS(&tempResource)));
		dxResources[i] = new DX12Resource(tempResource, D3D12_RESOURCE_STATE_COMMON);

		// Maybe this temporary thing could be avoided if i return the entire handle?
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
		rtvHandle.ptr = rtvDescriptorHeap->AllocateDescriptor();
		renderTargets[i] = new DX12RenderTarget(device, dxResources[i], rtvHandle);
	}
	commandList = new DX12CommandList(device);
	vertexShader = new DX12Shader("bypass_vs.cso");
	pixelShader = new DX12Shader("bypass_ps.cso");
	dxPipelineState = new DX12PipelineState(device, vertexShader, pixelShader);
	viewport = new DX12Viewport(width, height);

	camera = new Camera;
}

Sapphire::Renderer::~Renderer()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::~Renderer()");

	delete camera;
	delete viewport;
	delete dxPipelineState;
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
	const float clearColor[] = { 0.1176f, 0.1882f, 0.4470f, 1.0f };
	unsigned int currentFrameIndex = dxgiManager->currentFrameIndex;

	commandList->Reset();
	commandList->TransitionTo(dxResources[currentFrameIndex], D3D12_RESOURCE_STATE_RENDER_TARGET);
	commandList->SetPipelineState(dxPipelineState);
	commandList->SetRenderTarget(renderTargets[currentFrameIndex]);
	commandList->ClearRenderTarget(renderTargets[currentFrameIndex], clearColor);
	commandList->SetViewport(viewport);
	commandList->SetConstantBuffer(0, 16, &camera->view);
	commandList->SetConstantBuffer(1, 16, &camera->projection);
	commandList->TransitionTo(dxResources[currentFrameIndex], D3D12_RESOURCE_STATE_PRESENT);
	commandList->Close();

	commandQueue->Execute(commandList);
	dxgiManager->PresentFrame(settings.isVsyncEnabled);
}
