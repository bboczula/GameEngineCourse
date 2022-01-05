#include "Renderer.h"

Sapphire::Renderer::Renderer(HWND hwnd, LONG width, LONG height)
	: viewport({ 0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, 1.0f }),
	scissorRect({ 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) })
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
		// The default state of this resource is Common, we need to remember to set it accordingly
		renderTargets[i] = new DX12RenderTarget(device, dxResources[i], rtvHandle);
	}
	commandList = new DX12CommandList(device);
	vertexShader = new DX12Shader("bypass_vs.cso");
	pixelShader = new DX12Shader("bypass_ps.cso");
	dxPipelineState = new DX12PipelineState(device, vertexShader, pixelShader);
	
	// CH09 - And this shapes to be RenderObject
	CreateVertexBuffer(width, height);
}

Sapphire::Renderer::~Renderer()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::~Renderer()");

	delete triangle;
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
	RecordCommandList();
	ExecuteCommandList();
	PresentFrame();
}

void Sapphire::Renderer::RecordCommandList()
{
	const float clearColor[] = { 0.1176f, 0.1882f, 0.4470f, 1.0f };
	unsigned int currentFrameIndex = dxgiManager->currentFrameIndex;

	commandList->Reset();
	commandList->TransitionTo(dxResources[currentFrameIndex], D3D12_RESOURCE_STATE_RENDER_TARGET);
	commandList->SetPipelineState(dxPipelineState);
	commandList->SetRenderTarget(renderTargets[currentFrameIndex]);
	commandList->ClearRenderTarget(renderTargets[currentFrameIndex], clearColor);

	// CH09
	commandList->SetViewport(viewport);
	commandList->SetScissors(scissorRect);
	commandList->Draw(triangle);

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

void Sapphire::Renderer::CreateVertexBuffer(LONG width, LONG height)
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::CreateVertexBuffer()");

	// Define the geometry for a triangle.
	Math::Point3D triangleVertices[] =
	{
		{  0.0f,   0.5f, 0.0f },
		{  0.25f, -0.5f, 0.0f },
		{ -0.25f, -0.5f, 0.0f }
	};

	triangle = new DX12Geometry(device, triangleVertices, sizeof(Math::Point3D) * 3, sizeof(Math::Point3D), 3);
}
