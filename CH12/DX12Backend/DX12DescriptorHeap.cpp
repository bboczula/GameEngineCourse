#include "pch.h"

#include "DX12DescriptorHeap.h"

#include "DX12Device.h"

#define HEAP_SIZE 1000

Sapphire::DX12DescriptorHeap::DX12DescriptorHeap(DX12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type)
{
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc;
	ZeroMemory(&rtvDescHeapDesc, sizeof(rtvDescHeapDesc));
	rtvDescHeapDesc.NumDescriptors = HEAP_SIZE;
	rtvDescHeapDesc.Type = type;
	rtvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	if (type == D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
	{
		// This type of Descriptor Heap needs to be shader-visible
		rtvDescHeapDesc.Flags |= D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	}
	AExitIfFailed(device->GetDevice()->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&heap)));
	descriptorSize = device->GetDevice()->GetDescriptorHandleIncrementSize(type);
	baseIndex = 0;
}

Sapphire::DX12DescriptorHeap::~DX12DescriptorHeap()
{
	ASafeRelease(&heap);
}

SIZE_T Sapphire::DX12DescriptorHeap::AllocateDescriptor()
{
	ExitIfTrue(baseIndex >= HEAP_SIZE, L"Descriptor Heap out of space.");
	return SIZE_T(INT64(heap->GetCPUDescriptorHandleForHeapStart().ptr) + INT64(baseIndex++) * INT64(descriptorSize));
}

D3D12_GPU_DESCRIPTOR_HANDLE Sapphire::DX12DescriptorHeap::GetFirstGpuDescriptor()
{
	return heap->GetGPUDescriptorHandleForHeapStart();
}

UINT Sapphire::DX12DescriptorHeap::GetDescriptorSize()
{
	return descriptorSize;
}

UINT Sapphire::DX12DescriptorHeap::GetHeapSize()
{
	return baseIndex;
}
