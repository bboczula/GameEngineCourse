#include "pch.h"

#include "DX12Texture.h"
#include "DX12Resource.h"
#include "DX12Device.h"

Sapphire::DX12Texture::DX12Texture(DX12Device* device, D3D12_CPU_DESCRIPTOR_HANDLE srvDescriptorHandle, UINT width, UINT height, UINT descriptorIndex)
	: textureBuffer(nullptr), descriptorIndex(descriptorIndex)
{
	// Allocate the heap to fit the new texture buffer
	textureBuffer = DX12Resource::CreateTexture2D(device, DXGI_FORMAT_R32G32B32A32_FLOAT, width, height);

	// Create shader resource view
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture2D.MipLevels = 1;

	device->GetDevice()->CreateShaderResourceView(textureBuffer->GetResource(), &srvDesc, srvDescriptorHandle);
}

Sapphire::DX12Resource* Sapphire::DX12Texture::GetResource()
{
	return textureBuffer;
}

Sapphire::DX12Texture::~DX12Texture()
{
	delete textureBuffer;
}

UINT Sapphire::DX12Texture::GetDescriptorIndex()
{
	return descriptorIndex;
}
