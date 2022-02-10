#include "DX12Device.h"

Sapphire::DX12Device::DX12Device(IDXGIAdapter1* adapter)
{
#if _DEBUG
	EnableDebugLayer();
#endif

	ExitIfFailed(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device)));
}

Sapphire::DX12Device::~DX12Device()
{
	SafeRelease(&device);
}

ID3D12Device* Sapphire::DX12Device::GetDevice()
{
	return device;
}

void Sapphire::DX12Device::EnableDebugLayer()
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
