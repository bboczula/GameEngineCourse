#include "DX12CommandQueue.h"

Sapphire::DX12CommandQueue::DX12CommandQueue(ID3D12Device* device) : fenceValue(1)
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::CreateCommandQueue()");

	CreateCommandQueue(device);
	CreateFence(device);
	CreateEventObject();
	Flush();
}

Sapphire::DX12CommandQueue::~DX12CommandQueue()
{
	SafeRelease(&fence);
	SafeRelease(&commandQueue);
}

void Sapphire::DX12CommandQueue::CreateCommandQueue(ID3D12Device* device)
{
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc;
	ZeroMemory(&commandQueueDesc, sizeof(commandQueueDesc));

	commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	commandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	commandQueueDesc.NodeMask = 0;

	ExitIfFailed(device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue)));
	commandQueue->SetName(L"MyCommandQueue");
}

void Sapphire::DX12CommandQueue::CreateFence(ID3D12Device* device)
{
	// Create a new fence
	ExitIfFailed(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));
	fence->SetName(L"MyFence");
}

void Sapphire::DX12CommandQueue::CreateEventObject()
{
	fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (fenceEvent == nullptr)
	{
		ExitIfFailed(HRESULT_FROM_WIN32(GetLastError()));
	}
}

void Sapphire::DX12CommandQueue::Flush()
{
	Signal();
	WaitForGpu();
}

void Sapphire::DX12CommandQueue::Signal()
{
	ExitIfFailed(commandQueue->Signal(fence, fenceValue));
}

void Sapphire::DX12CommandQueue::WaitForGpu()
{
	// Wait until the previous frame is finished.
	if (fence->GetCompletedValue() < fenceValue)
	{
		ExitIfFailed(fence->SetEventOnCompletion(fenceValue, fenceEvent));
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
