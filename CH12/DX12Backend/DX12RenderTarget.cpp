#include "pch.h"

#include "DX12RenderTarget.h"

GpuApi::DX12RenderTarget::DX12RenderTarget(DX12Device* device, DX12Resource* resource, D3D12_CPU_DESCRIPTOR_HANDLE descriptor)
	: descriptorHandle(descriptor), resource(resource)
{
	device->GetDevice()->CreateRenderTargetView(resource->GetResource(), nullptr, descriptorHandle);
	viewport = new DX12Viewport(1280, 720);
}

GpuApi::DX12RenderTarget::DX12RenderTarget(DX12Device* device, D3D12_CPU_DESCRIPTOR_HANDLE descriptor, UINT width, UINT height)
	: descriptorHandle(descriptor), resource(nullptr)
{
	resource = DX12Resource::CreateRenderTarget(device, DXGI_FORMAT_R8G8B8A8_UNORM, width, height);
	device->GetDevice()->CreateRenderTargetView(resource->GetResource(), nullptr, descriptorHandle);
	viewport = new DX12Viewport(width, height);
}

GpuApi::DX12Resource* GpuApi::DX12RenderTarget::GetResource()
{
	return resource;
}

GpuApi::DX12RenderTarget::~DX12RenderTarget()
{
	delete viewport;
}
