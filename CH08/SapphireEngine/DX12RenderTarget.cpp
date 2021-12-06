#include "DX12RenderTarget.h"

Sapphire::DX12RenderTarget::DX12RenderTarget(DX12Device* device, DX12Resource* resource, D3D12_CPU_DESCRIPTOR_HANDLE descriptor)
	: descriptorHandle(descriptor), resource(resource)
{
	device->GetDevice()->CreateRenderTargetView(resource->GetResource(), nullptr, descriptorHandle);
}

Sapphire::DX12RenderTarget::~DX12RenderTarget()
{
}
