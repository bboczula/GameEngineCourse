#include "Fence.h"

Sapphire::Fence::Fence(ID3D12CommandQueue* commandQueue) : boundCommandQueue(nullptr)
{
	// Introduce asserts to make sure this is not nullptr

	Logger::GetInstance().Log("%s\n", "Sapphire::Fence::Fence()");

	// We need to store the the pointer to commandQueue interface.
	// Why can't we just copy the pointer, but rather query the interface?
	// This is interesting from the reference count perspective
	ExitIfFailed(commandQueue->QueryInterface(IID_PPV_ARGS(&boundCommandQueue)));

	CreateDxFence();

	CreateEventHandle();

	// Wait for the command list to execute; we are reusing the same command 
	// list in our main loop but for now, we just want to wait for setup to 
	// complete before continuing.
	FlushGpuQueue();
}

void Sapphire::Fence::CreateDxFence()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Fence::CreateDxFence()");

	// We can get the Device that created the Command Queue
	// We don't need to store the device, we do have to relese it though i think
	ID3D12Device* device;
	ExitIfFailed(boundCommandQueue->GetDevice(IID_PPV_ARGS(&device)));

	ExitIfFailed(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));
	fenceValue = 1;

	// We don't need the device anymore, we have to release it
	SafeRelease(&device);
}

void Sapphire::Fence::CreateEventHandle()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Fence::CreateEventHandle()");

	fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (fenceEvent == nullptr)
	{
		ExitIfFailed(HRESULT_FROM_WIN32(GetLastError()));
	}
}

Sapphire::Fence::~Fence()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Fence::~Fence()");

	CloseFenceHandle();

	SafeRelease(&fence);

	// Remember, we can't release the boundCommandQueue now, it is someone else responsibility
	// ...so, move semantics? smart pointers?
	// Or do we? since the ref count was increased
	SafeRelease(&boundCommandQueue);
}

void Sapphire::Fence::CloseFenceHandle()
{
	// Release and close the fence event
	if (fenceEvent)
	{
		CloseHandle(fenceEvent);
		fenceEvent = nullptr;
	}
}

void Sapphire::Fence::FlushGpuQueue()
{
	// WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
	// This is code implemented as such for simplicity. More advanced samples 
	// illustrate how to use fences for efficient resource usage.

	// This function drains the GPU Command Queue, so it blocks the calling thread until the work on the GPU is finished.

	const UINT64 tempFence = fenceValue;
	Signal(tempFence);
	CpuWait(tempFence);
}

void Sapphire::Fence::Signal(const UINT64& tempFence)
{
	ExitIfFailed(boundCommandQueue->Signal(fence, tempFence));
	fenceValue++;
}

void Sapphire::Fence::CpuWait(const UINT64& tempFence)
{
	// Wait until the previous frame is finished.
	if (fence->GetCompletedValue() < tempFence)
	{
		ExitIfFailed(fence->SetEventOnCompletion(tempFence, fenceEvent));
		WaitForSingleObject(fenceEvent, INFINITE);
	}
}
