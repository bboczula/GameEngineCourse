#include "CommandQueue.h"

Sapphire::CommandQueue::CommandQueue(ID3D12Device* device)
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::CreateCommandQueue()");

	D3D12_COMMAND_QUEUE_DESC commandQueueDesc;
	ZeroMemory(&commandQueueDesc, sizeof(commandQueueDesc));

	commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	commandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	commandQueueDesc.NodeMask = 0;

	ExitIfFailed(device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue)));

	// Create a new fence
	fence = new Fence(commandQueue);
}

Sapphire::CommandQueue::~CommandQueue()
{
	delete fence;
	SafeRelease(&commandQueue);
}

ID3D12CommandQueue* Sapphire::CommandQueue::Get()
{
	return commandQueue;
}

void Sapphire::CommandQueue::Execute(ID3D12CommandList* commandList)
{
	ID3D12CommandList* commandListArray[] = { commandList };
	commandQueue->ExecuteCommandLists(_countof(commandListArray), commandListArray);
	Flush();
}

void Sapphire::CommandQueue::Flush()
{
	fence->FlushGpuQueue();
}
