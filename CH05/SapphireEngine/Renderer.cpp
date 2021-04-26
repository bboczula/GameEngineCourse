#include "Renderer.h"

Sapphire::Renderer::Renderer(HWND hwnd, LONG width, LONG height) : hwnd(hwnd), width(width), height(height)
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::Renderer()");

	CreateDxgiFactory();
	EnumerateAdapters();
	CreateDevice();
	CreateCommandQueue();
	CreateCommandAllocator();
	CreateCommandList();
	//CreateSwapChain();
	//CreateSyncObjects();
}

Sapphire::Renderer::~Renderer()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::~Renderer()");

	SafeRelease(&dxgiAdapter);
	SafeRelease(&dxgiFactory);
}

void Sapphire::Renderer::Render()
{
	//ResetCommandList();
	//CloseCommandList();
	//ExecuteCommandList();
	//PresentFrame();
	//WaitForPreviousFrame();
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

void Sapphire::Renderer::CreateDevice()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::CreateDevice()");

#if _DEBUG
	EnableDebugLayer();
#endif

	ExitIfFailed(D3D12CreateDevice(dxgiAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device)));
}

void Sapphire::Renderer::CreateCommandQueue()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::CreateCommandQueue()");

	D3D12_COMMAND_QUEUE_DESC commandQueueDesc;
	ZeroMemory(&commandQueueDesc, sizeof(commandQueueDesc));

	commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	commandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	commandQueueDesc.NodeMask = 0;

	ExitIfFailed(device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&graphicsCommandQueue)));
}

void Sapphire::Renderer::CreateCommandAllocator()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::CreateCommandAllocator()");

	ExitIfFailed(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator)));
}

void Sapphire::Renderer::CreateCommandList()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::CreateCommandList()");

	ExitIfFailed(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, nullptr, IID_PPV_ARGS(&commandList)));
	commandList->Close();
}

void Sapphire::Renderer::CreateSwapChain()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::CreateSwapChain()");

	DXGI_MODE_DESC dxgiModeDesc;
	dxgiModeDesc.Width = width;
	dxgiModeDesc.Height = height;
	dxgiModeDesc.RefreshRate = DXGI_RATIONAL({ 0, 1 });
	dxgiModeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiModeDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	dxgiModeDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	DXGI_SAMPLE_DESC dxgiSampleDesc;
	dxgiSampleDesc.Count = 1;
	dxgiSampleDesc.Quality = 0;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferDesc = dxgiModeDesc;
	swapChainDesc.SampleDesc = dxgiSampleDesc;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = FRAME_COUNT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	swapChainDesc.Flags = 0;

	IDXGISwapChain* tempSwapChain;
	ExitIfFailed(dxgiFactory->CreateSwapChain(graphicsCommandQueue, &swapChainDesc, &tempSwapChain));

	// We need to "upcast" this to SwapChain3
	tempSwapChain->QueryInterface(IID_PPV_ARGS(&dxgiSwapChain));

	// Release the tempSwapChain
	//tempSwapChain->Release();
}

void Sapphire::Renderer::CreateSyncObjects()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::CreateSyncObjects()");
	
	ExitIfFailed(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));
	fenceValue = 1;

	// Create an event handle to use for frame synchronization.
	fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (fenceEvent == nullptr)
	{
		ExitIfFailed(HRESULT_FROM_WIN32(GetLastError()));
	}

	// Wait for the command list to execute; we are reusing the same command 
	// list in our main loop but for now, we just want to wait for setup to 
	// complete before continuing.
	WaitForPreviousFrame();
}

void Sapphire::Renderer::ResetCommandList()
{
	ExitIfFailed(commandAllocator->Reset());
	ExitIfFailed(commandList->Reset(commandAllocator, nullptr));
}

void Sapphire::Renderer::RecordCommandList()
{
}

void Sapphire::Renderer::CloseCommandList()
{
	ExitIfFailed(commandList->Close());
}

void Sapphire::Renderer::ExecuteCommandList()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::ExecuteCommandList()");

	ID3D12CommandList* commandListArray[] = { commandList };
	graphicsCommandQueue->ExecuteCommandLists(_countof(commandListArray), commandListArray);
}

void Sapphire::Renderer::PresentFrame()
{
	//Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::PresentFrame()");

	//Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::PresentFrame()");
	ExitIfFailed(dxgiSwapChain->Present(1, 0));
	//Sleep(20);
}

void Sapphire::Renderer::WaitForPreviousFrame()
{
	// WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
	// This is code implemented as such for simplicity. More advanced samples 
	// illustrate how to use fences for efficient resource usage.

	// Signal and increment the fence value.
	const UINT64 tempFence = fenceValue;
	ExitIfFailed(graphicsCommandQueue->Signal(fence, tempFence));
	fenceValue++;

	// Wait until the previous frame is finished.
	if (fence->GetCompletedValue() < tempFence)
	{
		ExitIfFailed(fence->SetEventOnCompletion(tempFence, fenceEvent));
		WaitForSingleObject(fenceEvent, INFINITE);
	}

	// Get the current Back Buffer index
	currentFrameIndex = dxgiSwapChain->GetCurrentBackBufferIndex();
}

void Sapphire::Renderer::EnableDebugLayer()
{
	// API level validation
	ID3D12Debug* debugInterface_0;
	ExitIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface_0)));
	debugInterface_0->EnableDebugLayer();

	// GPU level validation
	ID3D12Debug1* debugInterface_1;
	ExitIfFailed(debugInterface_0->QueryInterface(IID_PPV_ARGS(&debugInterface_1)));
	debugInterface_1->SetEnableGPUBasedValidation(true);
}
