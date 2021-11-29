#include "DxgiManager.h"

Sapphire::DxgiManager::DxgiManager()
{
	CreateDxgiFactory();
	GetCapabilities();
	EnumAdapters();
}

Sapphire::DxgiManager::~DxgiManager()
{
	SafeRelease(&dxgiAdapter);
	SafeRelease(&dxgiFactory);
}

void Sapphire::DxgiManager::CreateSwapChain(DX12CommandQueue* commandQueue, HWND hwnd, bool vsyncEnabled)
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
	if (!vsyncEnabled && hardwareCapabilities.getCapability(Capabilities::ALLOW_TEARING))
	{
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
	}

	IDXGISwapChain1* tempSwapChain;
	ExitIfFailed(dxgiFactory->CreateSwapChainForHwnd(commandQueue->Get(), hwnd, &swapChainDesc, nullptr, nullptr, &tempSwapChain));

	tempSwapChain->QueryInterface(IID_PPV_ARGS(&dxgiSwapChain));
	tempSwapChain->Release();

	DisableDxgiMsgQueueMonitoring(hwnd);
}

void Sapphire::DxgiManager::PresentFrame(bool vsyncEnabled)
{
	if (!vsyncEnabled && hardwareCapabilities.getCapability(Capabilities::ALLOW_TEARING))
	{
		ExitIfFailed(dxgiSwapChain->Present(0, DXGI_PRESENT_ALLOW_TEARING));
	}
	else
	{
		ExitIfFailed(dxgiSwapChain->Present(4, 0));
	}
	currentFrameIndex = dxgiSwapChain->GetCurrentBackBufferIndex();
}

void Sapphire::DxgiManager::DisableDxgiMsgQueueMonitoring(HWND hwnd)
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::MakeWindowAssociation()");

	ExitIfFailed(dxgiFactory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_WINDOW_CHANGES));
}

void Sapphire::DxgiManager::CreateDxgiFactory()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::CreateDxgiFactory()");

	ExitIfFailed(CreateDXGIFactory2(0, IID_PPV_ARGS(&dxgiFactory)));
}

void Sapphire::DxgiManager::GetCapabilities()
{
	BOOL isAllowedTearing = false;
	if (SUCCEEDED(dxgiFactory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &isAllowedTearing, sizeof(isAllowedTearing))))
	{
		Logger::GetInstance().Log("%s\n", "Hardware allows tearing.");
		hardwareCapabilities.setCapability(Capabilities::ALLOW_TEARING);
	}
}

void Sapphire::DxgiManager::EnumAdapters()
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

void Sapphire::DxgiManager::LogAdapterInfo(IDXGIAdapter1* adapter)
{
	DXGI_ADAPTER_DESC1 adapterDesc;
	adapter->GetDesc1(&adapterDesc);
	Logger::GetInstance().Log("  %ws (Dedicated VRAM: %zu MB)\n", adapterDesc.Description, adapterDesc.DedicatedVideoMemory / 1024 / 1024);
}

void Sapphire::DxgiManager::EnumerateOutputs(IDXGIAdapter1* currentAdapter)
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

void Sapphire::DxgiManager::LogOutputInfo(IDXGIOutput* output)
{
	DXGI_OUTPUT_DESC outputDesc;
	output->GetDesc(&outputDesc);
	Logger::GetInstance().Log("    Attached Output: %ws\n", outputDesc.DeviceName);
}
