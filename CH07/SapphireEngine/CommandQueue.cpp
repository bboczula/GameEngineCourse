#include "CommandQueue.h"

Sapphire::CommandQueue::CommandQueue(ID3D12Device* device) : fenceValue(1)
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::CreateCommandQueue()");

	CreateCommandQueue(device);
	CreateFence(device);
	CreateEventObject();
	Flush();
}

Sapphire::CommandQueue::~CommandQueue()
{
	SafeRelease(&fence);
	SafeRelease(&commandQueue);
}

void Sapphire::CommandQueue::CreateCommandQueue(ID3D12Device* device)
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

void Sapphire::CommandQueue::CreateFence(ID3D12Device* device)
{
	// Create a new fence
	ExitIfFailed(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));
	fence->SetName(L"MyFence");
}

void Sapphire::CommandQueue::CreateEventObject()
{
	fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (fenceEvent == nullptr)
	{
		ExitIfFailed(HRESULT_FROM_WIN32(GetLastError()));
	}
}

void Sapphire::CommandQueue::Flush()
{
	Signal();
	WaitForGpu();
}

void Sapphire::CommandQueue::Signal()
{
	ExitIfFailed(commandQueue->Signal(fence, fenceValue));
}

void Sapphire::CommandQueue::WaitForGpu()
{
	// Wait until the previous frame is finished.
	if (fence->GetCompletedValue() < fenceValue)
	{
		ExitIfFailed(fence->SetEventOnCompletion(fenceValue, fenceEvent));
		WaitForSingleObject(fenceEvent, INFINITE);
	}
	fenceValue++;
}

void Sapphire::CommandQueue::Execute(CommandList* commandList)
{
	ID3D12CommandList* commandListArray[] = { commandList->commandList };
	commandQueue->ExecuteCommandLists(_countof(commandListArray), commandListArray);
	Flush();
}

ID3D12CommandQueue* Sapphire::CommandQueue::Get()
{
	return commandQueue;
}
