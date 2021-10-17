#include "DX12RenderTarget.h"

Sapphire::DX12RenderTarget::DX12RenderTarget(ID3D12Device* device, ID3D12Resource* resource, D3D12_CPU_DESCRIPTOR_HANDLE descriptor) : descriptorHandle(descriptor)
{
	device->CreateRenderTargetView(resource, nullptr, descriptorHandle);
}

Sapphire::DX12RenderTarget::~DX12RenderTarget()
{
}
