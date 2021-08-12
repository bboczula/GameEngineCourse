#pragma once

#include <d3d12.h>

#include "Logger.h"
#include "Utils.h"

namespace Sapphire
{
	/// <summary>
	/// Implements the CPU/GPU DirectX 12 fence for Command Queue synchronization.
	/// </summary>
	class Fence
	{
	public:
		Fence(ID3D12CommandQueue* commandQueue);
		~Fence();
		void FlushGpuQueue();
	private:
		void CreateDxFence();
		void CreateEventHandle();
		void CloseFenceHandle();
		void Signal();
		void CpuWait();
		ID3D12CommandQueue* boundCommandQueue;
		HANDLE fenceEvent;
		UINT64 fenceValue;
		ID3D12Fence* fence;
	};
}

