#include "pch.h"

#include "DX12ConstantBuffer.h"
#include "DX12Resource.h"
#include "DX12Device.h"

GpuApi::DX12ConstantBuffer::DX12ConstantBuffer(DX12Device* device, UINT64 bufferSize, D3D12_CPU_DESCRIPTOR_HANDLE srvDescriptorHandle, UINT descriptorIndex)
	: descriptorIndex(descriptorIndex)
{
	// Allocate the heap to fit the new texture buffer
	constantBuffer = DX12Resource::CreateUpload(device, 256);

	// Create shader resource view
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.BufferLocation = constantBuffer->GetGpuVirtualAddress();
	cbvDesc.SizeInBytes = 256;

	device->GetDevice()->CreateConstantBufferView(&cbvDesc, srvDescriptorHandle);
}

GpuApi::DX12ConstantBuffer::~DX12ConstantBuffer()
{
}

UINT GpuApi::DX12ConstantBuffer::GetDescriptorIndex()
{
	return descriptorIndex;
}

void GpuApi::DX12ConstantBuffer::UploadFloat4(float a, float b, float c, float d)
{
	// Upload some hardcoded data
	float lightPosition[] = { a, b, c, d };
	constantBuffer->Upload(lightPosition, sizeof(float) * 4);
}
