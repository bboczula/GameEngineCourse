#pragma once

#include <d3d12.h>

#include "Logger.h"
#include "Utils.h"
#include "Fence.h"

namespace Sapphire
{
	/// <summary>
	/// A wrapper class for the ID3D12CommandQueue interface. Contains the Fence  for synchronization.
	/// </summary>
	class CommandQueue
	{
	public:
		CommandQueue(ID3D12Device* device);
		~CommandQueue();
		ID3D12CommandQueue* Get();
		void Execute(ID3D12CommandList* commandList);
		void Flush();
	private:
		ID3D12CommandQueue* commandQueue;
		Fence* fence;
	};
}

