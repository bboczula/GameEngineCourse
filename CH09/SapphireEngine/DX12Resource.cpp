#include "DX12Resource.h"

Sapphire::DX12Resource::DX12Resource(void* source, D3D12_RESOURCE_STATES state)
	: resource(static_cast<ID3D12Resource*>(source)), state(state)
{
}

Sapphire::DX12Resource::~DX12Resource()
{
	SafeRelease(&resource);
}

ID3D12Resource* Sapphire::DX12Resource::GetResource()
{
	return resource;
}
