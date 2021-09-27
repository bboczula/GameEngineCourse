#include "Renderer.h"

Sapphire::Renderer::Renderer(HWND hwnd, LONG width, LONG height) : hwnd(hwnd), width(width), height(height)
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::Renderer()");

	CreateDxgiFactory();
	GetCapabilites();
	EnumerateAdapters();
	CreateDevice();

	// Device dependent stuff
	CreateCommandQueue();
	CreateCommandList();
	CreateSwapChain();
	DisableDxgiMsgQueueMonitoring();

	// Create frame resources
	CreateDescriptorHeap();
	CreateFrameResources();
}

Sapphire::Renderer::~Renderer()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::~Renderer()");

	
	SafeRelease(&rtvHeap);
	SafeRelease(&dxgiSwapChain);
	delete commandList;
	delete commandQueue;
	SafeRelease(&device);
	SafeRelease(&dxgiAdapter);
	SafeRelease(&dxgiFactory);
}

void Sapphire::Renderer::Render()
{
	ResetCommandList();
	RecordCommandList();
	CloseCommandList();
	ExecuteCommandList();
	PresentFrame();
}

void Sapphire::Renderer::CreateDxgiFactory()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::CreateDxgiFactory()");

	ExitIfFailed(CreateDXGIFactory2(0, IID_PPV_ARGS(&dxgiFactory)));
}

void Sapphire::Renderer::EnumerateAdapters()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::EnumerateAdapters()");
	IDXGIAdapter1* currentAdapter;
	UINT index = 0;
	while (1)
	{
		HRESULT result = dxgiFactory->EnumAdapters1(index++, &currentAdapter);
		if (result == DXGI_ERROR_NOT_FOUND)
		{
			break;
		}
		ExitIfFailed(result);

		LogAdapterInfo(currentAdapter);
		EnumerateOutputs(currentAdapter);
		SafeRelease(&currentAdapter);
	}
	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
	// Finally we pick the first adapter
	ExitIfFailed(dxgiFactory->EnumAdapters1(0, &dxgiAdapter));
}

void Sapphire::Renderer::EnumerateOutputs(IDXGIAdapter1* currentAdapter)
{
	UINT index = 0;
	IDXGIOutput* output;
	while (1)
	{
		HRESULT result = currentAdapter->EnumOutputs(index++, &output);
		if (result == DXGI_ERROR_NOT_FOUND)
		{
			break;
		}
		ExitIfFailed(result);

		LogOutputInfo(output);
		SafeRelease(&output);
	}
}

void Sapphire::Renderer::LogOutputInfo(IDXGIOutput* output)
{
	DXGI_OUTPUT_DESC outputDesc;
	output->GetDesc(&outputDesc);
	Logger::GetInstance().Log("    Attached Output: %ws\n", outputDesc.DeviceName);
}

void Sapphire::Renderer::LogAdapterInfo(IDXGIAdapter1* adapter)
{
	DXGI_ADAPTER_DESC1 adapterDesc;
	adapter->GetDesc1(&adapterDesc);
	Logger::GetInstance().Log("  %ws (Dedicated VRAM: %zu MB)\n", adapterDesc.Description, adapterDesc.DedicatedVideoMemory / 1024 / 1024);
}

void Sapphire::Renderer::GetCapabilites()
{
	BOOL isAllowedTearing = false;
	if (SUCCEEDED(dxgiFactory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &isAllowedTearing, sizeof(isAllowedTearing))))
	{
		Logger::GetInstance().Log("%s\n", "Hardware allows tearing.");
		hardwareCapabilities.setCapability(Capabilities::ALLOW_TEARING);
	}
}

void Sapphire::Renderer::CreateDevice()
{
#if _DEBUG
	EnableDebugLayer();
#endif

	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::CreateDevice()");

	ExitIfFailed(D3D12CreateDevice(dxgiAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device)));
}

void Sapphire::Renderer::CreateCommandQueue()
{
	commandQueue = new CommandQueue(device);
}

void Sapphire::Renderer::CreateCommandList()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::CreateCommandList()");

	commandList = new CommandList(device);
}

void Sapphire::Renderer::CreateDescriptorHeap()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::CreateDescriptorHeap()");

	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc;
	ZeroMemory(&rtvDescHeapDesc, sizeof(rtvDescHeapDesc));
	rtvDescHeapDesc.NumDescriptors = FRAME_COUNT;
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	ExitIfFailed(device->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&rtvHeap)));
	rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
}

void Sapphire::Renderer::CreateFrameResources()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::CreateFrameResources()");

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
	rtvHandle.ptr = SIZE_T(INT64(rtvHeap->GetCPUDescriptorHandleForHeapStart().ptr));

	for (UINT i = 0; i < FRAME_COUNT; i++)
	{
		ExitIfFailed(dxgiSwapChain->GetBuffer(i, IID_PPV_ARGS(&renderTargets[i])));
		device->CreateRenderTargetView(renderTargets[i], nullptr, rtvHandle);
		rtvHandle.ptr += INT64(rtvDescriptorSize);
	}
}

void Sapphire::Renderer::CreateSwapChain()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::CreateSwapChain()");

	DXGI_SAMPLE_DESC sampleDesc;
	ZeroMemory(&sampleDesc, sizeof(sampleDesc));
	sampleDesc.Count = 1;
	sampleDesc.Quality = 0;

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.Width = 0;
	swapChainDesc.Height = 0;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = FRAME_COUNT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc = sampleDesc;
	if (IsVsyncDisabledAndTearingAllowed())
	{
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
	}

	IDXGISwapChain1* tempSwapChain;
	ExitIfFailed(dxgiFactory->CreateSwapChainForHwnd(commandQueue->Get(), hwnd, &swapChainDesc, nullptr, nullptr, &tempSwapChain));

	tempSwapChain->QueryInterface(IID_PPV_ARGS(&dxgiSwapChain));
	tempSwapChain->Release();
}

void Sapphire::Renderer::DisableDxgiMsgQueueMonitoring()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::MakeWindowAssociation()");

	ExitIfFailed(dxgiFactory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_WINDOW_CHANGES));
}

void Sapphire::Renderer::ResetCommandList()
{
	commandList->Reset();
}

void Sapphire::Renderer::RecordCommandList()
{
	Logger::GetInstance().Log("currentFrameIndex %d\n", currentFrameIndex);
	
	// Indicate that the back buffer will be used as a render target.
	D3D12_RESOURCE_BARRIER barrier;
	ZeroMemory(&barrier, sizeof(barrier));
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = renderTargets[currentFrameIndex];
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	commandList->SetResourceBarrier(barrier);
	
	D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle;
	ZeroMemory(&descriptorHandle, sizeof(descriptorHandle));
	descriptorHandle.ptr = SIZE_T(INT64(rtvHeap->GetCPUDescriptorHandleForHeapStart().ptr) + INT64(currentFrameIndex) * INT64(rtvDescriptorSize));
	
	commandList->SetRenderTarget(descriptorHandle);
	const float clearColor[] = { 0.0f, currentFrameIndex ? 0.3f : 0.2f, 0.4f, 1.0f };
	commandList->ClearRenderTarget(descriptorHandle, clearColor);
	
	// Transition back to resource
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	commandList->SetResourceBarrier(barrier);
}

void Sapphire::Renderer::CloseCommandList()
{
	commandList->Close();
}

void Sapphire::Renderer::ExecuteCommandList()
{
	commandQueue->Execute(commandList);
}

void Sapphire::Renderer::PresentFrame()
{
	if (IsVsyncDisabledAndTearingAllowed())
	{
		ExitIfFailed(dxgiSwapChain->Present(0, DXGI_PRESENT_ALLOW_TEARING));
	}
	else
	{
		ExitIfFailed(dxgiSwapChain->Present(4, 0));
	}
	currentFrameIndex = dxgiSwapChain->GetCurrentBackBufferIndex();
}

void Sapphire::Renderer::EnableDebugLayer()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::EnableDebugLayer()");

	// API level validation
	ID3D12Debug* debugInterface_0;
	ExitIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface_0)));
	debugInterface_0->EnableDebugLayer();

	// GPU level validation
	ID3D12Debug1* debugInterface_1;
	ExitIfFailed(debugInterface_0->QueryInterface(IID_PPV_ARGS(&debugInterface_1)));
	debugInterface_1->SetEnableGPUBasedValidation(true);
}

bool Sapphire::Renderer::IsVsyncDisabledAndTearingAllowed()
{
	return (!settings.isVsyncEnabled) && (hardwareCapabilities.getCapability(Capabilities::ALLOW_TEARING));
}
