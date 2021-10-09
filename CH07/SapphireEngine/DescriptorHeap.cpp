#include "DescriptorHeap.h"

Sapphire::DX12DescriptorHeap::DX12DescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT size)
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::CreateDescriptorHeap()");

	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc;
	ZeroMemory(&rtvDescHeapDesc, sizeof(rtvDescHeapDesc));
	rtvDescHeapDesc.NumDescriptors = size;
	rtvDescHeapDesc.Type = type;
	rtvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	ExitIfFailed(device->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&rtvHeap)));
	rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(type);
}

Sapphire::DX12DescriptorHeap::~DX12DescriptorHeap()
{
}

SIZE_T Sapphire::DX12DescriptorHeap::GetCpuDescriptorHandle(UINT index)
{
	return SIZE_T(INT64(rtvHeap->GetCPUDescriptorHandleForHeapStart().ptr) + INT64(index) * INT64(rtvDescriptorSize));
}
