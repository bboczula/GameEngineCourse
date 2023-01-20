#include "pch.h"

#include "DX12Resource.h"
#include "DX12Device.h"

Sapphire::DX12Resource::DX12Resource(void* source, D3D12_RESOURCE_STATES state)
	: resource(static_cast<ID3D12Resource*>(source)), currentState(state), previousState(state)
{
}

Sapphire::DX12Resource::DX12Resource(DX12Device* device, UINT64 bufferSize)
	: resource(nullptr), currentState(D3D12_RESOURCE_STATE_GENERIC_READ), previousState(currentState)
{
	auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	AExitIfFailed(device->GetDevice()->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE,
		&desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&resource)));
}

Sapphire::DX12Resource::DX12Resource(DX12Device* device, DXGI_FORMAT textureFormat, UINT width, UINT height)
	: currentState(D3D12_RESOURCE_STATE_COPY_DEST), previousState(currentState)
{
	// For now this will cover the Depth Buffer
	auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	auto desc = CD3DX12_RESOURCE_DESC::Tex2D(textureFormat, width, height);
	desc.MipLevels = 1;

	AExitIfFailed(device->GetDevice()->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE,
		&desc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&resource)));
}

Sapphire::DX12Resource::DX12Resource(DX12Device* device, DXGI_FORMAT textureFormat, UINT width, UINT height, D3D12_RESOURCE_FLAGS flags)
	: currentState(D3D12_RESOURCE_STATE_COPY_DEST), previousState(currentState)
{
	auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	auto desc = CD3DX12_RESOURCE_DESC::Tex2D(textureFormat, width, height);
	desc.MipLevels = 1;
	desc.Flags |= flags;

	bool isClearValueRequired = (flags == D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL) || (flags == D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
	if (isClearValueRequired)
	{
		D3D12_CLEAR_VALUE clearValue;
		ZeroMemory(&clearValue, sizeof(D3D12_CLEAR_VALUE));
		clearValue.Format = textureFormat;
		if (flags == D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL)
		{
			clearValue.DepthStencil.Depth = 1.0f;
			currentState = D3D12_RESOURCE_STATE_DEPTH_WRITE;
			previousState = currentState;
		}
		else if (flags == D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET)
		{
			// We've picked this color because we clear render target to get the sky, we will change it later
			// Looks like arrays are not really assignable, so I have to copy instead
			const FLOAT defaultClearColor[] = { 0.1176f, 0.1882f, 0.4470f, 1.0f };
			std::copy(defaultClearColor, defaultClearColor + 4, clearValue.Color);
		}
		AExitIfFailed(device->GetDevice()->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE,
			&desc, currentState, &clearValue, IID_PPV_ARGS(&resource)));
	}
	else
	{
		AExitIfFailed(device->GetDevice()->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE,
			&desc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&resource)));
	}
}

Sapphire::DX12Resource::~DX12Resource()
{
	ASafeRelease(&resource);
}

void Sapphire::DX12Resource::Upload(void* source, size_t size)
{
	UINT8* resourceData;
	CD3DX12_RANGE readRange(0, 0);
	AExitIfFailed(resource->Map(0, &readRange, reinterpret_cast<void**>(&resourceData)));
	memcpy(resourceData, source, size);
	resource->Unmap(0, nullptr);
}

void Sapphire::DX12Resource::Upload(float* source, UINT width, UINT heigh, UINT pixelSize, UINT alignedRowPitch)
{
	UINT8* resourceData;
	CD3DX12_RANGE readRange(0, 0);
	AExitIfFailed(resource->Map(0, &readRange, reinterpret_cast<void**>(&resourceData)));
	
	// For each bitmap row...
	for (int y = 0; y < heigh; y++)
	{
		UINT8* scan = resourceData + y * alignedRowPitch;
		memcpy(scan, &(source[y * width * 4]), width * pixelSize);
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

D3D12_RESOURCE_STATES Sapphire::DX12Resource::GetState()
{
	return currentState;
}

D3D12_RESOURCE_STATES Sapphire::DX12Resource::GetPreviousState()
{
	return previousState;
}

void Sapphire::DX12Resource::SetState(D3D12_RESOURCE_STATES state)
{
	previousState = currentState;
	currentState = state;
}
