#include "pch.h"

#include "DX12VertexBuffer.h"
#include "DX12Resource.h"

GpuApi::DX12VertexBuffer::DX12VertexBuffer(DX12Device* device, void* vertexData, UINT vertexSize, UINT numOfVertices)
	: positionVertexBuffer(nullptr), numOfVertices(numOfVertices)
{
	UINT bufferSize = vertexSize * numOfVertices;
	positionVertexBuffer = DX12Resource::CreateUpload(device, bufferSize);
	positionVertexBuffer->Upload(vertexData, bufferSize);

	vertexBufferView.BufferLocation = positionVertexBuffer->GetGpuVirtualAddress();
	vertexBufferView.StrideInBytes = vertexSize;
	vertexBufferView.SizeInBytes = bufferSize;
}

GpuApi::DX12VertexBuffer::~DX12VertexBuffer()
{
	delete positionVertexBuffer;
}