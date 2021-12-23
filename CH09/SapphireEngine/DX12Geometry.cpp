#include "DX12Geometry.h"

Sapphire::DX12Geometry::DX12Geometry(DX12Device* device, void* triangleVertices, UINT vertexBufferSize, UINT vertexSize, UINT numOfVertices)
{
	Logger::GetInstance().Log("%s\n", "Sapphire::DX12Geometry::DX12Geometry()");

	D3D12_HEAP_PROPERTIES heapProps;
	heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProps.CreationNodeMask = 1;
	heapProps.VisibleNodeMask = 1;

	//auto desc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
	D3D12_RESOURCE_DESC desc;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Alignment = 0;
	desc.Width = vertexBufferSize;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.Flags = D3D12_RESOURCE_FLAG_NONE;

	ExitIfFailed(device->GetDevice()->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexBuffer)));

	// Copy the triangle data to the vertex buffer.
	UINT8* pVertexDataBegin;
	//CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
	D3D12_RANGE readRange;
	readRange.Begin = 0;
	readRange.End = 0;
	ExitIfFailed(vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
	memcpy(pVertexDataBegin, triangleVertices, vertexBufferSize);
	vertexBuffer->Unmap(0, nullptr);

	// Initialize the vertex buffer view.
	vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
	vertexBufferView.StrideInBytes = vertexSize; // sizeof(Vertex);
	vertexBufferView.SizeInBytes = vertexBufferSize;
}

Sapphire::DX12Geometry::~DX12Geometry()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::DX12Geometry::~DX12Geometry()");
}