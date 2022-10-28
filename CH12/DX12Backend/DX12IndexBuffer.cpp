#include "pch.h"

#include "DX12IndexBuffer.h"
#include "DX12Resource.h"

GpuApi::DX12IndexBuffer::DX12IndexBuffer(DX12Device* device, void* indexData, UINT indexSize, UINT numOfIndices)
	: indexBuffer(nullptr), numOfIndices(numOfIndices)
{
	UINT indexBufferSize = indexSize * numOfIndices;
	indexBuffer = DX12Resource::CreateUpload(device, indexBufferSize);
	indexBuffer->Upload(indexData, indexBufferSize);

	indexBufferView.BufferLocation = indexBuffer->GetGpuVirtualAddress();
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;
	indexBufferView.SizeInBytes = indexBufferSize;
}

GpuApi::DX12IndexBuffer::~DX12IndexBuffer()
{
}

UINT GpuApi::DX12IndexBuffer::GetNumOfIndices()
{
	return numOfIndices;
}
