#include "Renderer.h"

Sapphire::Renderer::Renderer(HWND hwnd, LONG width, LONG height)
	: hwnd(hwnd), width(width), height(height),
	viewport({ 0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, 1.0f }),
	scissorRect({ 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) })
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::Renderer()");

	// This kind of shapes up as DX12DeviceContext
	dxgiManager = new DxgiManager;
	device = new DX12Device(dxgiManager->dxgiAdapter);
	commandQueue = new DX12CommandQueue(device->GetDevice());
	dxgiManager->CreateSwapChain(commandQueue, hwnd, settings.isVsyncEnabled);

	// And this kind of shapes up as DX12RenderContext
	commandList = new DX12CommandList(device->GetDevice());
	rtvDescriptorHeap = new DX12DescriptorHeap(device->GetDevice(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
	for (UINT i = 0; i < FRAME_COUNT; i++)
	{
		// This one is really hard to undangle. Maybe I can add this to the public interface?
		ExitIfFailed(dxgiManager->dxgiSwapChain->GetBuffer(i, IID_PPV_ARGS(&resources[i])));
		rtvHandle.ptr = rtvDescriptorHeap->AllocateDescriptor();
		// The default state of this resource is Common, we need to remember to set it accordingly
		renderTargets[i] = new DX12RenderTarget(device->GetDevice(), resources[i], rtvHandle, D3D12_RESOURCE_STATE_COMMON);
	}

	ShaderCompiler shaderCompiler;

	pixelShader = new DX12Shader;
	pixelShader->Compile(L"bypass.hlsl", SHADER_TYPE::PIXEL_SHADER, &shaderCompiler);

	vertexShader = new DX12Shader;
	vertexShader->Compile(L"bypass.hlsl", SHADER_TYPE::VERTEX_SHADER, &shaderCompiler);

	dxPipelineState = new DX12PipelineState(device->GetDevice(), vertexShader, pixelShader);
	
	// CH09 - And this shapes to be RenderObject
	CreateVertexBuffer();
}

Sapphire::Renderer::~Renderer()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::~Renderer()");

	SafeRelease(&vertexBuffer);
	delete dxPipelineState;
	delete pixelShader;
	delete vertexShader;

	for (int i = 0; i < FRAME_COUNT; i++)
	{
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
	commandList->TransitionTo(renderTargets[currentFrameIndex], D3D12_RESOURCE_STATE_RENDER_TARGET);
	commandList->SetPipelineState(dxPipelineState);
	commandList->SetRenderTarget(renderTargets[currentFrameIndex]);
	commandList->ClearRenderTarget(renderTargets[currentFrameIndex], currentFrameIndex ? clearColorOne : clearColorTwo);

	// CH09
	commandList->SetViewport(viewport);
	commandList->SetScissors(scissorRect);
	commandList->Draw(vertexBufferView);
	// CH09
	
	commandList->TransitionTo(renderTargets[currentFrameIndex], D3D12_RESOURCE_STATE_PRESENT);
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

void Sapphire::Renderer::CreateVertexBuffer()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::CreateVertexBuffer()");

	// Define the geometry for a triangle.
	Vertex triangleVertices[] =
	{
		{ { 0.0f, 0.25f * static_cast<float>(width) / static_cast<float>(height), 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
		{ { 0.25f, -0.25f * static_cast<float>(width) / static_cast<float>(height), 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ { -0.25f, -0.25f * static_cast<float>(width) / static_cast<float>(height), 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
	};

	const UINT vertexBufferSize = sizeof(triangleVertices);

	// Note: using upload heaps to transfer static data like vert buffers is not 
	// recommended. Every time the GPU needs it, the upload heap will be marshalled 
	// over. Please read up on Default Heap usage. An upload heap is used here for 
	// code simplicity and because there are very few verts to actually transfer.
	// CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);

	D3D12_HEAP_PROPERTIES heapProps;
	heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProps.CreationNodeMask = 1;
	heapProps.VisibleNodeMask = 1;

	//auto desc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
	D3D12_RESOURCE_DESC desc;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Alignment = 0;
	desc.Width = vertexBufferSize;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.Flags = D3D12_RESOURCE_FLAG_NONE;

	ExitIfFailed(device->GetDevice()->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexBuffer)));

	// Copy the triangle data to the vertex buffer.
	UINT8* pVertexDataBegin;
	//CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
	D3D12_RANGE readRange;
	readRange.Begin = 0;
	readRange.End = 0;
	ExitIfFailed(vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
	memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));
	vertexBuffer->Unmap(0, nullptr);

	// Initialize the vertex buffer view.
	vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
	vertexBufferView.StrideInBytes = sizeof(Vertex);
	vertexBufferView.SizeInBytes = vertexBufferSize;
}
