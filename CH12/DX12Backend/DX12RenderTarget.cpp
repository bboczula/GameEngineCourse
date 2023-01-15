#include "pch.h"

#include "DX12RenderTarget.h"
#include "Utils.h"
#include "DX12Device.h"
#include "DX12Resource.h"
#include "DX12Viewport.h"

Sapphire::DX12RenderTarget::DX12RenderTarget(DX12Device* device, DX12Resource* resource, D3D12_CPU_DESCRIPTOR_HANDLE descriptor)
	: descriptorHandle(descriptor), resource(resource), format(Format::UNDEFINED)
{
	device->GetDevice()->CreateRenderTargetView(resource->GetResource(), nullptr, descriptorHandle);
	viewport = new DX12Viewport(1280, 720);
}

Sapphire::DX12RenderTarget::DX12RenderTarget(DX12Device* device, D3D12_CPU_DESCRIPTOR_HANDLE descriptor, UINT width, UINT height, Format format)
	: descriptorHandle(descriptor), resource(nullptr), format(format)
{
	auto dxgiFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	if (format == Format::RGBA16_FLOAT)
	{
		dxgiFormat = DXGI_FORMAT_R16G16B16A16_FLOAT;
	}
	resource = DX12Resource::CreateRenderTarget(device, dxgiFormat, width, height);
	device->GetDevice()->CreateRenderTargetView(resource->GetResource(), nullptr, descriptorHandle);
	viewport = new DX12Viewport(width, height);
}

Sapphire::DX12RenderTarget::Format Sapphire::DX12RenderTarget::GetFormat()
{
	return format;
}

DXGI_FORMAT Sapphire::DX12RenderTarget::GetDxgiFormat()
{
	switch (format)
	{
	case Sapphire::DX12RenderTarget::RGBA8_UNORM:
		return DXGI_FORMAT_R8G8B8A8_UNORM;
	case Sapphire::DX12RenderTarget::RGBA16_FLOAT:
		return DXGI_FORMAT_R16G16B16A16_FLOAT;
	case Sapphire::DX12RenderTarget::UNDEFINED:
		return DXGI_FORMAT_UNKNOWN;
	default:
		return DXGI_FORMAT_UNKNOWN;
	};
}

Sapphire::DX12Resource* Sapphire::DX12RenderTarget::GetResource()
{
	return resource;
}

Sapphire::DX12RenderTarget::~DX12RenderTarget()
{
	delete viewport;
}
