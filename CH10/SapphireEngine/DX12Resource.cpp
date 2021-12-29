#include "DX12Resource.h"

Sapphire::DX12Resource::DX12Resource(void* source, D3D12_RESOURCE_STATES state)
	: resource(static_cast<ID3D12Resource*>(source)), state(state)
{
}

Sapphire::DX12Resource::DX12Resource(DX12Device* device, UINT64 bufferSize)
	: resource(nullptr), state(D3D12_RESOURCE_STATE_GENERIC_READ)
{
	auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	ExitIfFailed(device->GetDevice()->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE,
		&desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&resource)));
}

Sapphire::DX12Resource::~DX12Resource()
{
	SafeRelease(&resource);
}

void Sapphire::DX12Resource::Upload(void* source, size_t size)
{
	UINT8* pVertexDataBegin;
	CD3DX12_RANGE readRange(0, 0);
	ExitIfFailed(resource->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
	memcpy(pVertexDataBegin, source, size);
	resource->Unmap(0, nullptr);
}

D3D12_GPU_VIRTUAL_ADDRESS Sapphire::DX12Resource::GetGpuVirtualAddress()
{
	return resource->GetGPUVirtualAddress();
}

ID3D12Resource* Sapphire::DX12Resource::GetResource()
{
	return resource;
}
