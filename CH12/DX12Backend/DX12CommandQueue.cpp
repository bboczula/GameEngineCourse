#include "pch.h"

#include "DX12CommandQueue.h"
#include "DX12Device.h"
#include "DX12CommandList.h"

Sapphire::DX12CommandQueue::DX12CommandQueue(DX12Device* device) : fenceValue(1)
{
	CreateCommandQueue(device);
	CreateFence(device);
	CreateEventObject();
	Flush();
}

Sapphire::DX12CommandQueue::~DX12CommandQueue()
{
	ASafeRelease(&fence);
	ASafeRelease(&commandQueue);
}

void Sapphire::DX12CommandQueue::CreateCommandQueue(DX12Device* device)
{
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc;
	ZeroMemory(&commandQueueDesc, sizeof(commandQueueDesc));

	commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	commandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	commandQueueDesc.NodeMask = 0;

	AExitIfFailed(device->GetDevice()->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue)));
	commandQueue->SetName(L"MyCommandQueue");
}

void Sapphire::DX12CommandQueue::CreateFence(DX12Device* device)
{
	// Create a new fence
	AExitIfFailed(device->GetDevice()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));
	fence->SetName(L"MyFence");
}

void Sapphire::DX12CommandQueue::CreateEventObject()
{
	fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (fenceEvent == nullptr)
	{
		AExitIfFailed(HRESULT_FROM_WIN32(GetLastError()));
	}
}

void Sapphire::DX12CommandQueue::Flush()
{
	Signal();
	WaitForGpu();
}

void Sapphire::DX12CommandQueue::Signal()
{
	AExitIfFailed(commandQueue->Signal(fence, fenceValue));
}

void Sapphire::DX12CommandQueue::WaitForGpu()
{
	// Wait until the previous frame is finished.
	if (fence->GetCompletedValue() < fenceValue)
	{
		AExitIfFailed(fence->SetEventOnCompletion(fenceValue, fenceEvent));
		WaitForSingleObject(fenceEvent, INFINITE);
	}
	fenceValue++;
}

void Sapphire::DX12CommandQueue::Execute(DX12CommandList* commandList)
{
	ID3D12CommandList* commandListArray[] = { commandList->commandList };
	commandQueue->ExecuteCommandLists(_countof(commandListArray), commandListArray);
	Flush();
}

ID3D12CommandQueue* Sapphire::DX12CommandQueue::Get()
{
	return commandQueue;
}
