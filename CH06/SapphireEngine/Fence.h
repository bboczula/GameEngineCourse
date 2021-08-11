#pragma once

#include <d3d12.h>

#include "Logger.h"
#include "Utils.h"

namespace Sapphire
{
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
		void Signal(const UINT64& tempFence);
		void CpuWait(const UINT64& tempFence);
		ID3D12CommandQueue* boundCommandQueue;
		HANDLE fenceEvent;
		UINT64 fenceValue;
		ID3D12Fence* fence;
	};
}

