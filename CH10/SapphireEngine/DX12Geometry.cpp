#include "DX12Geometry.h"

Sapphire::DX12Geometry::DX12Geometry(DX12Device* device, void* triangleVertices, UINT vertexSize, UINT numOfVertices)
{
	Logger::GetInstance().Log("%s\n", "Sapphire::DX12Geometry::DX12Geometry()");

	UINT bufferSize = vertexSize * numOfVertices;

	vertexBuffer = new DX12Resource(device, bufferSize);
	vertexBuffer->Upload(triangleVertices, bufferSize);

	vertexBufferView.BufferLocation = vertexBuffer->GetGpuVirtualAddress();
	vertexBufferView.StrideInBytes = vertexSize;
	vertexBufferView.SizeInBytes = bufferSize;
}

Sapphire::DX12Geometry::~DX12Geometry()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::DX12Geometry::~DX12Geometry()");

	delete vertexBuffer;
}