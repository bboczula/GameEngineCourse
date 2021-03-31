#include "Renderer.h"

Sapphire::Renderer::Renderer()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::Renderer()");

	CreateDxgiFactory();
	EnumerateAdapters();
}

Sapphire::Renderer::~Renderer()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::~Renderer()");

	SafeRelease(dxgiAdapter.GetAddressOf());
	SafeRelease(dxgiFactory.GetAddressOf());
}

void Sapphire::Renderer::CreateDxgiFactory()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::CreateDxgiFactory()");
	ExitIfFailed(CreateDXGIFactory2(0, IID_PPV_ARGS(&dxgiFactory)));
}

void Sapphire::Renderer::EnumerateAdapters()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::EnumerateAdapters()");
	Microsoft::WRL::ComPtr<IDXGIAdapter1> currentAdapter;
	UINT index = 0;
	while (1)
	{
		HRESULT result = dxgiFactory->EnumAdapters1(index++, currentAdapter.GetAddressOf());
		if (result == DXGI_ERROR_NOT_FOUND)
		{
			break;
		}
		ExitIfFailed(result);

		LogAdapterInfo(currentAdapter.Get());
		EnumerateOutputs(currentAdapter.Get());
	}

	// Finally we pick the first adapter
	ExitIfFailed(dxgiFactory->EnumAdapters1(0, &dxgiAdapter));
}

void Sapphire::Renderer::EnumerateOutputs(IDXGIAdapter1* currentAdapter)
{
	UINT index = 0;
	Microsoft::WRL::ComPtr<IDXGIOutput> output;
	while (1)
	{
		HRESULT result = currentAdapter->EnumOutputs(index++, output.GetAddressOf());
		if (result == DXGI_ERROR_NOT_FOUND)
		{
			break;
		}
		ExitIfFailed(result);

		LogOutputInfo(output.Get());
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
