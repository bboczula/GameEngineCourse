#include "DX12ConstantBuffer.h"

#include "Logger.h"
#include "DX12Resource.h"
#include "DX12Device.h"

Sapphire::DX12ConstantBuffer::DX12ConstantBuffer(DX12Device* device, UINT64 bufferSize, D3D12_CPU_DESCRIPTOR_HANDLE srvDescriptorHandle, UINT descriptorIndex)
{
	Logger::GetInstance().Log("%s\n", "Sapphire::DX12Texture::DX12Texture()");

	// Allocate the heap to fit the new texture buffer
	constantBuffer = DX12Resource::CreateUpload(device, bufferSize);

	// Create shader resource view
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};

	device->GetDevice()->CreateConstantBufferView(&cbvDesc, srvDescriptorHandle);
}

Sapphire::DX12ConstantBuffer::~DX12ConstantBuffer()
{
}