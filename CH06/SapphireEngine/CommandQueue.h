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
		/// <summary>
		/// Creates new CommandQeueue instance.
		/// </summary>
		/// <param name="device">DirectX 12 device used to create new Command Queue</param>
		CommandQueue(ID3D12Device* device);

		~CommandQueue();

		/// <summary>
		/// Returns a pointer for wrapped DirectX 12 Command Queue interface.
		/// </summary>
		/// <returns>A raw pointer to ID3D12CommandQueue</returns>
		ID3D12CommandQueue* Get();

		/// <summary>
		/// Executes provided command list and fulshes the GPU queue.
		/// </summary>
		/// <param name="commandList">Single command list to execute</param>
		void Execute(ID3D12CommandList* commandList);

		/// <summary>
		/// Increments the GPU fence and halts current CPU thread until this value is reached.
		/// </summary>
		void Flush();
	private:
		ID3D12CommandQueue* commandQueue;
		Fence* fence;
	};
}

