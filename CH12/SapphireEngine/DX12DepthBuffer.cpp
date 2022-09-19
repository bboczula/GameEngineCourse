#include "DX12DepthBuffer.h"

Sapphire::DX12DepthBuffer::DX12DepthBuffer(DX12Device* device, D3D12_CPU_DESCRIPTOR_HANDLE dsvDescriptorHandle, UINT width, UINT height)
	: descriptorHandle(dsvDescriptorHandle)
{
	// Create Depth Stencil View
	D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilView;
	ZeroMemory(&depthStencilView, sizeof(D3D12_DEPTH_STENCIL_VIEW_DESC));
	depthStencilView.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilView.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	depthStencilView.Flags = D3D12_DSV_FLAG_NONE;
	depthStencilView.Texture2D.MipSlice = 0;

	// Now here comes GPU memory management
	// This is terrible, you have to find a better way to handle resource creation
	//depthBuffer = new DX12Resource(device);
	depthBuffer = DX12Resource::CreateDepthBuffer(device, width, height);

	device->GetDevice()->CreateDepthStencilView(depthBuffer->GetResource(), &depthStencilView, dsvDescriptorHandle);
}

Sapphire::DX12DepthBuffer::DX12DepthBuffer(DX12Device* device, D3D12_CPU_DESCRIPTOR_HANDLE dsvDescriptorHandle, UINT width, UINT height, D3D12_CPU_DESCRIPTOR_HANDLE srvDescriptorHandle, UINT descriptorIndex)
	: descriptorHandle(dsvDescriptorHandle), descriptorIndex(descriptorIndex)
{
	// Create Depth Stencil View
	D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilView;
	ZeroMemory(&depthStencilView, sizeof(D3D12_DEPTH_STENCIL_VIEW_DESC));
	depthStencilView.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilView.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	depthStencilView.Flags = D3D12_DSV_FLAG_NONE;
	depthStencilView.Texture2D.MipSlice = 0;

	// Now here comes GPU memory management
	// This is terrible, you have to find a better way to handle resource creation
	//depthBuffer = new DX12Resource(device);
	depthBuffer = DX12Resource::CreateDepthBuffer(device, width, height);

	device->GetDevice()->CreateDepthStencilView(depthBuffer->GetResource(), &depthStencilView, dsvDescriptorHandle);

	// Create Shader Resrouce View
	D3D12_SHADER_RESOURCE_VIEW_DESC shaderResourceView;
	ZeroMemory(&shaderResourceView, sizeof(D3D12_SHADER_RESOURCE_VIEW_DESC));
	shaderResourceView.Format = DXGI_FORMAT_R32_FLOAT;
	shaderResourceView.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	shaderResourceView.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	shaderResourceView.Texture2D.MipLevels = 1;

	device->GetDevice()->CreateShaderResourceView(depthBuffer->GetResource(), &shaderResourceView, srvDescriptorHandle);
}

Sapphire::DX12DepthBuffer::~DX12DepthBuffer()
{
	delete depthBuffer;
}

UINT Sapphire::DX12DepthBuffer::GetDescriptorIndex()
{
	return descriptorIndex;
}

Sapphire::DX12Resource* Sapphire::DX12DepthBuffer::GetResource()
{
	return depthBuffer;
}
