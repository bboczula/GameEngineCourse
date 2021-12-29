#include "DX12Geometry.h"

Sapphire::DX12Geometry::DX12Geometry(DX12Device* device, void* triangleVertices, UINT vertexBufferSize, UINT vertexSize, UINT numOfVertices)
{
	Logger::GetInstance().Log("%s\n", "Sapphire::DX12Geometry::DX12Geometry()");
	vertexBuffer = new DX12Resource(device, vertexBufferSize);
	vertexBuffer->Upload(triangleVertices, vertexBufferSize);

	vertexBufferView.BufferLocation = vertexBuffer->GetGpuVirtualAddress();
	vertexBufferView.StrideInBytes = vertexSize;
	vertexBufferView.SizeInBytes = vertexBufferSize;
}

Sapphire::DX12Geometry::~DX12Geometry()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::DX12Geometry::~DX12Geometry()");

	delete vertexBuffer;
}