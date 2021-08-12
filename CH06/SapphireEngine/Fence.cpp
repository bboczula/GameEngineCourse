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

/// <summary>
/// Releases and closes the fence event.
/// </summary>
void Sapphire::Fence::CloseFenceHandle()
{
	if (fenceEvent)
	{
		CloseHandle(fenceEvent);
		fenceEvent = nullptr;
	}
}

/// <summary>
/// Increments the GPU fence value and halts current CPU thread until this value is reached.
/// </summary>
void Sapphire::Fence::FlushGpuQueue()
{
	// WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
	// This is code implemented as such for simplicity. More advanced samples 
	// illustrate how to use fences for efficient resource usage.

	// This function drains the GPU Command Queue, so it blocks the calling thread until the work on the GPU is finished.
	Signal();
	CpuWait();
	fenceValue++;
}

void Sapphire::Fence::Signal()
{
	ExitIfFailed(boundCommandQueue->Signal(fence, fenceValue));
}

void Sapphire::Fence::CpuWait()
{
	// Wait until the previous frame is finished.
	if (fence->GetCompletedValue() < fenceValue)
	{
		ExitIfFailed(fence->SetEventOnCompletion(fenceValue, fenceEvent));
		WaitForSingleObject(fenceEvent, INFINITE);
	}
}
