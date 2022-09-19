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

Sapphire::DX12Resource::DX12Resource(DX12Device* device, DXGI_FORMAT textureFormat, UINT width, UINT height)
	: state(D3D12_RESOURCE_STATE_COPY_DEST)
{
	// For now this will cover the Depth Buffer
	auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	auto desc = CD3DX12_RESOURCE_DESC::Tex2D(textureFormat, width, height);

	ExitIfFailed(device->GetDevice()->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE,
		&desc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&resource)));
}

Sapphire::DX12Resource::DX12Resource(DX12Device* device, DXGI_FORMAT textureFormat, UINT width, UINT height, D3D12_RESOURCE_FLAGS flags)
	: state(D3D12_RESOURCE_STATE_COPY_DEST)
{
	auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	auto desc = CD3DX12_RESOURCE_DESC::Tex2D(textureFormat, width, height);
	desc.Flags |= flags;

	if (flags == D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL)
	{
		D3D12_CLEAR_VALUE depthClearValue;
		ZeroMemory(&depthClearValue, sizeof(D3D12_CLEAR_VALUE));
		depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;
		depthClearValue.DepthStencil.Depth = 1.0f;

		state = D3D12_RESOURCE_STATE_DEPTH_WRITE;

		ExitIfFailed(device->GetDevice()->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE,
			&desc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &depthClearValue, IID_PPV_ARGS(&resource)));
	}
	else
	{
		ExitIfFailed(device->GetDevice()->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE,
			&desc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&resource)));
	}
}

// Sapphire::DX12Resource::DX12Resource(DX12Device* device) : state(D3D12_RESOURCE_STATE_DEPTH_WRITE)
// {
// 	// For now this will cover the Depth Buffer
// 	auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
// 	auto desc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, 1280, 720);
// 	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
// 
// 	D3D12_CLEAR_VALUE depthClearValue;
// 	ZeroMemory(&depthClearValue, sizeof(D3D12_CLEAR_VALUE));
// 	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;
// 	depthClearValue.DepthStencil.Depth = 1.0f;
// 
// 	ExitIfFailed(device->GetDevice()->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE,
// 		&desc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &depthClearValue, IID_PPV_ARGS(&resource)));
// }

Sapphire::DX12Resource::~DX12Resource()
{
	SafeRelease(&resource);
}

void Sapphire::DX12Resource::Upload(void* source, size_t size)
{
	UINT8* resourceData;
	CD3DX12_RANGE readRange(0, 0);
	ExitIfFailed(resource->Map(0, &readRange, reinterpret_cast<void**>(&resourceData)));
	memcpy(resourceData, source, size);
	resource->Unmap(0, nullptr);
}

void Sapphire::DX12Resource::Upload(PixelDefinition* source, UINT width, UINT heigh, UINT pixelSize, UINT alignedRowPitch)
{
	UINT8* resourceData;
	CD3DX12_RANGE readRange(0, 0);
	ExitIfFailed(resource->Map(0, &readRange, reinterpret_cast<void**>(&resourceData)));
	
	// For each bitmap row...
	for (int y = 0; y < heigh; y++)
	{
		UINT8* scan = resourceData + y * alignedRowPitch;
		memcpy(scan, &(source[y * width]), width * pixelSize);
	}
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
